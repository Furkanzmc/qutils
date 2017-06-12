#pragma once
#include <QObject>
#include <QVariantMap>

class iOSNativeUtils;

namespace zmc
{

class iOSUtils : public QObject
{
    Q_OBJECT

public:
    explicit iOSUtils(QObject *parent = nullptr);

    /**
     * @brief Shows a native AlertDialog according to the given dialog properties.
     * You can mix the properties for both Android and iOS. Only the related properties will be used by this function.
     *
     * **Example**
     * @code
     * // Show an alert sheet with items
     * var properties = {
     *     "title": "Select An Item",
     *     "buttons": [
     *         "Button 1",
     *         "Button 2"
     *     ]
     * };
     *
     * // This will have the same effect on both iOS and Android
     * var properties = {
     *     "title": "Select An Item",
     *     "positive": "Button 2",
     *     "negative": "Button 1",
     *     "buttons": [
     *         "Button 1",
     *         "Button 2"
     *     ]
     * };
     * @endcode
     * @param dialogData
     */
    Q_INVOKABLE void showAlertView(const QVariantMap &dialogProperties);

    /**
     * @brief Show the default share dialog on iOS. There's no dialog title on iOS.
     * @param text
     */
    Q_INVOKABLE void shareText(const QString &text);

signals:
    void alertDialogClicked(int buttonIndex);

private:
    iOSNativeUtils *m_iOSNative;
};

}
