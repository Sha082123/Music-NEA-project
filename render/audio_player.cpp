#include "audio_player.h"
#include "../track_objects/track_manager.h"
#include "../part_objects/part_manager.h"

audio_player::audio_player(QObject *parent)
    : QObject{parent}
{
    m_rtaudio = new RtAudio();
    m_playback_data = new audio_playback_data();
    m_part_manager = nullptr;

    sample_rate = 44100;
    buffer_frames = 512;

    parameters.deviceId = m_rtaudio->getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    settotal_ms (1000);
    setdecibels(-1000.0f);
    setqml_position (0);

}

void audio_player::set_part_manager(part_manager *part_manager)
{
    m_part_manager = part_manager;
}

void audio_player::load_audio(QVector<QVector<qint16>> &pcm_buffer, track_manager *track_manager)
{
    //qInfo() << pcm_buffer.size();

    // Determine the number of devices available

    //qInfo() << m_part_manager;

    int devices = m_rtaudio->getDeviceCount();

    std::vector<unsigned int> deviceIds = m_rtaudio->getDeviceIds();

    // Scan through devices for various capabilities
    for (unsigned int deviceId : deviceIds) {
        RtAudio::DeviceInfo info = m_rtaudio->getDeviceInfo(deviceId);
        qInfo() << "deviceId =" << deviceId;
        qInfo() << ": maximum output channels =" << info.outputChannels << "\n";
    }

    if ( deviceIds.size() < 1 ) {
        qInfo() << "\nNo audio devices found!\n";
        m_rtaudio = nullptr;
    }

    //qInfo() << m_rtaudio->getDefaultOutputDevice();



    m_playback_data->pcm_buffer = pcm_buffer;
    m_playback_data->current_position = 0;
    m_playback_data->self = this;
    m_playback_data->master_volume = 1.0f;
    m_playback_data->total_samples = 0;

    setqml_position (0);

    struct track_manager::playback_states m_playback_states = track_manager->get_playback_states();

    for (int index = 0; index < m_playback_data->pcm_buffer.count(); index++ ) {
        int total_samples = m_playback_data->pcm_buffer[index].size();
        if (total_samples > m_playback_data->total_samples) {
            m_playback_data->total_samples = total_samples;
        }

        m_playback_data->mute.append(m_playback_states.mute_list[index] ? 0.0f : 1.0f);

        m_playback_data->volumes.append(m_playback_states.volume_list[index]);
        //m_playback_data->mute.append(1.0f);
    }

    for (auto &track : m_playback_data->pcm_buffer) {
        if (track.size() < m_playback_data->total_samples) {
            track.append(QVector<qint16>(m_playback_data->total_samples - track.size(), 0)); // Pad with zeros to match the size of the largest track
        }
    }

    qInfo() << m_playback_data->total_samples;

    if ( m_rtaudio->openStream( &parameters, NULL, RTAUDIO_SINT16, sample_rate,
                              &buffer_frames, &pcm_callback, (void *)m_playback_data ) ) {
        qInfo() << '\n' << m_rtaudio->getErrorText() << '\n' << Qt::endl;
        m_rtaudio = nullptr; // problem with device settings
    }

    settotal_ms((m_playback_data->total_samples / (sample_rate * parameters.nChannels)) * 1000);



}

void audio_player::close_stream()
{
    m_rtaudio->closeStream ();
}

void audio_player::play_audio()
{
    if ( m_rtaudio->startStream() ) {
        qInfo() << "yo " << m_rtaudio->getErrorText() << Qt::endl;
        m_rtaudio->closeStream();
    }
}

void audio_player::pause_audio()
{
    m_rtaudio->stopStream();
}

void audio_player::refresh()
{
    m_rtaudio->closeStream ();

    parameters.deviceId = m_rtaudio->getDefaultOutputDevice();
    m_rtaudio->openStream(&parameters, NULL, RTAUDIO_SINT16, sample_rate,
                          &buffer_frames, &pcm_callback, (void *)m_playback_data);
}

void audio_player::back_10s()
{
    if (m_playback_data->current_position <= 10 * sample_rate * parameters.nChannels) {
        m_playback_data->current_position = 0; // Prevent going back before the start
    } else {
        m_playback_data->current_position -= 10 * sample_rate * parameters.nChannels;
    }

    setqml_position (m_playback_data->current_position);
}



void audio_player::forward_10s()
{
    if (m_playback_data->current_position >= m_playback_data->total_samples - (10 * sample_rate * parameters.nChannels)) {
        m_playback_data->current_position = m_playback_data->current_position = m_playback_data->total_samples - 1;
    } else {
        m_playback_data->current_position += 10 * sample_rate * parameters.nChannels;
    }

    setqml_position (m_playback_data->current_position);
}

void audio_player::skip_to_start()
{
    m_playback_data->current_position = 0;
    setqml_position (m_playback_data->current_position);
}

void audio_player::skip_to_end()
{
    m_playback_data->current_position = m_playback_data->total_samples - 1;
    setqml_position (m_playback_data->current_position);
    m_rtaudio->stopStream();
}

void audio_player::set_position(const int position)
{
    qInfo() << position;

    double factor = static_cast<double>(position)/static_cast<double>(m_total_ms);
    double decimal_position = factor * (m_playback_data->total_samples - 1);

    size_t new_position = static_cast<size_t>(decimal_position);

    qInfo() << new_position;

    m_playback_data->current_position = new_position;
}

void audio_player::set_volume(int track_index, float volume)
{
    m_playback_data->volumes[track_index] = volume;
}

void audio_player::set_master_volume(float volume)
{
    m_playback_data->master_volume = volume;
}

void audio_player::mute(int index)
{
    m_playback_data->mute[index] = 0.0f;
}

void audio_player::unmute(int index)
{
    m_playback_data->mute[index] = 1.0f;
}




int audio_player::pcm_callback(void *output_buffer, void *input_buffer, unsigned int n_buffer_frames,
                               double stream_time, RtAudioStreamStatus status, void *user_data)
{
    //qInfo() << "pcm callback triggered";

    audio_playback_data *data = static_cast<audio_playback_data*>(user_data);
    qint16 *output = static_cast<qint16*>(output_buffer);

    //qInfo() << data->self;

    unsigned int channels = 2;
    unsigned int samples_needed = n_buffer_frames * channels;
    size_t position = data->current_position;
    size_t samples_left = data->total_samples - position;
    size_t samples_to_copy = qMin(samples_needed, samples_left);

    //qInfo() << data->pcm_buffer.size();

    QVector<qint16> decibel_samples;

    //qInfo() << samples_to_copy;


    for (int index = 0; index < samples_to_copy; ++index) {
        //qInfo() << index;

        qint16 current_sample = 0;

        for (int track_number = 0; track_number < data->pcm_buffer.size(); ++track_number) {
            current_sample += static_cast<qint16>(data->pcm_buffer[track_number][index+position] * data->volumes[track_number] *
                                                  data->mute[track_number] * data->master_volume);

            //qInfo() << data->volumes[track_number];
        }

        //qInfo() << current_sample;

        output[index] = current_sample;
        decibel_samples.append(current_sample);



        if (index % 1470 == 0) { // Update decibels at 30fps

            //qInfo() << "hello";



            // Calculate RMS
            float sum = 0.0;
            for (auto s : decibel_samples)
                sum += s * s;
            float rms = sqrt(sum / decibel_samples.size());

            // Convert to dB
            float db = 20.0 * log10(std::max(rms / 32768.0, 1e-10));

            data->self->setdecibels(db);

            decibel_samples.clear();
        }



    }

    data->current_position += samples_to_copy;
    //qInfo() << data->current_position;

    //qInfo() << stream_time;
    // data->self->setqml_position (stream_time * 1000);

    // if (samples_to_copy > 0) {
    //     memcpy(output, &(pcm_to_play), samples_to_copy * sizeof(qint16));
    //     data->current_position += samples_to_copy;
    // }
    if (samples_to_copy < samples_needed) {
        memset(output + samples_to_copy, 0, (samples_needed - samples_to_copy) * sizeof(qint16)); // fill location at output + samples to copy with 0s
    }

    if (data->current_position >= data->total_samples) {
        data->self->setqml_position(data->total_samples - 1); // Prevent going out of bounds
        //qInfo() << data->self->qml_position();
        data->self->m_part_manager->set_tracker_time(data->self->qml_position());

    } else {
        data->self->setqml_position(data->current_position);
        //qInfo() << data->self->qml_position();
        data->self->m_part_manager->set_tracker_time(data->self->qml_position());

        // for (int index = 0; index < 5; ++index) {
        //     qInfo() << index;
        // }

    }


    return 0; // if reached the end, then stop stream
}

size_t audio_player::qml_position() const
{
    return m_qml_position;
}

void audio_player::setqml_position(size_t newQml_position)
{

    newQml_position = (float(newQml_position) / float(sample_rate * parameters.nChannels)) * 1000;

    if (m_qml_position == newQml_position)
        return;
    m_qml_position = newQml_position;
    emit qml_positionChanged();
}

size_t audio_player::total_ms() const
{
    return m_total_ms;
}

void audio_player::settotal_ms(size_t newTotal_ms)
{
    if (m_total_ms == newTotal_ms)
        return;
    m_total_ms = newTotal_ms;
    emit total_msChanged();
}

float audio_player::decibels() const
{
    return m_decibels;
}

void audio_player::setdecibels(float newDecibels)
{
    if (qFuzzyCompare(m_decibels, newDecibels))
        return;
    m_decibels = newDecibels;
    emit decibelsChanged();
}
