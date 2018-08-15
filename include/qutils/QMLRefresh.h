#pragma once
// Qt
#include <QObject>

// Forward Declerations
class QJSEngine;
class QQmlEngine;
class QQuickWindow;
class QQmlApplicationEngine;

namespace zmc
{

/*!
 * \class QMLRefresh
 * \brief The QMLRefresh is a helper class to reload QML on runtime.
 *
 * To use it, pass the QQmlApplicationEngine pointer and call the reload function when you need.
 * For more information about runtime reloading go here -> http://www.slideshare.net/ICSinc/how-best-to-realize-a-runtime-reload-of-qml
 *
 * We can also get the main.qml file using QResource but that would mean that we only get the version of the file
 * embedded in the executable. But the user may want to load the files from the file system.
 */
class QMLRefresh : public QObject
{
    Q_OBJECT

public:
    explicit QMLRefresh(QObject *parent = nullptr);

    /*!
     * \brief The singleton provider for Cuz class.
     * \return QObject *
     */
    static QObject *singletonProvider(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    /*!
     * \brief Reloads the main QML file and refreshes the window. If a delay is given, the window will be refreshed after the delay.
     * \param delay
     */
    Q_INVOKABLE void reload(float delay = 0.f);

    /*!
     * \brief Returns the path of the main qml file.
     * \return
     */
    Q_INVOKABLE QString getMainQMLFile() const;

    /*!
     * \brief This is the main file that will be loaded when the window is refreshed.
     * \param file
     */
    Q_INVOKABLE void setMainQMLFile(const QString &file);

private:
    static QMLRefresh *m_Instance;

    QQmlApplicationEngine *m_Engine;
    QQuickWindow *m_Window;
    QString m_MainQMLFile;

private:
    void reloadCache();
};

}
