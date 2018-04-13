# qutils

qutils is a set of tools I'm developing to help me with Qt mobile development. Right now, it is only for Android notifications.

**Note:** The library is in usable form and I've been using it in production with various apps. But it's not as clean as I would like it to be. I started refactoring and once I'm done it will be more expressive, usable and easy to read.

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

# Notification Behavior

When you send an instant notification, only the `NonitificationManager` that sent the notification will receive `notificationReceived` signal. But this behavior changes when you send a scheduled notification. Scheduled notifications emit the signals according to the `objectName` of the `NotificationManager` that sent it. If a `NotificationManager` has an empty `objectName`, then when the app is opened by tapping on the scheduled notification all of the `NotificationManager` instances are notified about the `Notification`.
But if `NotificationManager` has an `objectName` only that `NotificationManager` will be signaled.


# FileUtils

`FileUtils` is the main class for file manipulation and file/photo selection. File and photo selection only works on mobile devices.

On desktop platforms, you can use the Qt's default file dialog. You can checkout the code documentation to learn which signals are called when.

## Functions

- `FileUtils::changeImageQuality`: You can use this function to reduce the quality of an image to the specified quality. This can be useful when you are uploading a profile photo and you don't want the file to be 5 MB.
- `FileUtils::getFileInfo`: This will return a `QVariantMap` that contains various information about the file. The available keys are:
    + `absoluteFilePath`
    + `baseName`
    + `completeBaseName`
    + `completeSuffix`
    + `created`: The time format is in Qt::DateFormat::ISODate
    + `fileName`
    + `size`
    + `absoluteDirPath`: The result of fileInfo.absoluteDir().absolutePath()
    + `exists`: If this is false, all the other fields will be missing.
- `FileUtils::remove`: Deletes the file at the specified path.
- `FileUtils::exists`: Returns true If the specified file exists.
- `FileUtils::copy`: Copies the file from one location to another.
- `FileUtils::getFileChecksum`: Returns the file's checksum using `QCryptographicHash::Algorithm::Md5`.
- `FileUtils::isValidURL`: Returns true if the URL is non-empty and valid; otherwise returns false. Calls the QUrl::isValid.
- `FileUtils::isLocalFile`: Returns true if this URL is pointing to a local file path. A URL is a local file path if the scheme is "file". Calls the QUrl::isLocalFile.
- `FileUtils::openDocumentPicker`: This works on iOS and Android. On iOS, it will open the `UIDocumentPickerView` and on Android it will open the default file picker. You can set different file types that can be selected using the document picker on both iOS and Android. If you leave `documentTypes` empy on iOS, it will default to `public.data`. On Android, `documentTypes` is required.
- `FileUtils::openGallery`: Opens the photo gallery on iOS and Android. This does not check for permissions on each platform, so make sure that you have acess to user's photo library before calling this.
- `FileUtils::getTemporaryFile`: Returns the file path to a temporary file that you can use. This file is created using `QTemporaryFile` and it is not automatically removed.
- `FileUtils::readFile`: Reads the content of the given file and returns it as `QString`.

## Notes

`FileUtils::openGallery` and `FileUtils::openDocumentPicker` can open files from any type of source but on Androıd only the local files are supported as of now. So, even though you can see Google Drive as the source in Android the selected file path will be empty.

## Example Usage

```qml
Window {
    id: window
    visible: true
    width: 640
    height: 480
    color: "red"
    title: qsTr("Hello World")

    Text {
        id: lb
        width: parent.width
        anchors.centerIn: parent
        font.pointSize: 20
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        color: "black"
    }

    Image {
        id: img
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            lb.text = "";
            var types = undefined;
            if (Q_OS_IOS) {
                types = ["public.data"];
            }
            else if (Q_OS_ANDROID) {
                types = ["image/*"];
            }

            fileUtils.openDocumentPicker(types, false);
        }
        onPressAndHold: {
            fileUtils.openGallery();
        }
    }

    FileUtils {
        id: fileUtils
        onDocumentPickerCanceled: {
            lb.text = "Canceled!";
        }
        onPhotoSelected: {
            img.source = photoPath;
        }
        onPhotoSelectionCanceled: {
            lb.text = "Photo Canceled!";
        }
        onDocumentPicked: {
            lb.text = JSON.stringify(paths);
            if (paths.length > 0) {
                img.source = paths[0];
            }
        }
    }
}
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

## File Provider

**res/xml/file_paths.xml**

```xml
<?xml version="1.0" encoding="utf-8"?>
<paths xmlns:android="http://schemas.android.com/apk/res/android">
    <external-path name="my_images" path="Android/data/org.example.app/cache" />
</paths>
```

**AndroidManifest.xml**

```
<provider android:name="android.support.v4.content.FileProvider" android:authorities="org.example.app.provider" android:exported="false" android:grantUriPermissions="true">
    <meta-data android:name="android.support.FILE_PROVIDER_PATHS" android:resource="@xml/file_paths"/>
</provider>
```

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

### Note

You need to set the notification icon by using `m_NotificationClient.setSmallIcon(R.drawable.icon);`. Otherwise you'll get a `android.app.RemoteServiceException: Bad notification posted from package` error when the notification arrives.

Also, when tapping on the notification Android's navigation system messes with Qt window so you should change the `android:launchMode="singleTop"` to `android:launchMode="singleInstance"`

# iOS Features

## Deep Linking

You just need to add `CFBundleURLTypes` key to the `*.plist` file. Everything else is taken care of by qutils.

## Location

You can use `iOSNativeUtils::requestLocationPermission` and `iOSNativeUtils::getLocationAuthorizationStatus` If you added `positioning` to Qt modules.
If `positioning` is not present in the enabled modules, those methods will not function.

## Push Notifications

You can follow the official instructions [here](https://firebase.google.com/docs/cloud-messaging/ios/client).
To use CocoaPods with Qt create a `ios/CocoaPods` directory in your project's root. And then create an empty XCode project as a dummy project for CocoaPods.
Initialize the pod and add the dependencies.

Example Pod file:

```
target 'Dummy' do
pod 'Firebase/Core'
pod 'Firebase/Messaging'
pod 'Firebase/Crash'
end

```

Adding the libraries in your main `pro` file.

```
ios {
    LIBS += -F$$PWD/ios/CocoaPods/Pods/FirebaseCore/Frameworks -framework FirebaseCore \
        -F$$PWD/ios/CocoaPods/Pods/FirebaseCrash/Frameworks -framework FirebaseCrash \
        -F$$PWD/ios/CocoaPods/Pods/FirebaseInstanceID/Frameworks -framework FirebaseInstanceID \
        -F$$PWD/ios/CocoaPods/Pods/FirebaseMessaging/Frameworks -framework FirebaseMessaging

    INCLUDEPATH += $$PWD/ios/CocoaPods/Pods/FirebaseCore/Frameworks/FirebaseCore.framework/Headers \
        $$PWD/ios/CocoaPods/Pods/FirebaseAnalytics/Frameworks/FirebaseAnalytics.framework/Headers \
        $$PWD/ios/CocoaPods/Pods/FirebaseCrash/Frameworks/FirebaseCrash.framework/Headers \
        $$PWD/ios/CocoaPods/Pods/FirebaseInstanceID/Frameworks/FirebaseInstanceID.framework/Headers \
        $$PWD/ios/CocoaPods/Pods/FirebaseMessaging/Frameworks/FirebaseMessaging.framework/Headers

    XCODE_EXTRAS.files = ios/GoogleService-Info.plist
    QMAKE_BUNDLE_DATA += XCODE_EXTRAS
}
```


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

Also, add the following to the `build.gradle` file as well.

```
defaultConfig {
    applicationId "org.my.appid"
}
```
