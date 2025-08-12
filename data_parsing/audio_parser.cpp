#include "audio_parser.h"

audio_parser::audio_parser(QObject *parent)
    : QObject{parent}
{
    m_audio_decoder = new QAudioDecoder();
}

void audio_parser::parse_audio_file(const QString &file_path)
{

    m_audio_decoder->setSource(QUrl::fromUserInput(file_path));

    pcm_data.clear();

    qInfo() << "Parsing audio file:" << file_path;

    QObject::connect(m_audio_decoder, &QAudioDecoder::bufferReady, this, [this]() {
        QAudioBuffer buffer = m_audio_decoder->read();
        QAudioFormat format = buffer.format();

        // Check sample format
        if (format.sampleFormat() == QAudioFormat::Float) {

            int count = buffer.sampleCount();

            const float* samples = buffer.constData<float>();

            for (int index = 0; index < count; index++) {
                qint16 int_sample = static_cast<qint16>(samples[index] * 32767); // scale down

                pcm_data.append(int_sample);
            }

        } else if (format.sampleFormat() == QAudioFormat::Int16) {
            const qint16* samples = buffer.constData<qint16>();
            int count = buffer.sampleCount();
            for (int index = 0; index < count; ++index) {
                qint16 sample = static_cast<qint16>(samples[index]); // scale down
                pcm_data.append(sample);
            }
        } else {
            qWarning() << "Unsupported format" << format.sampleFormat();
        }
    });

    QObject::connect(m_audio_decoder, &QAudioDecoder::finished, this, [this]() {
        qInfo() << "Decoding finished. PCM data size:" << pcm_data.size();
        emit audio_parsing_finished(pcm_data); // <-- emit your own signal!
    });

    m_audio_decoder->start();

}
