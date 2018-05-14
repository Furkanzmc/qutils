#import "qutils/ios/iOSNativeUtils.h"
// iOS
#ifdef SAFARI_SERVICES_ENABLED
#import <SafariServices/SafariServices.h>
#endif // SAFARI_SERVICES_ENABLED
#if QUTILS_LOCATION_ENABLED == 1
#import <CoreLocation/CoreLocation.h>
#endif // QUTILS_LOCATION_ENABLED
#import <Photos/PHPhotoLibrary.h>
#import <UIKit/UIKit.h>
// Firebase
#if FCM_ENABLED == 1
#import <FirebaseMessaging/FirebaseMessaging.h>
#endif // FCM_ENABLED
// Local
#import "qutils/ios/QutilsViewDelegate.h"
#include "qutils/Macros.h"

namespace zmc
{

    static QutilsViewDelegate *m_QutilsDelegate = [[QutilsViewDelegate alloc] init];
    QList<iOSNativeUtils *> iOSNativeUtils::m_Instances = QList<iOSNativeUtils *>();

    iOSNativeUtils::iOSNativeUtils()
        : m_InstanceIndex(m_Instances.size())
        , m_IsImagePickerOpen(false)
        , m_IsAlertDialogVisible(false)
        , m_IsActionSheetDialogVisible(false)
        , m_IsPhotoAccessPermissionRequested(false)
        , m_IsCameraOpen(false)
    {
        if (m_Instances.size() == 0) {
            [[NSNotificationCenter defaultCenter] addObserverForName: UIKeyboardWillHideNotification object: nil queue: nil usingBlock: ^ (NSNotification * _Nonnull note) {
                Q_UNUSED(note);
                iOSNativeUtils::emitKeyboardHeightChangedSignals(0);
            }];

            [[NSNotificationCenter defaultCenter] addObserverForName: UIKeyboardWillShowNotification object: nil queue: nil usingBlock: ^ (NSNotification * _Nonnull note) {
                const float height = [note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue].size.height;
                iOSNativeUtils::emitKeyboardHeightChangedSignals(static_cast<int>(height));
            }];
        }

        m_Instances.append(this);
    }

    iOSNativeUtils::~iOSNativeUtils()
    {
        m_Instances[m_InstanceIndex] = nullptr;
        onAlertDialogClicked = nullptr;
        onActionSheetClicked = nullptr;

        onImagePickerControllerCancelled = nullptr;
        onImagePickerControllerFinishedPicking = nullptr;
        onKeyboardHeightChanged = nullptr;

        onPhotosAccessGranted = nullptr;
        onPhotosAccessDenied = nullptr;
        onCameraCancelled = nullptr;
    }

    void iOSNativeUtils::showAlertView(const QString &title, const QString &message, const QStringList &buttons)
    {
        if (m_IsAlertDialogVisible) {
            LOG("Only one alert dialog can be shown at a time.");
            return;
        }

        m_IsAlertDialogVisible = true;
        UIAlertController *alert = [UIAlertController
                                    alertControllerWithTitle: [NSString stringWithUTF8String: title.toStdString().c_str()]
                                    message: [NSString stringWithUTF8String: message.toStdString().c_str()]
                                    preferredStyle: UIAlertControllerStyleAlert];

        for (auto it = buttons.constBegin(); it != buttons.constEnd(); it++) {
            const QString buttonText = (*it);
            UIAlertAction *button = [UIAlertAction
                                     actionWithTitle: [NSString stringWithUTF8String: buttonText.toStdString().c_str()]
                                     style: UIAlertActionStyleDefault
                                     handler: ^ (UIAlertAction * action) {
                                         NSUInteger index = [[alert actions] indexOfObject: action];
                                         iOSNativeUtils::emitAlertDialogClickedSignal(static_cast<unsigned int>(index));
                                     }
                                     ];

            [alert addAction: button];
        }

        UIApplication *app = [UIApplication sharedApplication];
        [[[app keyWindow] rootViewController] presentViewController: alert animated: YES completion: nil];
    }

    void iOSNativeUtils::shareText(const QString &text)
    {
        NSMutableArray *activityItems = [NSMutableArray new];
        [activityItems addObject: [NSString stringWithUTF8String: text.toStdString().c_str()]];
        UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems: activityItems applicationActivities: nil];

        UIApplication *app = [UIApplication sharedApplication];
        [[[app keyWindow] rootViewController] presentViewController: activityVC animated: YES completion: nil];
    }

    void iOSNativeUtils::showActionSheet(const QString &title, const QString &message, const QVariantList &buttons)
    {
        if (m_IsActionSheetDialogVisible) {
            LOG("Only one action sheet can be called.");
            return;
        }

        m_IsActionSheetDialogVisible = true;
        UIAlertController *alert = [UIAlertController
                                    alertControllerWithTitle: [NSString stringWithUTF8String: title.toStdString().c_str()]
                                    message: [NSString stringWithUTF8String: message.toStdString().c_str()]
                                    preferredStyle: UIAlertControllerStyleActionSheet];

        for (const QVariant &button : buttons) {
            UIAlertActionStyle alertActionStyle = UIAlertActionStyleDefault;
            const QVariantMap buttonData = button.toMap();
            const QString buttonStyle = buttonData.value("type", "").toString();
            const QString buttonTitle = buttonData.value("title", "").toString();

            if (buttonStyle == "destructive") {
                alertActionStyle = UIAlertActionStyleDestructive;
            }
            else if (buttonStyle == "cancel") {
                alertActionStyle = UIAlertActionStyleCancel;
            }

            UIAlertAction *actionButton = [UIAlertAction
                                           actionWithTitle: [NSString stringWithUTF8String: buttonTitle.toStdString().c_str()]
                                           style: alertActionStyle
                                           handler: ^ (UIAlertAction * action) {
                                               NSUInteger index = [[alert actions] indexOfObject: action];
                                               iOSNativeUtils::emitActionSheetDialogClickedSignal(static_cast<unsigned int>(index));
                                           }
                                           ];

            [alert addAction: actionButton];
        }

        UIApplication *app = [UIApplication sharedApplication];
        [[[app keyWindow] rootViewController] presentViewController: alert animated: YES completion: nil];
    }

    void iOSNativeUtils::schedulePushNotification(const QString &title, const QString &body, const int &delayInSeconds)
    {
        UILocalNotification *localNotification = [[UILocalNotification alloc] init];
        localNotification.fireDate = [NSDate dateWithTimeIntervalSinceNow: delayInSeconds];
        localNotification.alertTitle = title.toNSString();
        localNotification.alertBody = body.toNSString();
        localNotification.timeZone = [NSTimeZone defaultTimeZone];
        UIApplication *app = [UIApplication sharedApplication];
        [app scheduleLocalNotification: localNotification];
    }

    void iOSNativeUtils::dismissKeyboard()
    {
        [[UIApplication sharedApplication] sendAction: @selector(resignFirstResponder) to: nil from: nil forEvent: nil];
    }

    QString iOSNativeUtils::getFCMToken() const
    {
        QString token = "";
#if FCM_ENABLED == 1
        NSString *fcmToken = [FIRMessaging messaging].FCMToken;
        if (fcmToken) {
            token = QString::fromNSString(fcmToken);
        }
#endif // FCM_ENABLED

        return token;
    }

    void iOSNativeUtils::setApplicationIconBadgeNumber(const int &number)
    {
        [UIApplication sharedApplication].applicationIconBadgeNumber = number;
    }

    bool iOSNativeUtils::isiPad() const
    {
        return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
    }

    void iOSNativeUtils::openSafari(const QString &url)
    {
#ifdef SAFARI_SERVICES_ENABLED
        SFSafariViewController *safariVC = [[SFSafariViewController alloc]initWithURL: [NSURL URLWithString: url.toNSString()] entersReaderIfAvailable: NO];

        UIApplication *app = [UIApplication sharedApplication];
        [[[app keyWindow] rootViewController] presentViewController: safariVC animated: YES completion: nil];
#else
        Q_UNUSED(url)
#endif // SAFARI_SERVICES_ENABLED
    }

    void iOSNativeUtils::requestLocationPermission()
    {
#if QUTILS_LOCATION_ENABLED == 1
        CLLocationManager *locationManager = [[CLLocationManager alloc] init];
        [locationManager requestWhenInUseAuthorization];
#endif // QUTILS_LOCATION_ENABLED
    }

    iOSNativeUtils::LocationAuthorizationStatus iOSNativeUtils::getLocationAuthorizationStatus()
    {
        LocationAuthorizationStatus authStatus = LocationAuthorizationStatus::LANone;
#if QUTILS_LOCATION_ENABLED == 1
        CLAuthorizationStatus status = [CLLocationManager authorizationStatus];
        if (status == kCLAuthorizationStatusDenied) {
            authStatus = LocationAuthorizationStatus::LADenied;
        }
        else if (status == kCLAuthorizationStatusRestricted) {
            authStatus = LocationAuthorizationStatus::LARestricted;
        }
        else if (status == kCLAuthorizationStatusNotDetermined) {
            authStatus = LocationAuthorizationStatus::LANotDetermined;
        }
        else if (status == kCLAuthorizationStatusAuthorizedAlways) {
            authStatus = LocationAuthorizationStatus::LAAuthorizedAlways;
        }
        else if (status == kCLAuthorizationStatusAuthorizedWhenInUse) {
            authStatus = LocationAuthorizationStatus::LAAuthorizedWhenInUse;
        }
#endif // QUTILS_LOCATION_ENABLED

        return authStatus;
    }

    void iOSNativeUtils::requestPhotosPermisson()
    {
#if QUTILS_PHOTOS_ENABLED == 1
        if (m_IsPhotoAccessPermissionRequested) {
            LOG("Permission is already requested.");
            return;
        }

        m_IsPhotoAccessPermissionRequested = true;
        if ([PHPhotoLibrary authorizationStatus] == PHAuthorizationStatusNotDetermined) {
            [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
                iOSNativeUtils::emitPhotoAccessPermissionSignals(status == PHAuthorizationStatusAuthorized);
            }];
        }
#endif // QUTILS_PHOTOS_ENABLED
    }

    iOSNativeUtils::PhotosAuthorizationStatus iOSNativeUtils::getPhotosAuthorizationStatus()
    {
        PhotosAuthorizationStatus authStatus = PhotosAuthorizationStatus::PANone;
#if QUTILS_PHOTOS_ENABLED == 1
        PHAuthorizationStatus status = [PHPhotoLibrary authorizationStatus];
        if (status == PHAuthorizationStatusDenied) {
            authStatus = PhotosAuthorizationStatus::PADenied;
        }
        else if (status == PHAuthorizationStatusRestricted) {
            authStatus = PhotosAuthorizationStatus::PARestricted;
        }
        else if (status == PHAuthorizationStatusNotDetermined) {
            authStatus = PhotosAuthorizationStatus::PANotDetermined;
        }
        else if (status == PHAuthorizationStatusAuthorized) {
            authStatus = PhotosAuthorizationStatus::PAAuthorized;
        }
#endif // QUTILS_PHOTOS_ENABLED

        return authStatus;
    }

    void iOSNativeUtils::openGallery()
    {
        m_IsImagePickerOpen = true;

        UIImagePickerController *picker = [[UIImagePickerController alloc] init];
        picker.allowsEditing = NO;
        picker.delegate = (id<UINavigationControllerDelegate, UIImagePickerControllerDelegate>)m_QutilsDelegate;
        picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;

        UIApplication *app = [UIApplication sharedApplication];
        [[[app keyWindow] rootViewController] presentViewController: picker animated: YES completion: nil];
    }

    void iOSNativeUtils::showCamera()
    {
        m_IsCameraOpen = true;

        UIImagePickerController *picker = [[UIImagePickerController alloc] init];
        picker.allowsEditing = NO;
        picker.delegate = (id<UINavigationControllerDelegate, UIImagePickerControllerDelegate>)m_QutilsDelegate;
        picker.sourceType = UIImagePickerControllerSourceTypeCamera;

        UIApplication *app = [UIApplication sharedApplication];
        [[[app keyWindow] rootViewController] presentViewController: picker animated: YES completion: nil];
    }

    void iOSNativeUtils::setStatusBarVisible(bool visible)
    {
        UIApplication *app = [UIApplication sharedApplication];
        [app setStatusBarHidden:!visible];
    }

    void iOSNativeUtils::emitImagePickerFinished(QVariantMap data)
    {
        for (iOSNativeUtils *instance : m_Instances) {
            if (instance != nullptr && (instance->isImagePickerOpen() || instance->isCameraOpen())) {
                instance->callImagePickerFinishedCallback(data);
                break;
            }
        }
    }

    void iOSNativeUtils::emitImagePickerCancelled()
    {
        for (iOSNativeUtils *instance : m_Instances) {
            if (instance != nullptr && (instance->isImagePickerOpen() || instance->isCameraOpen())) {
                instance->callImagePickerCancelledCallback();
                break;
            }
        }
    }

    bool iOSNativeUtils::isImagePickerOpen() const
    {
        return m_IsImagePickerOpen;
    }

    bool iOSNativeUtils::isCameraOpen() const
    {
        return m_IsCameraOpen;
    }

    bool iOSNativeUtils::isStatusBarVisible() const
    {
        UIApplication *app = [UIApplication sharedApplication];
        return [app isStatusBarHidden] == YES;
    }

    void iOSNativeUtils::emitKeyboardHeightChangedSignals(int height)
    {
        for (iOSNativeUtils *instance : m_Instances) {
            if (instance) {
                instance->emitKeyboardHeightChanged(height);
            }
        }
    }

    void iOSNativeUtils::callImagePickerFinishedCallback(QVariantMap &data)
    {
        if (onImagePickerControllerFinishedPicking) {
            onImagePickerControllerFinishedPicking(data);
        }

        m_IsImagePickerOpen = false;
        m_IsCameraOpen = false;
    }

    void iOSNativeUtils::callImagePickerCancelledCallback()
    {
        if (m_IsImagePickerOpen) {
            if (onImagePickerControllerCancelled) {
                onImagePickerControllerCancelled();
            }

            m_IsImagePickerOpen = false;
        }
        else if (m_IsCameraOpen) {
            if (onCameraCancelled) {
                onCameraCancelled();
            }

            m_IsCameraOpen = false;
        }
    }

    void iOSNativeUtils::emitKeyboardHeightChanged(int height)
    {
        if (onKeyboardHeightChanged) {
            onKeyboardHeightChanged(height);
        }
    }

    void iOSNativeUtils::emitAlertDialogClickedSignal(unsigned int index)
    {
        for (iOSNativeUtils *instance : m_Instances) {
            if (instance) {
                instance->callAlertDialogClickedCallback(index);
            }
        }
    }

    void iOSNativeUtils::emitActionSheetDialogClickedSignal(unsigned int index)
    {
        for (iOSNativeUtils *instance : m_Instances) {
            if (instance) {
                instance->callActionSheetDialogClickedCallback(index);
            }
        }
    }

    void iOSNativeUtils::emitPhotoAccessPermissionSignals(bool isAccessGranted)
    {
        for (iOSNativeUtils *instance : m_Instances) {
            if (instance) {
                instance->callPhotoAccessResultCallback(isAccessGranted);
            }
        }
    }

    void iOSNativeUtils::callAlertDialogClickedCallback(unsigned int index)
    {
        if (m_IsAlertDialogVisible) {
            if (onAlertDialogClicked) {
                onAlertDialogClicked(index);
            }

            m_IsAlertDialogVisible = false;
        }
    }

    void iOSNativeUtils::callActionSheetDialogClickedCallback(unsigned int index)
    {
        if (m_IsActionSheetDialogVisible) {
            if (onActionSheetClicked) {
                onActionSheetClicked(index);
            }

            m_IsActionSheetDialogVisible = false;
        }
    }

    void iOSNativeUtils::callPhotoAccessResultCallback(bool isAccessGranted)
    {
        if (isAccessGranted) {
            if (onPhotosAccessGranted) {
                onPhotosAccessGranted();
            }
        }
        else if (onPhotosAccessDenied) {
            onPhotosAccessDenied();
        }

        m_IsPhotoAccessPermissionRequested = false;
    }
}
