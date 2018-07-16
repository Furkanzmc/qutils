#pragma once
// Qt
#include <QObject>

// Forward Declerations
#if defined(QT_QML_LIB)
class QJSEngine;
class QQmlEngine;
#endif // QT_QML_LIB

namespace zmc
{

/*!
 * \class SystemInfo
 * \brief The SystemInfo class exposes the methods of QSysInfo.
 *
 * SystemInfo also uses platform specific code to give you more information about the device that it runs on.
 *
 * \sa QSysInfo
 */
class SystemInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString bootUniqueId READ bootUniqueId CONSTANT)
    Q_PROPERTY(QString buildAbi READ buildAbi CONSTANT)
    Q_PROPERTY(QString buildCpuArchitecture READ buildCpuArchitecture CONSTANT)

    Q_PROPERTY(QString currentCpuArchitecture READ currentCpuArchitecture CONSTANT)
    Q_PROPERTY(QString kernelType READ kernelType CONSTANT)
    Q_PROPERTY(QString kernelVersion READ kernelVersion CONSTANT)

    Q_PROPERTY(QString machineHostName READ machineHostName CONSTANT)
    Q_PROPERTY(QString machineUniqueId READ machineUniqueId CONSTANT)
    Q_PROPERTY(QString prettyProductName READ prettyProductName CONSTANT)

    Q_PROPERTY(QString productType READ productType CONSTANT)
    Q_PROPERTY(QString productVersion READ productVersion CONSTANT)
    Q_PROPERTY(QString deviceModel READ deviceModel CONSTANT)

public:
    explicit SystemInfo(QObject *parent = nullptr);

#if defined(QT_QML_LIB)
    static QObject *singletonProvider(QQmlEngine *, QJSEngine *);
#endif // QT_QML_LIB

    /*!
     * \property SystemInfo::bootUniqueId
     * \brief Returns the result from QSysInfo::bootUniqueId()
     *
     * This method is available starting with Qt 5.11. In lower libraries, it will return an empty QByteArray.
     *
     * \note QByteArray is converted to ArrayBuffer by Qt's JS engine.
     * \return QByteArray
     */
    QByteArray bootUniqueId() const;

    /*!
     * \property SystemInfo::buildAbi
     * \brief Returns the result of QSysInfo::buildAbi()
     * \return QString
     */
    QString buildAbi() const;

    /*!
     * \property SystemInfo::buildCpuArchitecture
     * \brief Returns the result of QSysInfo::buildCpuArchitecture()
     * \return QString
     */
    QString buildCpuArchitecture() const;

    /*!
     * \property SystemInfo::currentCpuArchitecture
     * \brief Returns the result of QSysInfo::currentCpuArchitecture()
     * \return QString
     */
    QString currentCpuArchitecture() const;

    /*!
     * \property SystemInfo::kernelType
     * \brief Returns the result of QSysInfo::kernelType()
     * \return QString
     */
    QString kernelType() const;

    /*!
     * \property SystemInfo::kernelVersion
     * \brief Returns the result of QSysInfo::kernelVersion()
     * \return QString
     */
    QString kernelVersion() const;

    /*!
     * \property SystemInfo::machineHostName
     * \brief Returns the result of QSysInfo::machineHostName()
     * \return QString
     */
    QString machineHostName() const;

    /*!
     * \property SystemInfo::machineUniqueId
     * \brief Returns the result from QSysInfo::machineUniqueId()
     *
     * This method is available starting with Qt 5.11. In lower versions, it will return an empty QByteArray.
     *
     * \note QByteArray is converted to ArrayBuffer by Qt's JS engine.
     * \return QByteArray
     */
    QByteArray machineUniqueId() const;

    /*!
     * \property SystemInfo::prettyProductName
     * \brief Returns the result of QSysInfo::prettyProductName()
     * \return QString
     */
    QString prettyProductName() const;

    /*!
     * \property SystemInfo::productType
     * \brief Returns the result of QSysInfo::productType()
     * \return QString
     */
    QString productType() const;

    /*!
     * \property SystemInfo::productVersion
     * \brief Returns the result of QSysInfo::productVersion()
     * \return QString
     */
    QString productVersion() const;

    /*!
     * \property SystemInfo::deviceModel
     * \brief Returns the device model for iOS and Android devices.
     *
     * On macOS this will return the result of `sysctlbyname`.
     * On Windows it will return `windows` for now.
     * On WinRT it will return `winrt` for now.
     *
     * \return QString
     * \todo Add Windows specific code to get the device model.
     */
    QString deviceModel() const;

private:
    static SystemInfo *m_Instance;
};

}
