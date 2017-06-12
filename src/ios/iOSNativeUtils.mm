#import "qutils/ios/iOSNativeUtils.h"
// UIKit
#import <UIKit/UIKit.h>

iOSNativeUtils::iOSNativeUtils()
{

}

void iOSNativeUtils::showAlertView(const QString &title, const QString &message, const QStringList &buttons)
{
    UIAlertController *alert = [UIAlertController
                                alertControllerWithTitle:[NSString stringWithUTF8String:title.toStdString().c_str()]
                                message:[NSString stringWithUTF8String:message.toStdString().c_str()]
                                preferredStyle:UIAlertControllerStyleAlert];

    for (auto it = buttons.constBegin(); it != buttons.constEnd(); it++) {
        const QString buttonText = (*it);
        UIAlertAction *button = [UIAlertAction actionWithTitle:[
                NSString stringWithUTF8String:buttonText.toStdString().c_str()]
                style:UIAlertActionStyleDefault
                handler:^(UIAlertAction * action)
                {
                    if (onAlertDialogClicked) {
                        NSUInteger index = [[alert actions] indexOfObject:action];
                        onAlertDialogClicked((unsigned int)index);
                     }
                }
        ];

        [alert addAction:button];
    }

    UIApplication *app = [UIApplication sharedApplication];
    [[[app keyWindow] rootViewController] presentViewController:alert animated:YES completion:nil];
}
