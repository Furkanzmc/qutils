#pragma once
#include <QObject>
#include <QMediaRecorder>
#include <QUrl>

// Forward Declerations
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;

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

public:
    explicit AudioRecorder(QObject *parent = nullptr);
    ~AudioRecorder();

public:
    void setOutputPath(QString location);
    QString getOutputPath() const;
    Q_INVOKABLE void pause();

    Q_INVOKABLE void record(float duration = 0.0f);
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
     * @return
     */
    bool hasValidRecording() const;

private:
    QAudioRecorder *m_AudioRecorder;
    QAudioProbe *m_Probe;
    // Duration is in seconds.
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
};

}
