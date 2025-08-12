#ifndef AUDIO_OPTIONS_H
#define AUDIO_OPTIONS_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QDir>

class audio_options : public QObject
{
    Q_OBJECT
public:

    struct options {
        bool mute = false;
        bool solo = false;
        float volume = 1.0f;
        int start_ms = 0;
        int end_ms = 0;
        int duration = 0;
        int time_before_start = 0;
    };

    explicit audio_options(QObject *parent = nullptr);

    options initialise_file(const QString &file_path);
    void write_options_file();



signals:

private:

    options read_options_file();

    QString m_file_path;

    options* m_options;




};

#endif // AUDIO_OPTIONS_H
