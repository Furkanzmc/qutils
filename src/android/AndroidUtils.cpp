
#include "qutils/android/AndroidUtils.h"
// Qt
#include <QDebug>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
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
    jclass javaClass = env->FindClass("org/zmc/qutils/notification/CppCallbacks");
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

std::vector<AndroidUtils *> AndroidUtils::m_Instances = std::vector<AndroidUtils *>();
int AndroidUtils::m_LastInstanceID = 0;

AndroidUtils::AndroidUtils(QObject *parent)
    : QObject(parent)
    , m_InstanceID(m_LastInstanceID)
    , m_IsAlertShown(false)
    , m_IsDatePickerShown(false)
    , m_IsTimePickerShown(false)
{
    m_Instances.push_back(this);
    m_LastInstanceID++;
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
    m_IsAlertShown = true;
    const QAndroidJniObject hashMapClass = getHashMap(dialogProperties);
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
    m_IsTimePickerShown = true;
    auto runnable = []() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "showTimePicker",
            "()V");
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::emitBackButtonPressed()
{
    emit backButtonPressed();
}

void AndroidUtils::emitMenuButtonPressed()
{
    emit menuButtonPressed();
}

void AndroidUtils::emitAlertDialogClicked(int buttonType)
{
    if (m_IsAlertShown) {
        m_IsAlertShown = false;
        if (buttonType == -2) {
            emit alertDialogCancelled();
        }
        else {
            emit alertDialogClicked(buttonType);
        }
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

QAndroidJniObject AndroidUtils::getHashMap(const QVariantMap &map) const
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
            jniValue = getHashMap(value.toMap());
            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                jniValue.object<jobject>());
        }
        else if (value.type() == QVariant::Bool) {
            hashMapClass.callObjectMethod(
                "put",
                "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                jniKey.object<jstring>(),
                value.toBool());
        }
    }

    return hashMapClass;
}

void AndroidUtils::emitButtonPressedSignals(bool isBackButton, bool isMenuButton)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        if (isBackButton) {
            utils->emitBackButtonPressed();
        }
        else if (isMenuButton) {
            utils->emitMenuButtonPressed();
        }
    }
}

void AndroidUtils::emitAlertDialogClickedSignals(int buttonType)
{
    for (AndroidUtils *utils : m_Instances) {
        if (utils == nullptr) {
            continue;
        }

        utils->emitAlertDialogClicked(buttonType);
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

}
