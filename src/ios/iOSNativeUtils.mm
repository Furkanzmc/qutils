#import "qutils/ios/iOSNativeUtils.h"
// iOS
#ifdef SAFARI_SERVICES_ENABLED
    #import <SafariServices/SafariServices.h>
#endif // SAFARI_SERVICES_ENABLED
#import <sys/utsname.h>
#import <UIKit/UIKit.h>
// Firebase
#if FCM_ENABLED == 1
    #import <FirebaseMessaging/FirebaseMessaging.h>
#endif // FCM_ENABLED
// Local
#include "qutils/Macros.h"
#import "qutils/ios/QutilsViewDelegate.h"

namespace zmc
{

static QutilsViewDelegate *m_QutilsDelegate = [[QutilsViewDelegate alloc] init];

QMap<int, iOSNativeUtils *> iOSNativeUtils::m_Instances = QMap<int, iOSNativeUtils *>();

iOSNativeUtils::iOSNativeUtils()
    : m_InstanceIndex(m_Instances.size())
    , m_IsImagePickerOpen(false)
    , m_IsAlertDialogVisible(false)
    , m_IsActionSheetDialogVisible(false)
    , m_IsCameraOpen(false)
{
    if (m_Instances.size() == 0) {
        [[NSNotificationCenter defaultCenter] addObserverForName: UIKeyboardWillHideNotification object: nil queue: nil usingBlock: ^(NSNotification * _Nonnull note) {
            Q_UNUSED(note);
            iOSNativeUtils::emitKeyboardHeightChangedSignals(0);
        }];

        [[NSNotificationCenter defaultCenter] addObserverForName: UIKeyboardWillShowNotification object: nil queue: nil usingBlock: ^(NSNotification * _Nonnull note) {
            const float height = [note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue].size.height;
            iOSNativeUtils::emitKeyboardHeightChangedSignals(static_cast<int>(height));
        }];
    }

    m_Instances.insert(m_InstanceIndex, this);
}

iOSNativeUtils::~iOSNativeUtils()
{
    m_Instances.remove(m_InstanceIndex);
    onAlertDialogClicked = nullptr;
    onActionSheetClicked = nullptr;

    onImagePickerControllerCancelled = nullptr;
    onImagePickerControllerFinishedPicking = nullptr;
    onKeyboardHeightChanged = nullptr;

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
                handler: ^(UIAlertAction * action) {
                    NSUInteger index = [[alert actions] indexOfObject: action];
                    iOSNativeUtils::emitAlertDialogClickedSignal(static_cast<unsigned int>(index));
                }];

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

void iOSNativeUtils::showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect)
{
    UIApplication *app = [UIApplication sharedApplication];
    UIViewController *viewController = [[app keyWindow] rootViewController];
    UIViewController *presentedView = [viewController presentedViewController];

    /*
     * Alert sheet can be dismissed by clicking outside of the view on iPads. We are checking to see If there's a
     * presented view because If there isn't and m_IsActionSheetDialogVisible is set to true, we can omit that check.
     */
    if (m_IsActionSheetDialogVisible && presentedView != nil) {
        LOG_ERROR("Only one action sheet can be called.");
        return;
    }

    if (isiPad() && rect.isEmpty()) {
        LOG_ERROR("rect parameter is required on iOS devices.");
        return;
    }

    m_IsActionSheetDialogVisible = true;
    UIAlertController *alert = [UIAlertController
        alertControllerWithTitle: [NSString stringWithUTF8String: title.toStdString().c_str()]
        message: [NSString stringWithUTF8String: message.toStdString().c_str()]
        preferredStyle: UIAlertControllerStyleActionSheet
    ];

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

        UIAlertAction *actionButton = [UIAlertAction actionWithTitle: [NSString stringWithUTF8String: buttonTitle.toStdString().c_str()] style: alertActionStyle
            handler:^(UIAlertAction * action) {
                NSUInteger index = [[alert actions] indexOfObject: action];
                iOSNativeUtils::emitActionSheetDialogClickedSignal(static_cast<unsigned int>(index));
            }
        ];

        [alert addAction: actionButton];
    }

    if (isiPad()) {
        alert.popoverPresentationController.sourceView = viewController.view;
        alert.popoverPresentationController.sourceRect = CGRectMake(rect.x(), rect.y(), rect.width(), rect.height());
    }

    [viewController presentViewController: alert animated: YES completion: nil];
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
    UIView *statusBar = (UIView *)[app valueForKey:@"statusBar"];
    [statusBar setHidden:!visible];
}

void iOSNativeUtils::emitImagePickerFinished(QVariantMap data)
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        iOSNativeUtils *instance = it.value();
        if (instance != nullptr && (instance->isImagePickerOpen() || instance->isCameraOpen())) {
            instance->callImagePickerFinishedCallback(data);
            break;
        }
    }
}

void iOSNativeUtils::emitImagePickerCancelled()
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        iOSNativeUtils *instance = it.value();
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

QString iOSNativeUtils::getDeviceName() const
{
    struct utsname systemInfo;
    uname(&systemInfo);
    return QString::fromNSString([NSString stringWithCString: systemInfo.machine encoding: NSUTF8StringEncoding]);
}

void iOSNativeUtils::emitKeyboardHeightChangedSignals(int height)
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        iOSNativeUtils *instance = it.value();
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
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        iOSNativeUtils *instance = it.value();
        if (instance) {
            instance->callAlertDialogClickedCallback(index);
        }
    }
}

void iOSNativeUtils::emitActionSheetDialogClickedSignal(unsigned int index)
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        iOSNativeUtils *instance = it.value();
        if (instance) {
            instance->callActionSheetDialogClickedCallback(index);
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
}
