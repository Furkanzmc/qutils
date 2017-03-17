# qutils

qutils is a set of tools I'm developing to help me with Qt mobile development. Right now, it is only for Android notifications.

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
