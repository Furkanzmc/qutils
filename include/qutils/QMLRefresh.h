#pragma once
// std
#include <functional>
// Qt
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QQuickWindow>
#include <QQmlApplicationEngine>

namespace zmc
{

/**
 * @brief The QMLRefresh is a helper class to reload QML on runtime. To use it, pass the QQmlApplicationEngine pointer and call the reload function when you need.
 * For more information about runtime reloading go here -> http://www.slideshare.net/ICSinc/how-best-to-realize-a-runtime-reload-of-qml
 * @code
 *     // in main.cpp
 *     QMLRefresh manager(&engine);
 *     manager.setMainQMLFile("file:///E:/Development/Playground/Live-QML/qml/main.qml");
 *     engine.rootContext()->setContextProperty("QM", &manager);
 * @endcode
 *
 * Pass the `QQuickWindow` to `QMLRefresh`, then use QM.relad() to reload at runtime!
 * @code
 *     Window {
 *         id: mainWindow
 *         Component.onCompleted: QM.setWindow(mainWindow)
 *
 *         MouseArea {
 *             anchors.fill: parent
 *             onClicked: {
 *                 QM.reload();
 *             }
 *         }
 *     }
 * @endcode
 */
class QMLRefresh : public QObject
{
    Q_OBJECT

public:
    explicit QMLRefresh(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    /**
     * @brief Reloads the main QML file and refreshes the window. If a delay is given, the window will be refreshed after the delay.
     * @param delay
     */
    Q_INVOKABLE void reload(float delay = 0.f);

    /**
     * @brief Set the main window. You may end up getting multiple windows when you reload If you do not set the main window here.
     * @param window
     */
    Q_INVOKABLE void setWindow(QQuickWindow *window);

    /**
     * @brief Returns the path of the main qml file.
     * @return
     */
    Q_INVOKABLE QString getMainQMLFile() const;

    /**
     * @brief This is the main file that will be loaded when the window is refreshed.
     * @param file
     */
    Q_INVOKABLE void setMainQMLFile(const QString &file);

private:
    QQmlApplicationEngine *m_Engine;
    QQuickWindow *m_Window;
    QString m_MainQMLFile;

private:
    void reloadCache();
};

}
