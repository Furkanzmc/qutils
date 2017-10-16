#import "qutils/ios/iOSNativeUtils.h"
// UIKit
#import <UIKit/UIKit.h>
// Firebase
#if FCM_ENABLED == 1
#import <FirebaseMessaging/FirebaseMessaging.h>
#endif // FCM_ENABLED

iOSNativeUtils::iOSNativeUtils()
{
    [[NSNotificationCenter defaultCenter] addObserverForName:UIKeyboardWillHideNotification object:nil queue:nil usingBlock:^(NSNotification * _Nonnull note) {
        Q_UNUSED(note);
        if (onKeyboardHeightChanged) {
            onKeyboardHeightChanged(0);
        }
    }];

    [[NSNotificationCenter defaultCenter] addObserverForName:UIKeyboardWillShowNotification object:nil queue:nil usingBlock:^(NSNotification * _Nonnull note) {
        if (onKeyboardHeightChanged) {
            const float height = [note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue].size.height;
            onKeyboardHeightChanged(static_cast<int>(height));
        }
    }];
}

void iOSNativeUtils::showAlertView(const QString &title, const QString &message, const QStringList &buttons)
{
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
                                    if (onAlertDialogClicked) {
                                        NSUInteger index = [[alert actions] indexOfObject: action];
                                        onAlertDialogClicked((unsigned int)index);
                                    }
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
    UIAlertController* alert = [UIAlertController
                                alertControllerWithTitle:[NSString stringWithUTF8String: title.toStdString().c_str()]
                                message:[NSString stringWithUTF8String: message.toStdString().c_str()]
                                preferredStyle:UIAlertControllerStyleActionSheet];

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
                                 actionWithTitle:[NSString stringWithUTF8String: buttonTitle.toStdString().c_str()]
                                 style:alertActionStyle
                                 handler:^(UIAlertAction * action) {
                                     if (onActionSheetClicked) {
                                         NSUInteger index = [[alert actions] indexOfObject: action];
                                         onActionSheetClicked((unsigned int)index);
                                     }
                                 }
        ];

        [alert addAction:actionButton];
    }

    UIApplication *app = [UIApplication sharedApplication];
    [[[app keyWindow] rootViewController] presentViewController: alert animated: YES completion: nil];
}

void iOSNativeUtils::schedulePushNotification(const QString &title, const QString &body, const int &delayInSeconds)
{
    UILocalNotification* localNotification = [[UILocalNotification alloc] init];
    localNotification.fireDate = [NSDate dateWithTimeIntervalSinceNow:delayInSeconds];
    localNotification.alertTitle = title.toNSString();
    localNotification.alertBody = body.toNSString();
    localNotification.timeZone = [NSTimeZone defaultTimeZone];
    UIApplication *app = [UIApplication sharedApplication];
    [app scheduleLocalNotification:localNotification];
}

void iOSNativeUtils::dismissKeyboard()
{
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
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
