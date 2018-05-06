#pragma once
// Qt
#include <QObject>
#include <QProcess>

namespace zmc
{

class UpdateInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(qint64 updateSize READ updateSize CONSTANT)

    Q_PROPERTY(bool valid READ valid CONSTANT)

public:
    UpdateInfo(QObject *parent = nullptr);

    /**
     * @brief Returns the name of the update.
     * @return QString
     */
    QString name() const;

    /**
     * @brief Sets the name of the update.
     * @param name
     * @return void
     */
    void setName(QString name);

    /**
     * @brief Returns the version of this update.
     * @return QString
     */
    QString version() const;

    /**
     * @brief Sets the version for this update.
     * @param ver
     * @return void
     */
    void setVersion(QString ver);

    /**
     * @brief Returns the size of this update.
     * @param size
     * @return qint64
     */
    quint64 updateSize() const;

    /**
     * @brief Set the size of this update.
     * @param size
     * @return void
     */
    void setUpdateSize(quint64 size);

    /**
     * @brief Returns true If the update information has all of the necessary information.
     * @return bool
     */
    bool valid() const;

private:
    QString m_Name, m_Version;
    quint64 m_UpdateSize;
};

/**
 * @brief The UpdateChecker class checks for updates for apps deployed with Qt Installer Framework.
 * It runs the maintanance tool with the option `--checkupdates` and parses the output to see If there is an update.
 * If the maintance tools returns an XML containing the update information, the `updateAvailable()` signal will be emitted.
 * The name of the maintance tools can vary depending on your configiguration.
 * The the fault maintance tool name is "maintenancetool".
 *
 * This class will also be available for non-desktop platforms but will have no functionalty.
 */
class UpdateChecker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString maintenanceToolName READ maintenanceToolName WRITE setMaintenanceToolName NOTIFY maintenanceToolNameChanged)

    Q_ENUM(QProcess::ProcessError)

public:
    explicit UpdateChecker(QObject *parent = nullptr);

    /**
     * @brief Checks for updates and emits `updateAvailable()` If there's a new update.
     * Otherwise does nothing.
     */
    Q_INVOKABLE void checkForUpdates();

    /**
     * @brief Returns the name of the maintenance tool.
     * @return QString
     */
    QString maintenanceToolName() const;

    /**
     * @brief Sets the name of the maintenance tool.
     * @param name
     * @return void
     */
    void setMaintenanceToolName(const QString &name);

    /**
     * @brief Startes the updater in a separate process.
     * @param silentUpdate If set to true, the silent update will begin without the updater UI.
     * @return True If the update starts sucessfuly, otherwıse returns false.
     */
    Q_INVOKABLE bool startUpdater(bool silentUpdate);

signals:
    /**
     * @brief Emittedn when there's a new update available.
     */
    void updateAvailable(QList<QObject *> updates);

    /**
     * @brief Emitted when there's an error checking for update.
     * @param error
     */
    void errorOcurred(int error);

    /**
     * @brief Emitted when the maintenance tool name changes.
     */
    void maintenanceToolNameChanged();

private:
    QString m_MaintenanceToolName;
    QProcess m_Process;

private:
    /**
     * @brief Returns the absolute path of the maintenance tool.
     */
    QString getMaintenanceToolPath() const;

    /**
     * @brief Handles the case where the process finishes succesfuly.
     * @param exitCode
     * @param exitStatus
     * @return void
     */
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    /**
     * @brief Handles the QProcess errors.
     * @param error
     */
    void onProcessErrorOcurred(QProcess::ProcessError error);

    /**
     * @brief Parses the update output and returns a list of update information.
     * @param output
     * @return QList<UpdateInfo *>
     */
    QList<zmc::UpdateInfo *> parseOutPut(const QByteArray &output);
};

}
