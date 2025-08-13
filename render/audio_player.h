#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QObject>
#include <QDebug>
#include "RtAudio.h"

class track_manager; // Forward declaration of track_manager

class audio_player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(size_t qml_position READ qml_position WRITE setqml_position NOTIFY qml_positionChanged FINAL) // in milliseconds
    Q_PROPERTY(size_t total_ms READ total_ms WRITE settotal_ms NOTIFY total_msChanged FINAL)
    Q_PROPERTY(float decibels READ decibels WRITE setdecibels NOTIFY decibelsChanged FINAL)

public:
    explicit audio_player(QObject *parent = nullptr);

    void load_audio(QVector<QVector<qint16>> &pcm_buffer, track_manager* track_manager); // returns total milliseconds
    void close_stream();
    Q_INVOKABLE void play_audio();
    Q_INVOKABLE void pause_audio();

    Q_INVOKABLE void refresh();

    Q_INVOKABLE void back_10s();
    Q_INVOKABLE void forward_10s();

    Q_INVOKABLE void skip_to_start();
    Q_INVOKABLE void skip_to_end();

    Q_INVOKABLE void set_position(const int position);

    Q_INVOKABLE void set_volume(int track_index, float volume);
    Q_INVOKABLE void set_master_volume(float volume);
    Q_INVOKABLE void mute(int index);
    Q_INVOKABLE void unmute(int index);


    static int pcm_callback(void *output_buffer, void *input_buffer, unsigned int n_buffer_frames,
                           double stream_time, RtAudioStreamStatus status, void *user_data);



    size_t qml_position() const;
    void setqml_position(size_t newQml_position);

    size_t total_ms() const;
    void settotal_ms(size_t newTotal_ms);

    float decibels() const;
    void setdecibels(float newDecibels);

signals:

    void qml_positionChanged();
    void sample_position_changed(size_t current_position);

    void total_msChanged();

    void decibelsChanged();

private:
    struct audio_playback_data {
        QVector<QVector<qint16>> pcm_buffer;
        float master_volume;
        QVector<float> volumes; // Stores the volume for each track
        QVector<float> mute; // Stores the mute state for each track
        size_t current_position = 0;
        size_t total_samples = 0;
        audio_player *self = nullptr;
    };

    audio_playback_data* m_playback_data = nullptr;

    RtAudio *m_rtaudio = nullptr;

    RtAudio::StreamParameters parameters;
    unsigned int sample_rate;
    unsigned int buffer_frames;

    size_t m_total_ms;
    size_t m_qml_position;
    float m_decibels;
};

#endif // AUDIO_PLAYER_H
