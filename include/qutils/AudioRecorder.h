#pragma once
// Qt
#include <QMediaRecorder>
#include <QObject>
#include <QUrl>

// Forward Declerations
class QAudioRecorder;
class QAudioBuffer;
class QAudioProbe;
class QAudioLevel;

namespace zmc
{

class AudioRecorder : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool recording READ isRecording NOTIFY recordingStarted)
    Q_PROPERTY(bool paused READ isPaused NOTIFY recordingPaused)
    Q_PROPERTY(bool stopped READ isStopped NOTIFY recordingStopped)

    Q_PROPERTY(qint64 duration READ getDuration NOTIFY durationChanged)
    Q_PROPERTY(QString writableDir READ getWritableDir CONSTANT)
    Q_PROPERTY(QString outputPath READ getOutputPath WRITE setOutputPath NOTIFY outputPathChanged)

    Q_PROPERTY(bool hasValidRecording READ hasValidRecording CONSTANT)
    Q_PROPERTY(QMediaRecorder::State state READ getState NOTIFY stateChanged)
    Q_PROPERTY(QMediaRecorder::Status status READ getStatus NOTIFY statusChanged)

    Q_ENUM(QMediaRecorder::State)
    Q_ENUM(QMediaRecorder::Status)

public:
    explicit AudioRecorder(QObject *parent = nullptr);
    ~AudioRecorder();

public:
    /**
     * @brief Sets the output path of the recording. This is the file path that is used to store the recording.
     * @param location
     */
    void setOutputPath(QString location);

    /**
     * @brief Returns the output path for the recording.
     * @return QString
     */
    QString getOutputPath() const;

    /**
     * @brief Pauses the recording. Depending on platform recording pause may be not supported, in this case the recorder state stays unchanged.
     */
    Q_INVOKABLE void pause();

    /**
     * @brief If the recorder is already recording does nothing. If the recorder is either stopped or paused, it starts the recording again. If the `duration`
     * is set to any other value than 0, the recording will be automatically stopped when the duration is reached. The duration is reset here.
     * @param duration The value is in seconds.
     */
    Q_INVOKABLE void record(float duration = 0.0f);

    /**
     * @brief The recording is stopped and the file is finalized with the recording. The duration variable stays the same after the recording is stopped.
     * Duration is reset when a new recording is started.
     */
    Q_INVOKABLE void stop();

    /**
     * @brief Deletes the current recording from the file system. If a recording is in process, stops the recording first.
     */
    Q_INVOKABLE void deleteRecording();

    bool isRecording() const;
    bool isPaused() const;
    bool isStopped() const;

    int getDuration() const;
    QString getWritableDir() const;

    /**
     * @brief Returns true If the recording is currently saved on the file system.
     * @return bool
     */
    bool hasValidRecording() const;

    QMediaRecorder::State getState() const;
    QMediaRecorder::Status getStatus() const;

private:
    QAudioRecorder *m_AudioRecorder;

    /**
     * @brief Duration is in seconds.
     */
    int m_Duration;

private:
    void onStateChanged(QMediaRecorder::State state);
    void onStatusChanged(QMediaRecorder::Status status);
    void onErrorOccurred(QMediaRecorder::Error error);

    void onDurationChanged(qint64 duration);

signals:
    void recordingStarted();
    void recordingPaused();
    void recordingStopped();

    void durationChanged();
    void outputPathChanged();
    void hasValidRecordingChanged();

    void stateChanged();
    void statusChanged();
};

}
