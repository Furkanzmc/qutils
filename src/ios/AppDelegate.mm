#import "qutils/ios/iOSNativeUtils.h"
// UIKit
#import <UIKit/UIKit.h>
#import <UserNotifications/UserNotifications.h>
#import <UserNotifications/UNUserNotificationCenter.h>
#if FCM_ENABLED == 1
// Firebase
#import <FirebaseCore/FirebaseCore.h>
#import <FirebaseMessaging/FirebaseMessaging.h>
#import <FirebaseMessaging/FIRMessaging.h>
#import <Firebase.h>
// qutils
#import "NotificationClient.h"
#endif // FCM_ENABLED

#if FCM_ENABLED == 1
@interface QIOSApplicationDelegate : UIResponder <UIApplicationDelegate, UNUserNotificationCenterDelegate, FIRMessagingDelegate>
#else
@interface QIOSApplicationDelegate : UIResponder <UIApplicationDelegate, UNUserNotificationCenterDelegate>
#endif // FCM_ENABLED
@end
//add a category to QIOSApplicationDelegate
@interface QIOSApplicationDelegate (QAppDelegate)
@end

@implementation QIOSApplicationDelegate (QPushNotificationDelegate)
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    (void)application;
    (void)launchOptions;

#if FCM_ENABLED == 1
    [FIRApp configure];
    [FIRMessaging messaging].delegate = self;

    if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_9_x_Max) {
        UIUserNotificationType allNotificationTypes =
        (UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge);
        UIUserNotificationSettings *settings =
        [UIUserNotificationSettings settingsForTypes:allNotificationTypes categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    }
    else {
        // iOS 10 or later
#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
        // For iOS 10 display notification (sent via APNS)
        [UNUserNotificationCenter currentNotificationCenter].delegate = self;
        UNAuthorizationOptions authOptions = UNAuthorizationOptionAlert | UNAuthorizationOptionSound | UNAuthorizationOptionBadge;
        [[UNUserNotificationCenter currentNotificationCenter] requestAuthorizationWithOptions:authOptions completionHandler:^(BOOL granted, NSError * _Nullable error) {
            (void)granted;
            (void)error;
        }];
#endif // defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
    }

    UIApplication *app = [UIApplication sharedApplication];
    [app registerForRemoteNotifications];

    // Remote Notification Info
    NSDictionary *remoteNotifiInfo = [launchOptions objectForKey: UIApplicationLaunchOptionsRemoteNotificationKey];

    // Accept push notification when app is not open
    if (remoteNotifiInfo) {
        NSMutableDictionary *mutDict = [remoteNotifiInfo mutableCopy];
        [mutDict setValue:@"true" forKey:@"is_tapped"];
        NSDictionary *dict = [mutDict copy];
        [self application:application didReceiveRemoteNotification: dict];
    }
#endif // FCM_ENABLED

    return YES;
}

#if FCM_ENABLED == 1

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    (void)application;
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:userInfo options:NSJSONWritingPrettyPrinted error:&error];
    NSString *jsonString;
    if (jsonData) {
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    else {
        jsonString = @"";
    }

    const QString payload = QString::fromNSString(jsonString);
    zmc::NotificationClient *client = zmc::NotificationClient::getInstance(QString(""), -1);
    if (client) {
        client->emitNotificationReceivedSignal(payload);
    }
    else {
        bool isTapped = [userInfo objectForKey:@"is_tapped"] != nil;
        zmc::NotificationClient::addNotifiationQueue(std::make_tuple("", -1, "", payload, isTapped));
    }
}

- (void)messaging:(nonnull FIRMessaging *)messaging didRefreshRegistrationToken:(nonnull NSString *)fcmToken {
    (void)messaging;
    zmc::NotificationClient::emitFCMTokenReceivedSignal(QString::fromNSString(fcmToken));
}

#endif // FCM_ENABLED
@end
