#include "qutils/AudioRecorder.h"
// std
#include <cmath>
// Qt
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QMediaRecorder>
#include <QStandardPaths>
#include <QTimer>
// qutils
#include "qutils/Macros.h"

namespace zmc
{

AudioRecorder::AudioRecorder(QObject *parent)
    : QObject(parent)
    , m_AudioRecorder(new QAudioRecorder(this))
    , m_Probe(nullptr)
    , m_Duration(0)
{
    connect(m_AudioRecorder, &QAudioRecorder::stateChanged, this, &AudioRecorder::onStateChanged);
    connect(m_AudioRecorder, &QAudioRecorder::statusChanged, this, &AudioRecorder::onStatusChanged);
    connect(m_AudioRecorder, static_cast<void(QMediaRecorder::*)(QMediaRecorder::Error)>(&QMediaRecorder::error), this, &AudioRecorder::onErrorOccurred);

    connect(m_AudioRecorder, &QAudioRecorder::durationChanged, this, &AudioRecorder::onDurationChanged);
}

AudioRecorder::~AudioRecorder()
{
    if (m_AudioRecorder) {
        if (isRecording()) {
            m_AudioRecorder->stop();
        }

        delete m_AudioRecorder;
    }
}

void AudioRecorder::setOutputPath(QString location)
{
    location.remove("file:///");
    m_AudioRecorder->setOutputLocation(QUrl::fromLocalFile(location));
}

QString AudioRecorder::getOutputPath() const
{
    return m_AudioRecorder->outputLocation().toString();
}

void AudioRecorder::record(float duration)
{
    if (m_AudioRecorder->state() == QMediaRecorder::StoppedState || m_AudioRecorder->state() == QMediaRecorder::PausedState) {
        if (m_AudioRecorder->state() == QMediaRecorder::StoppedState) {
            m_AudioRecorder->setAudioInput(m_AudioRecorder->defaultAudioInput());

            QAudioEncoderSettings settings;
            settings.setCodec("");
            settings.setQuality(QMultimedia::NormalQuality);
            settings.setSampleRate(-1);
            settings.setBitRate(32000);
            settings.setChannelCount(-1);
            settings.setQuality(QMultimedia::NormalQuality);
            settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

            m_AudioRecorder->setEncodingSettings(settings);
        }

        m_AudioRecorder->record();

        if (duration > 0.f) {
            QTimer::singleShot(duration * 1000, this, &AudioRecorder::stop);
        }
    }
}

void AudioRecorder::pause()
{
    if (m_AudioRecorder->state() == QMediaRecorder::RecordingState) {
        m_AudioRecorder->pause();
    }
}

void AudioRecorder::stop()
{
    if (m_AudioRecorder->state() == QMediaRecorder::RecordingState) {
        m_AudioRecorder->stop();
        m_Duration = 0;
    }
}

void AudioRecorder::deleteRecording()
{
    if (isRecording()) {
        stop();
    }

    QFile file(m_AudioRecorder->outputLocation().toString());
    if (file.exists()) {
        file.remove();
    }
}

bool AudioRecorder::isRecording() const
{
    return m_AudioRecorder->state() == QMediaRecorder::RecordingState;
}

bool AudioRecorder::isPaused() const
{
    return m_AudioRecorder->state() == QMediaRecorder::PausedState;
}

bool AudioRecorder::isStopped() const
{
    return m_AudioRecorder->state() == QMediaRecorder::StoppedState;
}

int AudioRecorder::getDuration() const
{
    return m_Duration;
}

QString AudioRecorder::getWritableDir() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

bool AudioRecorder::hasValidRecording() const
{
    QFile file(m_AudioRecorder->outputLocation().toString());
    return file.exists();
}

void AudioRecorder::onStateChanged(QMediaRecorder::State state)
{
    if (state == QMediaRecorder::RecordingState) {
        LOG("Recording...");
        emit recordingStarted();
    }
    else if (state == QMediaRecorder::PausedState) {
        LOG("Paused...");
        emit recordingPaused();
    }
    else if (state == QMediaRecorder::StoppedState) {
        LOG("Stoppped...");
        emit recordingStopped();
    }
}

void AudioRecorder::onStatusChanged(QMediaRecorder::Status status)
{
    if (status == QMediaRecorder::UnavailableStatus) {
        LOG_ERROR("The recorder is not available or not supported by connected media object.");
    }
    else if (status == QMediaRecorder::UnloadedStatus) {
        LOG("The recorder is avilable but not loaded.");
    }
    else if (status == QMediaRecorder::LoadingStatus) {
        LOG("The recorder is initializing.");
    }
    else if (status == QMediaRecorder::LoadedStatus) {
        LOG("The recorder is initialized and ready to record media.");
    }
    else if (status == QMediaRecorder::StartingStatus) {
        LOG("Recording is requested but not active yet.");
    }
    else if (status == QMediaRecorder::RecordingStatus) {
        LOG("Recording is active.");
    }
    else if (status == QMediaRecorder::PausedStatus) {
        LOG("Recording is paused.");
    }
    else if (status == QMediaRecorder::FinalizingStatus) {
        LOG("Recording is stopped with media being finalized.");
    }
}

void AudioRecorder::onErrorOccurred(QMediaRecorder::Error error)
{
    if (error == QMediaRecorder::Error::NoError) {
        LOG("No error...");
    }
    else if (error == QMediaRecorder::Error::ResourceError) {
        LOG_ERROR("Device is not ready or not available.");
    }
    else if (error == QMediaRecorder::Error::FormatError) {
        LOG_ERROR("Current format is not supported.");
    }
    else if (error == QMediaRecorder::Error::OutOfSpaceError) {
        LOG_ERROR("No space left on device.");
    }
}

void AudioRecorder::onDurationChanged(qint64 duration)
{
    // FIXME: durationChanged is not emitted to QML. It works when I connect a C++ slot to the signal.
    const qint64 newDuration = floor(static_cast<float>(duration) / 1000.f);
    if (m_Duration != newDuration) {
        m_Duration = newDuration;
        emit durationChanged();
    }
}

}
