// UIKit
#import <UserNotifications/UNUserNotificationCenter.h>
#import <UserNotifications/UserNotifications.h>
#import <UIKit/UIKit.h>
// qutils
#import "iOSUtils.h"

@interface QIOSApplicationDelegate : UIResponder <UIApplicationDelegate>
@end
//add a category to QIOSApplicationDelegate
@interface QIOSApplicationDelegate (QAppDelegate)
@end

@implementation QIOSApplicationDelegate (QAppDelegate)
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    Q_UNUSED(application);
    Q_UNUSED(launchOptions);

    return YES;
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey, id> *)options {
    Q_UNUSED(options);
    Q_UNUSED(app);
    zmc::iOSUtils::emitOpenedWithURLSignal(QString::fromNSString(url.absoluteString));
    return YES;
}

@end
