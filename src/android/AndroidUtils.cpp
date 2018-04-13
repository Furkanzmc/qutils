#include "qutils/android/AndroidUtils.h"
// Qt
#include <QDebug>
#include <QGuiApplication>
#include <QtAndroid>
#include <QTimer>
// qutils
#include "qutils/Macros.h"

#define ANDROID_UTILS_CLASS "org/zmc/qutils/AndroidUtils"

namespace zmc
{

QList<AndroidUtils *> AndroidUtils::m_Instances = QList<AndroidUtils *>();
QString AndroidUtils::m_URLOpenedWith = "";

AndroidButtonEvent::AndroidButtonEvent(QObject *parent)
    : QObject(parent)
    , m_IsAccepted(false)
{

}

bool AndroidButtonEvent::isAccepted() const
{
    return m_IsAccepted;
}

void AndroidButtonEvent::setAccepted(bool accepted)
{
    const bool changed = m_IsAccepted != accepted;
    if (changed) {
        m_IsAccepted = accepted;
        emit acceptedChanged();
    }
}

AndroidUtils::AndroidUtils(QObject *parent)
    : QObject(parent)
    , m_InstanceID(m_Instances.size())
    , m_IsAlertShown(false)
    , m_IsDatePickerShown(false)
    , m_IsTimePickerShown(false)
    , m_IsCameraShown(false)
    , m_IsGalleryShown(false)
    , m_IsMainController(false)
    , m_IsButtonEventsEnabled(false)
{
    m_Instances.append(this);
    if (isMainController()) {
        if (m_URLOpenedWith.length() > 0) {
            /*
             * This is to execute the `emitOpenedWithURLSignals` function with the next cycle.
             * This is used to make sure that NativeUtils and AndroidUtils can catch the emitted signal.
             */
            QTimer::singleShot(1, std::bind(AndroidUtils::emitOpenedWithURLSignal, m_URLOpenedWith));
            m_URLOpenedWith = "";
        }
        else {
            /*
             * This is to execute the `emitOpenedWithURLSignals` function with the next cycle.
             * This is used to make sure that NativeUtils and AndroidUtils can catch the emitted signal.
             */
            QTimer::singleShot(1, std::bind(AndroidUtils::emitOpenedWithoutURLSignal));
        }
    }
}

AndroidUtils::~AndroidUtils()
{
    m_Instances[m_InstanceID] = nullptr;
}

void AndroidUtils::setStatusBarColor(QColor color)
{
    if (color.isValid()) {
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
    else {
        LOG_ERROR("Given color is not valid!");
    }
}

QString AndroidUtils::getStatusBarColor()
{
    const QAndroidJniObject color = QAndroidJniObject::callStaticObjectMethod(ANDROID_UTILS_CLASS, "getStatusBarColor", "()Ljava/lang/String;");
    return color.toString();
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

bool AndroidUtils::isStatusBarVisible() const
{
    return QAndroidJniObject::callStaticMethod<jboolean>(ANDROID_UTILS_CLASS, "isStatusBarVisible", "()Z") == 1;
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

void AndroidUtils::openGallery(const QString &fileType)
{
    if (this->signalsBlocked()) {
        return;
    }

    m_IsGalleryShown = true;
    auto runnable = [fileType]() {
        const QAndroidJniObject jniStr = QAndroidJniObject::fromString(fileType);
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "openGallery",
            "(Ljava/lang/String;)V",
            jniStr.object<jstring>());
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::dismissKeyboard()
{
    auto runnable = []() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "dismissKeyboard",
            "()V");
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

QString AndroidUtils::getDeviceModel()
{
    QAndroidJniObject jniStr = QAndroidJniObject::callStaticObjectMethod(
                                   ANDROID_UTILS_CLASS,
                                   "getDeviceModel",
                                   "()Ljava/lang/String;");
    return jniStr.toString();
}

void AndroidUtils::emitBackButtonPressed(AndroidButtonEvent *event)
{
    if (this->signalsBlocked()) {
        return;
    }

    if (m_IsButtonEventsEnabled) {
        emit backButtonPressed(event);
    }
}

void AndroidUtils::emitMenuButtonPressed(AndroidButtonEvent *event)
{
    if (this->signalsBlocked()) {
        return;
    }

    if (m_IsButtonEventsEnabled) {
        emit menuButtonPressed(event);
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

    const int currentCount = m_Instances.count() - 1;
    AndroidButtonEvent *event = new AndroidButtonEvent();
    for (int index = currentCount; index > -1; index--) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils != nullptr && utils->isButtonEventsEnabled()) {
            if (isBackButton) {
                utils->emitBackButtonPressed(event);
            }
            else if (isMenuButton) {
                utils->emitMenuButtonPressed(event);
            }

            if (event->isAccepted()) {
                break;
            }
        }
    }

    event->deleteLater();
}

void AndroidUtils::emitAlertDialogClickedSignals(int buttonIndex)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitAlertDialogClicked(buttonIndex);
        }
    }
}

void AndroidUtils::emitDatePickedSignals(int year, int month, int day)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitDatePicked(year, month, day);
        }
    }
}

void AndroidUtils::emitTimePickedSignals(int hourOfDay, int minute)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitTimePicked(hourOfDay, minute);
        }
    }
}

void AndroidUtils::emitCameraCapturedSignals(const QString &capturePath)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitCameraCaptured(capturePath);
        }
    }
}

void AndroidUtils::emitFileSelectedSignals(const QString &filePath)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitFileSelected(filePath);
        }
    }
}

void AndroidUtils::emitKeyboardHeightChangedSignals(const int &keyboardHeight)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitKeyboardHeightChanged(keyboardHeight);
        }
    }
}


void AndroidUtils::emitCameraCaptureCancelledSignals()
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitCameraCaptureCancelled();
        }
    }
}

void AndroidUtils::emitFileSelectionCancelledSignals()
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils) {
            utils->emitFileSelectionCancelled();
        }
    }
}

void AndroidUtils::emitOpenedWithURLSignal(const QString &url)
{
    if (m_Instances.size() == 0) {
        m_URLOpenedWith = url;
    }
    else {
        const int currentCount = m_Instances.count();
        for (int index = 0; index < currentCount; index++) {
            AndroidUtils *utils = m_Instances.at(index);
            if (utils && utils->isMainController()) {
                emit utils->openedWithURL(url);
                break;
            }
        }
    }
}

void AndroidUtils::emitOpenedWithoutURLSignal()
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils && utils->isMainController()) {
            emit utils->openedWithoutURL();
            break;
        }
    }
}

bool AndroidUtils::isMainController() const
{
    return m_IsMainController;
}

void AndroidUtils::setMainController(bool IsMainController)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        AndroidUtils *utils = m_Instances.at(index);
        if (utils && utils->isMainController()) {
            utils->setMainController(false);
            break;
        }
    }

    if (m_IsMainController != IsMainController) {
        emit mainControllerChanged();
    }

    m_IsMainController = IsMainController;
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
