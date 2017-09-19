# qutils

qutils is a set of tools I'm developing to help me with Qt mobile development. Right now, it is only for Android notifications.

# Configuration Definitions

- `QUTILS_APP_NAME`: This is internally used to name the settings and the cache files. If it's not provided, a default one is used. This is used on both mobile and desktop platforms.
- `QUTILS_APP_PACKAGE_NAME`: This is used only on mobile platforms. This is useful when you are using the `LOG_JNI` macro to log. When logging with JNI, this is added as a tag so you can see the output with `adb logcat | grep <QUTILS_APP_PACKAGE_NAME>`
- `QUTILS_FOR_MOBILE`: This signfies that you are using the library for mobile development.
- `ENABLE_FCM`: Add this If you want to use Firebase Cloud Messaging.

## Example Values

```
DEFINES += QUTILS_APP_NAME="MyApp"
DEFINES += QUTILS_APP_PACKAGE_NAME="org.myorg.myapp"
DEFINES += QUTILS_FOR_MOBILE
CONFIG += ENABLE_FCM
```


# Android Features

## How to Use in Your Project

The best way I know of doing this is to add the following line to your `gradle.properties` file:

```
qutilsDir=ABSOLUTE_PATH_TO_QUTILS/android/
```

And then in your `build.gradle` file add this `qutilsDir + '/src' to `java.srcDirs`:

```
java.srcDirs = [qt5AndroidDir + '/src', 'src', 'java', qutilsDir + '/src']
```

I'm not really experienced in Android development, so anyone out there with experience please tell me how to better package the library. :D

## Notifications

qutils have support for Android local push notifications. You can send an instant notification or a scheduled one.
Available notification properties are:

- priority
- category
- ledColor
- ledOnMS
- ledOffMS
- title
- text
- delay
- date
- notificationTag
- notificationID
- sound
- defaults
- flags
- visibility

You can send notifications either by creating a `Notification` object, or just by using Javascript dictionary.
`NotificationManager` handles the notification sending and receiving.

You can check out the `qutils_demo` app source code or the app on [Play Store](https://play.google.com/store/apps/details?id=org.zmc.qutils.demo&rdid=org.zmc.qutils.demo).

### Notification Behavior

When you send an instant notification, only the `NonitificationManager` that sent the notification will receive `notificationReceived` signal. But this behavior changes when you send a scheduled notification. Scheduled notifications emit the signals according to the `objectName` of the `NotificationManager` that sent it. If a `NotificationManager` has an empty `objectName`, then when the app is opened by tapping on the scheduled notification all of the `NotificationManager` instances are notified about the `Notification`.
But if `NotificationManager` has an `objectName` only that `NotificationManager` will be signaled.

### Note

You need to set the notification icon by using `m_NotificationClient.setSmallIcon(R.drawable.icon);`. Otherwise you'll get a `android.app.RemoteServiceException: Bad notification posted from package` error when the notification arrives.


# Common

## QMLRefresh

The QMLRefresh is a helper class to reload QML on runtime. To use it, pass the QQmlApplicationEngine pointer and call the reload function when you need.
For more information about runtime reloading go here -> http://www.slideshare.net/ICSinc/how-best-to-realize-a-runtime-reload-of-qml

 ```
 // in main.cpp
 QMLRefresh manager(&engine);
 manager.setMainQMLFile("file:///E:/Development/Playground/Live-QML/qml/main.qml");
 engine.rootContext()->setContextProperty("QM", &manager);
 ```

Pass the `QQuickWindow` to `QMLRefresh`, then use QM.relad() to reload at runtime!

```
Window {
    id: mainWindow
    Component.onCompleted: {
        QM.setWindow(mainWindow);
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            QM.reload();
        }
    }
}
```

## ScreenHelper

ScreenHelper is a utility class to help create resolution indepentant UI in QML.

```
// in main.cpp
ScreenHelper manager;
engine.rootContext()->setContextProperty("SH", &manager);
```

Then use like so:

```
Window {
    id: mainWindow
     Rectangle {
        width: SH.dp(50)
    }
}
```

# Disable Multimedia

If you want to disable multimedia functions, just put the following in your project's `pro` file.

```
CONFIG += QUTILS_NO_MULTIMEDIA
```

# Enable Deep Linking for Android

To get an in-depth look into app linkink, I recommend you read the [official documentation](https://developer.android.com/training/app-links/index.html). All you need to do to enable deep linking with qutils is to add the following intent filter to you `AndroidManifest.xml` file.

```
<intent-filter>
    <action android:name="android.intent.action.VIEW"/>
    <category android:name="android.intent.category.DEFAULT"/>
    <category android:name="android.intent.category.BROWSABLE"/>
    <data android:scheme="http"/>
    <data android:scheme="https"/>
    <data android:host="www.arifname-2000.appspot.com"/>
</intent-filter>
```

And then you can respond to app link with the `AndroidUtils::void openedWithURL(const QString &url)` or `NativeUtils::void openedWithURL(const QString &url)` signals.

```
NativeUtils {
    id: nativeUtils
    buttonEventsEnabled: false
    onOpenedWithURL: {
        // Do something with the url.
    }
}
```

# Enable Firebase Cloud Messaging for Android

You just need to add the Firebase SDK dependancy to your `build.gradle` file.

You can follow the official setup [here](https://firebase.google.com/docs/android/setup).

Add the following to your `dependencies`.

```
dependencies {
    ...
    classpath 'com.google.gms:google-services:3.1.0'
}
```

Just make sure that these lines are at the bottom of your `build.gradle` file.

```
dependencies {
    compile 'com.google.firebase:firebase-core:11.0.2'
    compile 'com.google.firebase:firebase-messaging:11.0.2'
    compile 'com.google.android.gms:play-services-base:11.0.2'
}

apply plugin: 'com.google.gms.google-services'
```

If you don't want to use Firebase in your project, you can use the following code in your `build.gradle` file to exclude the Firebase related files.

```
java {
    exclude "**/notification/QutilsFirebase**"
}
```

And add these to your `AndroidManifest.xml` file after the `activity` tag.

```
<receiver android:name="org.zmc.qutils.notification.NotificationReceiver"/>
<service android:name="org.zmc.qutils.notification.QutilsFirebaseMessagingService">
    <intent-filter>
      <action android:name="com.google.firebase.MESSAGING_EVENT"/>
    </intent-filter>
</service>
<service android:name="org.zmc.qutils.notification.QutilsFirebaseInstanceIDService">
    <intent-filter>
        <action android:name="com.google.firebase.INSTANCE_ID_EVENT"/>
    </intent-filter>
</service>
```
