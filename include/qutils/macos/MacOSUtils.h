#pragma once
// Qt
#include <QObject>

namespace zmc
{

class MacOSUtils : public QObject
{
    Q_OBJECT

public:
    explicit MacOSUtils(QObject *parent = nullptr);
    ~MacOSUtils();

    bool eventFilter(QObject *obj, QEvent *event);

    /**
     * @brief Returns true if this is the main controller.
     * @return bool
     */
    Q_INVOKABLE bool isMainController() const;

    /**
     * @brief Set the main controller to this one. When an instance is set as main controller, certain signals will only be emitted for this instance.
     * Only one instance can be the main controller. When an instance is set as main controller while there is already an existing main controller, the existing
     * main controller will relinquish its position and the new instance will be the new main controller.
     * The signals that are designated to main controller are:
     * - openedWithUrl
     * - openedWithoutUrl
     * @param isMain
     * @return void
     */
    Q_INVOKABLE void setMainController(bool isMain, bool disableOthers = true);

    /**
     * @brief Only the first instance will be notified of this.
     * @param url
     */
    static void emitOpenedWithURLSignal(QString url);

    /**
     * @brief Only the first instance will be notified of this.
     * @param url
     */
    static void emitOpenedWithoutURLSignal();

signals:
    /**
     * @brief This signal is emitted when the app is opened with a URL.
     * Follow this tutorial for how to implement int into your app: https://developer.android.com/training/app-links/index.html
     * @param url
     */
    void openedWithURL(QString url);

    /**
     * @brief This is also called for the first instance. This is just a complementary signal. This is emitted If an app is not opened from a URL.
     */
    void openedWithoutURL();

    /**
     * @brief Emitted when m_IsMainController is changed.
     * @return void
     */
    void mainControllerChanged();

private:
    static QList<MacOSUtils *> m_Instances;

    const int m_InstanceID;
    bool m_IsMainController;
};

}
