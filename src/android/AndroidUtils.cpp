#include "qutils/android/AndroidUtils.h"
// Qt
#include <QDebug>
#include <QGuiApplication>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
// qutils
#include "qutils/Macros.h"

#define ANDROID_UTILS_CLASS "org/zmc/qutils/AndroidUtils"

static const JNINativeMethod JAVA_CALLBACK_METHODS[] = {
    {
        "notificationReceived", // const char* function name;
        "(Ljava/lang/String;ILjava/lang/String;)V", // const char* function signature
        (void *)notificationReceivedCallback // function pointer
    },
    {
        "backButtonPressed", // const char* function name;
        "()V", // const char* function signature
        (void *)backButtonPressedCallback // function pointer
    },
    {
        "menuButtonPressed", // const char* function name;
        "()V", // const char* function signature
        (void *)menuButtonPressedCallback // function pointer
    },
    {
        "alertDialogClicked", // const char* function name;
        "(I)V", // const char* function signature
        (void *)alertDialogClickedCallback // function pointer
    },
    {
        "datePicked", // const char* function name;
        "(III)V", // const char* function signature
        (void *)datePickedCallback // function pointer
    },
    {
        "timePicked", // const char* function name;
        "(II)V", // const char* function signature
        (void *)timePickedCallback // function pointer
    },
    {
        "cameraCaptured", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)cameraCapturedCallback // function pointer
    },
    {
        "cameraCaptureCancelled", // const char* function name;
        "()V", // const char* function signature
        (void *)cameraCaptureCancelledCallback // function pointer
    },
    {
        "fileSelected", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)fileSelectedCallback // function pointer
    },
    {
        "fileSelectionCancelled", // const char* function name;
        "()V", // const char* function signature
        (void *)fileSelectionCancelledCallback // function pointer
    },
    {
        "keyboardHeightChanged", // const char* function name;
        "(I)V", // const char* function signature
        (void *)keyboardHeightChangedCallback // function pointer
    }
};

// This method is called automatically by Java VM after the .so file is loaded
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/)
{
    JNIEnv *env;
    // Get the JNIEnv pointer.
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Search for Java class which declares the native methods
    jclass javaClass = env->FindClass("org/zmc/qutils/CppCallbacks");
    if (!javaClass) {
        return JNI_ERR;
    }

    // Register our native methods
    if (env->RegisterNatives(javaClass, JAVA_CALLBACK_METHODS, sizeof(JAVA_CALLBACK_METHODS) / sizeof(JAVA_CALLBACK_METHODS[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

#endif // Q_OS_ANDROID

namespace zmc
{

QList<AndroidUtils *> AndroidUtils::m_Instances = QList<AndroidUtils *>();

AndroidUtils::AndroidUtils(QObject *parent)
    : QObject(parent)
    , m_InstanceID(m_Instances.size())
    , m_IsAlertShown(false)
    , m_IsDatePickerShown(false)
    , m_IsTimePickerShown(false)
    , m_IsCameraShown(false)
    , m_IsGalleryShown(false)
    , m_IsButtonEventsEnabled(true)
{
    m_Instances.append(this);
}

AndroidUtils::~AndroidUtils()
{
    m_Instances[m_InstanceID] = nullptr;
}

void AndroidUtils::setStatusBarColor(QColor color)
{
    auto runnable = [color]() {
        QString ledColor = "";
        if (color.isValid()) {
            ledColor = color.name(QColor::NameFormat::HexArgb);
        }
        QAndroidJniObject jniColor = QAndroidJniObject::fromString(ledColor);
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "setStatusBarColor",
            "(Ljava/lang/String;)V",
            jniColor.object<jstring>());
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::setStatusBarVisible(bool visible)
{
    auto runnable = [visible]() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "setStatusBarVisible",
            "(Z)V",
            visible);
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::setImmersiveMode(bool visible)
{
    auto runnable = [visible]() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "setImmersiveMode",
            "(Z)V",
            visible);
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::shareText(const QString &dialogTitle, const QString &text)
{
    auto runnable = [dialogTitle, text]() {
        QAndroidJniObject jniDialogTitle = QAndroidJniObject::fromString(dialogTitle);
        QAndroidJniObject jniText = QAndroidJniObject::fromString(text);

        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "shareText",
            "(Ljava/lang/String;Ljava/lang/String;)V",
            jniDialogTitle.object<jstring>(),
            jniText.object<jstring>());
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::showAlertDialog(const QVariantMap &dialogProperties)
{
    if (this->signalsBlocked()) {
        return;
    }

    m_IsAlertShown = true;
    const QAndroidJniObject hashMapClass = getJNIHashMap(dialogProperties);
    auto runnable = [hashMapClass]() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "showAlertDialog",
            "(Ljava/util/HashMap;)V",
            hashMapClass.object<jobject>());
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::showDatePicker()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_IsDatePickerShown = true;
    auto runnable = []() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "showDatePicker",
            "()V");
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::showTimePicker()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_IsTimePickerShown = true;
    auto runnable = []() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "showTimePicker",
            "()V");
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::showCamera(const QString &fileName)
{
    if (this->signalsBlocked()) {
        return;
    }

    m_IsCameraShown = true;
    auto runnable = [fileName]() {
        const QAndroidJniObject jniStr = QAndroidJniObject::fromString(fileName);
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "dispatchTakePictureIntent",
            "(Ljava/lang/String;)V",
            jniStr.object<jstring>());
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::showToast(const QString &text, bool isLongDuration)
{
    auto runnable = [text, isLongDuration]() {
        const QAndroidJniObject jniStr = QAndroidJniObject::fromString(text);
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "showTaost",
            "(Ljava/lang/String;Z)V",
            jniStr.object<jstring>(),
            isLongDuration);
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::openGallery()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_IsGalleryShown = true;
    auto runnable = []() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "openGallery",
            "()V");
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::emitBackButtonPressed()
{
    if (m_IsButtonEventsEnabled) {
        emit backButtonPressed();
    }
}

void AndroidUtils::emitMenuButtonPressed()
{
    if (m_IsButtonEventsEnabled) {
        emit menuButtonPressed();
    }
}

void AndroidUtils::emitAlertDialogClicked(int buttonIndex)
{
    if (m_IsAlertShown) {
        m_IsAlertShown = false;
        emit alertDialogClicked(buttonIndex);
    }
}

void AndroidUtils::emitDatePicked(int year, int month, int day)
{
    if (m_IsDatePickerShown) {
        m_IsDatePickerShown = false;
        if (year == -1 && month == -1 && day == -1) {
            emit datePickerCancelled();
        }
        else {
            emit datePicked(year, month, day);
        }
    }
}

void AndroidUtils::emitTimePicked(int hourOfDay, int minute)
{
    if (m_IsTimePickerShown) {
        m_IsTimePickerShown = false;
        if (hourOfDay == -1 && minute == -1) {
            emit timePickerCancelled();
        }
        else {
            emit timePicked(hourOfDay, minute);
        }
    }
}

void AndroidUtils::emitCameraCaptured(const QString &capturePath)
{
    if (m_IsCameraShown) {
        m_IsCameraShown = false;
        emit cameraCaptured(capturePath);
    }
}

void AndroidUtils::emitFileSelected(const QString &filePath)
{
    if (m_IsGalleryShown) {
        m_IsGalleryShown = false;
        emit fileSelected(filePath);
    }
}

void AndroidUtils::emitFileSelectionCancelled()
{
    if (m_IsGalleryShown) {
        m_IsGalleryShown = false;
        emit fileSelectionCancelled();
    }
}

void AndroidUtils::emitKeyboardHeightChanged(const int &keyboardHeight)
{
    emit keyboardHeightChanged(keyboardHeight);
}

void AndroidUtils::emitCameraCaptureCancelled()
{
    if (m_IsCameraShown) {
        m_IsCameraShown = false;
        emit cameraCaptureCancelled();
    }
}

QAndroidJniObject AndroidUtils::getJNIHashMap(const QVariantMap &map) const
{
    QAndroidJniObject hashMapClass("java/util/HashMap");
    for (auto it = map.constBegin(); it != map.constEnd(); it++) {
        QAndroidJniObject jniKey = QAndroidJniObject::fromString(it.key());
        QAndroidJniObject jniValue;
        const QVariant &value = it.value();
        if (value.type() == QVariant::String) {
            jniValue = QAndroidJniObject::fromString(value.toString());
            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                jniValue.object<jstring>());
        }
        else if (value.type() == QVariant::Int) {
            jniValue = QAndroidJniObject::callStaticObjectMethod("java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;", value.toInt());
            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                jniValue.object<jobject>());
        }
        else if (value.type() == QVariant::Map) {
            jniValue = getJNIHashMap(value.toMap());
            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                jniValue.object<jobject>());
        }
        else if (value.type() == QVariant::Bool) {
            jniValue = QAndroidJniObject::callStaticObjectMethod("java/lang/Boolean", "valueOf", "(Z)Ljava/lang/Boolean;", value.toInt());
            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                jniValue.object<jobject>());
        }
        else if (value.type() == QVariant::List) {
            QAndroidJniObject jniList = QAndroidJniObject("java/util/ArrayList");
            const QVariantList list = value.toList();

            for (int i = 0; i < list.size(); i++) {
                QAndroidJniObject o = QAndroidJniObject::fromString(list.at(i).toString());
                jniList.callMethod<jboolean>("add", "(Ljava/lang/Object;)Z", o.object<jstring>());
            }

            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                jniList.object<jobject>());
        }
    }

    return hashMapClass;
}

void AndroidUtils::emitButtonPressedSignals(bool isBackButton, bool isMenuButton)
{
    if (QGuiApplication::applicationState() != Qt::ApplicationActive) {
        return;
    }

    AndroidUtils *utils = nullptr;

    for (int i = m_Instances.size() - 1; i > -1; i--) {
        if (m_Instances.at(i) != nullptr && m_Instances.at(i)->isButtonEventsEnabled()) {
            utils = m_Instances.at(i);
            break;
        }
    }

    if (utils) {
        if (isBackButton) {
            utils->emitBackButtonPressed();
        }
        else if (isMenuButton) {
            utils->emitMenuButtonPressed();
        }
    }
}

void AndroidUtils::emitAlertDialogClickedSignals(int buttonIndex)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitAlertDialogClicked(buttonIndex);
    }
}

void AndroidUtils::emitDatePickedSignals(int year, int month, int day)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitDatePicked(year, month, day);
    }
}

void AndroidUtils::emitTimePickedSignals(int hourOfDay, int minute)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitTimePicked(hourOfDay, minute);
    }
}

void AndroidUtils::emitCameraCapturedSignals(const QString &capturePath)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitCameraCaptured(capturePath);
    }
}

void AndroidUtils::emitFileSelectedSignals(const QString &filePath)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitFileSelected(filePath);
    }
}

void AndroidUtils::emitKeyboardHeightChangedSignals(const int &keyboardHeight)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitKeyboardHeightChanged(keyboardHeight);
    }
}


void AndroidUtils::emitCameraCaptureCancelledSignals()
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitCameraCaptureCancelled();
    }
}

void AndroidUtils::emitFileSelectionCancelledSignals()
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitFileSelectionCancelled();
    }
}

bool AndroidUtils::isButtonEventsEnabled() const
{
    return m_IsButtonEventsEnabled;
}

void AndroidUtils::setButtonEventsEnabled(bool enabled)
{
    if (m_IsButtonEventsEnabled != enabled) {
        emit buttonEventsEnabledChanged();
    }

    m_IsButtonEventsEnabled = enabled;
}

bool AndroidUtils::isEnabled() const
{
    return this->signalsBlocked();
}

void AndroidUtils::setEnabled(bool enabled)
{
    if (!this->signalsBlocked() != enabled) {
        emit enabledChanged();
    }

    this->blockSignals(!enabled);
}

}
