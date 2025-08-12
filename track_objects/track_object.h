#ifndef TRACK_OBJECT_H
#define TRACK_OBJECT_H

#include <QObject>
#include <QFileDialog>
#include "../data_parsing/audio_parser.h"
#include "../file_opening/audio_options.h"

class track_manager;


class track_object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString track_name READ track_name WRITE settrack_name NOTIFY track_nameChanged FINAL)
public:
    explicit track_object(QObject *parent = nullptr, track_manager* track_manager = nullptr, QString track_name = "");

    void open_file(QString &file_path);
    audio_options::options get_track_options();

    QVector<qint16> pcm_buffer();

    QString track_name() const;
    void settrack_name(const QString &newTrack_name);

signals:

    void track_nameChanged();

public slots:

    void handle_audio_parsing_finished(const QVector<qint16> pcm_data);

private:

    audio_parser* m_audio_parser = nullptr;
    track_manager* m_track_manager = nullptr;
    audio_options* m_audio_options = nullptr;

    audio_options::options m_track_options;

    QString m_track_name;
    QString m_directory;

    QVector<qint16> m_pcm_buffer;
};

#endif // TRACK_OBJECT_H
