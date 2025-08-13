#include "track_object.h"
#include "track_manager.h"

track_object::track_object(QObject *parent, track_manager *track_manager, QString track_name)
    : QObject{parent}
{

    settrack_name(track_name);
    m_audio_parser = new audio_parser(this);
    m_track_manager = track_manager;
    m_audio_options = new audio_options(this);

    m_track_options = audio_options::options();

    m_file_path = "";

}

void track_object::open_file(QString &file_path)
{
    QObject::connect(m_audio_parser, &audio_parser::audio_parsing_finished,
                     this, &track_object::handle_audio_parsing_finished);

    m_file_path = file_path;
    m_track_options = m_audio_options->initialise_file(file_path);

    qInfo() << Qt::endl << Qt::endl << Qt::endl;
    qInfo() << "Mute:" << m_track_options.mute;
    qInfo() << "Solo:" << m_track_options.solo;
    qInfo() << "Volume:" << m_track_options.volume;
    qInfo() << "Start MS:" << m_track_options.start_ms;
    qInfo() << "End MS:" << m_track_options.end_ms;
    qInfo() << "Duration:" << m_track_options.duration;
    qInfo() << Qt::endl << Qt::endl << Qt::endl;

    m_audio_parser->parse_audio_file(file_path); // Parse the audio file and store PCM data

    qInfo() << "file opened: " << file_path;
}

audio_options::options track_object::get_track_options()
{
    return m_track_options;
}

void track_object::save_options(audio_options::options &new_options)
{
    m_audio_options->update_options (new_options);
}

void track_object::delete_file()
{
    m_audio_parser->clear_parser();
    delete m_audio_parser;

    qInfo() << "Deleting file: " << m_file_path;
    QFile file(m_file_path);
    if (file.exists()) {
        if (file.remove()) {
            qInfo() << "File deleted successfully: " << m_file_path;
            m_file_path.clear(); // Clear the file path after deletion
        } else {
            qWarning() << "Failed to delete file: " << m_file_path;
        }
    } else {
        qWarning() << "File does not exist: " << m_file_path;
    }

    m_audio_options->delete_options();

    delete m_audio_options;

}

QVector<qint16> track_object::pcm_buffer()
{
    return m_pcm_buffer;
}

QString track_object::track_name() const
{
    return m_track_name;
}

void track_object::settrack_name(const QString &newTrack_name)
{
    if (m_track_name == newTrack_name)
        return;
    m_track_name = newTrack_name;
    emit track_nameChanged();
}

void track_object::handle_audio_parsing_finished(const QVector<qint16> pcm_data)
{
    m_pcm_buffer = pcm_data;

    qInfo() << "PCM buffer size: " << m_pcm_buffer.size();



    if (m_track_options.duration == 0) {
        qInfo() << "hello";
        int length = (m_pcm_buffer.size() * 1000) / (44100 * 2); // get duration in milliseconds
        m_track_options.duration = length;
        m_track_options.end_ms = length;
    }


    m_track_manager->track_loaded(); // increase tracks loaded by 1
    qInfo() << "Tracks loaded: " << m_track_manager->get_tracks_loaded();

    if (m_track_manager->get_tracks_loaded() == m_track_manager->get_total_tracks ()) {

        m_track_manager->load_tracks ();
        m_track_manager->setmusic_loaded(true); // Set music loaded to true when all tracks are loaded
    }
}

