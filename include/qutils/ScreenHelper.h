#pragma once
#include <QObject>

namespace zmc
{

/**
 * @brief ScreenHelper is a utility class to help create resolution indepentant UI in QML.
 * @code
 *     // in main.cpp
 *     ScreenHelper manager;
 *     engine.rootContext()->setContextProperty("SH", &manager);
 * @endcode
 *
 * Then use like so:
 * @code
 *     Window {
 *         id: mainWindow
 *
 *         Rectangle {
 *             width: SH.dp(50)
 *         }
 *     }
 * @endcode
 */
class ScreenHelper : public QObject
{
    Q_OBJECT

public:
    explicit ScreenHelper(QObject *parent = 0);

    Q_INVOKABLE qreal dp(const qreal &size);

private:
    const qreal m_DPI;

};

}
