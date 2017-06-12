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
     * `title` and `message` properties are mandatory. And at least one type of button should be given.
     *
     * If the property contains the `items` key then the other buttons will be ignored, the item indexes will be reported with the `buttonIndex` variable.
     * Be careful that when you set the `message` property the `items` will be ignored.
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
     * @endcode
     * @param dialogData
     */
    Q_INVOKABLE void showAlertView(const QVariantMap &dialogProperties);

signals:
    void alertDialogClicked(int buttonIndex);

private:
    iOSNativeUtils *m_iOSNative;
};

}
