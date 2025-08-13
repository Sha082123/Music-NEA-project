#ifndef AUDIO_PARSER_H
#define AUDIO_PARSER_H

#include <QObject>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QFile>
#include <QUrl>
#include <QVector>
#include <QDebug>

class audio_parser : public QObject
{
    Q_OBJECT
public:
    explicit audio_parser(QObject *parent = nullptr);

    void parse_audio_file(const QString &file_path); // Parses the audio file and returns PCM data
    void clear_parser();

signals:

    void audio_parsing_finished(QVector<qint16> pcm_data);

private:
    QVector<qint16> pcm_data;
    QAudioDecoder* m_audio_decoder = nullptr;
};

#endif // AUDIO_PARSER_H
