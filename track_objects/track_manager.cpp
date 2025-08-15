#include "track_manager.h"
#include "../part_objects/part_manager.h"

track_manager::track_manager(QObject *parent, QQmlApplicationEngine *engine)
    : QObject{parent}
{
    m_file_open = new file_open(nullptr, g_verovio_loader);
    qInfo() << "checkpoint";
    m_audio_player = new audio_player(this);
    m_part_manager = nullptr;

    m_music_loaded = false;

    m_engine = engine;

    m_directory = "";
    m_options_path = "";

    m_main_options = new main_options(this);
    m_sync_points = {};
}

void track_manager::set_part_manager(part_manager *part_manager)
{
    m_part_manager = part_manager;
    m_audio_player->set_part_manager(m_part_manager);
}

void track_manager::clear()
{
    setmusic_loaded(false);
    m_directory = "";
    tracks_loaded = 0;
    setqml_track_list ({});
    for (track_object* track : track_object_list) {
        delete track;
    }
    track_object_list.clear();

    m_audio_player->close_stream ();

    m_engine->rootContext()->setContextProperty("audio_player", nullptr);
    delete m_audio_player;

    m_audio_player = new audio_player(this); // Recreate the audio player
    m_engine->rootContext()->setContextProperty("audio_player", m_audio_player);
    m_audio_player->set_part_manager(m_part_manager);

    m_playback_states = playback_states(); // Reset playback states
    m_mute_list.clear();
    m_solo_list.clear();
    m_volume_list.clear();
    m_start_ms_list.clear();
    m_end_ms_list.clear();
    m_duration_list.clear();
    m_time_before_start_list.clear();

    emit mute_listChanged ();
    emit solo_listChanged ();
    emit volume_listChanged ();
    emit start_ms_listChanged ();
    emit end_ms_listChanged ();
    emit duration_listChanged ();
    emit time_before_start_listChanged ();
    emit qml_track_listChanged ();
    emit music_loadedChanged ();

    setprevent_mute (false);
}

int track_manager::get_track_count()
{
    return track_object_list.size();
}

void track_manager::load_tracks()
{
    QVector<QVector<qint16>> pcm_buffer_2d;
    QVector<qint16> temp_buffer;

    for (track_object* track : track_object_list) {
        temp_buffer = track->pcm_buffer();
        pcm_buffer_2d.append(temp_buffer);      // Append the PCM data from each track

        // get the options and store them in playback states
        audio_options::options current_track_options = track->get_track_options();

        m_playback_states.mute_list.append(current_track_options.mute);
        m_playback_states.solo_list.append(current_track_options.solo);
        m_playback_states.volume_list.append(current_track_options.volume);
        m_playback_states.start_ms_list.append(current_track_options.start_ms);
        m_playback_states.end_ms_list.append(current_track_options.end_ms);
        m_playback_states.duration_list.append(current_track_options.duration);
        m_playback_states.time_before_start_list.append(current_track_options.time_before_start);

        m_mute_list.append(current_track_options.mute);
        m_solo_list.append(current_track_options.solo);
        m_volume_list.append(current_track_options.volume);
        m_start_ms_list.append(current_track_options.start_ms);
        m_end_ms_list.append(current_track_options.end_ms);
        m_duration_list.append(current_track_options.duration);
        m_time_before_start_list.append(current_track_options.time_before_start);

    }

    emit mute_listChanged ();
    emit solo_listChanged ();
    emit volume_listChanged ();
    emit start_ms_listChanged ();
    emit end_ms_listChanged ();
    emit duration_listChanged ();
    emit time_before_start_listChanged ();

    bool solo_exists = false;

    for (auto solo : m_solo_list) {
        if (solo.toBool()) {
            solo_exists = true;
            break;
        }
    }

    setprevent_mute(solo_exists); // If any track is soloed, prevent mute

    qInfo() << Qt::endl << Qt::endl << Qt::endl;
    qInfo() << "mute list: " << m_mute_list;
    qInfo() << "solo list: " << m_solo_list;
    qInfo() << "volume list: " << m_volume_list;
    qInfo() << "start ms list: " << m_start_ms_list;
    qInfo() << "end ms list: " << m_end_ms_list;
    qInfo() << "duration list: " << m_duration_list;
    qInfo() << "time before start list: " << m_time_before_start_list;
    qInfo() << "prevent mute: " << prevent_mute();
    qInfo() << Qt::endl << Qt::endl << Qt::endl;


    // for (auto sample : pcm_buffer_2d[0]) {
    //     qInfo() << sample; // Log the first sample of the first track
    // }



    m_audio_player->load_audio(pcm_buffer_2d, this);

    m_engine->rootContext()->setContextProperty("audio_player", m_audio_player);
}




void track_manager::add_audio_track(QString &file_path)
{
    QString track_name = m_file_open->get_base_name(file_path); // Get the base name of the file

    track_object* new_track_object = new track_object(this, this, track_name);
    track_object_list.append(new_track_object);

    new_track_object->open_file(file_path);

    QVariantList temp_list = qml_track_list();
    temp_list.append(track_name);
    setqml_track_list(temp_list);
}

void track_manager::scan_audio_directory(QString directory)
{
    m_directory = directory + "/";
    m_options_path = m_directory + "options.txt";

    m_sync_points = m_main_options->initialise_options(m_options_path);

    qInfo() << m_sync_points.size();

    QVariantList qml_list = convert_sync_points_to_qml(m_sync_points);
    setqml_sync_points (qml_list);

    qInfo() << Qt::endl << Qt::endl << Qt::endl;
    for (auto point : m_sync_points) {
        qInfo() << "Sync point time:" << point.time << "Measure:" << point.measure << "Beat:" << point.beat;
    }
    qInfo() << Qt::endl << Qt::endl << Qt::endl;

    qInfo() << "Scanning directory:" << m_directory;
    m_music_loaded = false;
    tracks_loaded = 0;

    QDirIterator file(directory, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    while (file.hasNext()) {
        QString next_file = file.next();
        QFileInfo next_info(next_file);
        if (next_info.isFile() && (next_info.suffix() != "txt")) {
            add_audio_track(next_file); // Add the audio track to the manager
        }
    }
}

bool track_manager::music_loaded() const
{
    return m_music_loaded;
}

void track_manager::setmusic_loaded(bool newMusic_loaded)
{
    if (m_music_loaded == newMusic_loaded)
        return;
    m_music_loaded = newMusic_loaded;
    emit music_loadedChanged();
}

void track_manager::track_loaded()
{
    tracks_loaded ++;
}

int track_manager::get_total_tracks()
{
    return track_object_list.size();
}

int track_manager::get_tracks_loaded()
{
    return tracks_loaded;
}

void track_manager::toggle_mute(int index)
{
    if (index < 0 || index >= m_mute_list.size()) {
        qWarning() << "Index out of bounds for mute list.";
        return;
    }

    bool current_mute_state = m_mute_list[index].toBool();

    if (current_mute_state) {
        qInfo() << "Unmuting track at index" << index;
        m_audio_player->unmute(index);
    } else {
        qInfo() << "Muting track at index" << index;
        m_audio_player->mute(index);
    }

    m_mute_list[index] = !current_mute_state; // Toggle the mute state
    m_playback_states.mute_list[index] = !current_mute_state; // Update the playback state

    emit mute_listChanged();

}

void track_manager::toggle_solo(int index)
{
    if (index < 0 || index >= m_mute_list.size()) {
        qWarning() << "Index out of bounds for mute list.";
        return;
    }

    m_solo_list[index] = !(m_solo_list[index].toBool());
    m_playback_states.solo_list[index] = !(m_playback_states.solo_list[index]);
    emit solo_listChanged();

    bool solo_exists = false;

    for (int index = 0; index < m_solo_list.size(); index++) {
        if (m_solo_list[index].toBool() == true) {
            m_audio_player->unmute(index);
            solo_exists = true;
            setprevent_mute (true);
        } else {
            m_audio_player->mute(index);
        }
    }

    if (!solo_exists) { // if no solo, unmute everything
        setprevent_mute (false);
        for (int index = 0; index < m_solo_list.size(); index++) {
            m_audio_player->unmute(index);
            m_mute_list[index] = false;
            m_playback_states.mute_list[index] = false;

            emit mute_listChanged();
        }
    }
}

void track_manager::set_volume(int index, float volume)
{
    m_volume_list[index] = volume;
    m_playback_states.volume_list[index] = volume;
    m_audio_player->set_volume (index, volume);
    emit volume_listChanged();
}

void track_manager::save_playback_states()
{
    for (int index = 0; index < m_mute_list.size(); index++) {
        audio_options::options current_track_options;
        current_track_options.mute = m_mute_list[index].toBool();
        current_track_options.solo = m_solo_list[index].toBool();
        current_track_options.volume = m_volume_list[index].toFloat();
        current_track_options.start_ms = m_start_ms_list[index].toInt();
        current_track_options.end_ms = m_end_ms_list[index].toInt();
        current_track_options.duration = m_duration_list[index].toInt();
        current_track_options.time_before_start = m_time_before_start_list[index].toInt();

        track_object_list.at(index)->save_options(current_track_options);
    }

    m_main_options->update_options(m_sync_points);
}

void track_manager::open_new_track()
{
    // QString target_dir = QDir::currentPath() + "/UserFiles/dump/" +
    //                      m_file_open->name_from_project_files(root_path) + "/tracks/";

    QString target_dir = m_directory;
    QString file_path = m_file_open->createNewTrack(target_dir);

    setmusic_loaded (false);

    m_audio_player->close_stream ();

    m_playback_states = playback_states(); // Reset playback states

    m_mute_list.clear();
    m_solo_list.clear();
    m_volume_list.clear();
    m_start_ms_list.clear();
    m_end_ms_list.clear();
    m_duration_list.clear();
    m_time_before_start_list.clear();

    add_audio_track(file_path);
}

void track_manager::delete_track(int index)
{
    m_audio_player->close_stream ();

    track_object *track_to_delete = track_object_list[index];

    // Remove the track from the QML list
    QVariantList temp_list = qml_track_list();
    temp_list.removeAt(index);
    setqml_track_list(temp_list);

    // Remove the track from the playback states
    // m_playback_states.mute_list.removeAt(index);
    // m_playback_states.solo_list.removeAt(index);
    // m_playback_states.volume_list.removeAt(index);
    // m_playback_states.start_ms_list.removeAt(index);
    // m_playback_states.end_ms_list.removeAt(index);
    // m_playback_states.duration_list.removeAt(index);
    // m_playback_states.time_before_start_list.removeAt(index);

    m_playback_states = playback_states(); // Reset playback states

    // m_mute_list.removeAt(index);
    // m_solo_list.removeAt(index);
    // m_volume_list.removeAt(index);
    // m_start_ms_list.removeAt(index);
    // m_end_ms_list.removeAt(index);
    // m_duration_list.removeAt(index);
    // m_time_before_start_list.removeAt(index);

    m_mute_list.clear();
    m_solo_list.clear();
    m_volume_list.clear();
    m_start_ms_list.clear();
    m_end_ms_list.clear();
    m_duration_list.clear();
    m_time_before_start_list.clear();

    emit mute_listChanged();
    emit solo_listChanged();
    emit volume_listChanged();
    emit start_ms_listChanged();
    emit end_ms_listChanged();
    emit duration_listChanged();
    emit time_before_start_listChanged();

    emit qml_track_listChanged();

    qInfo() << Qt::endl << Qt::endl << Qt::endl;
    qInfo() << "mute list: " << m_mute_list;
    qInfo() << "solo list: " << m_solo_list;
    qInfo() << "volume list: " << m_volume_list;
    qInfo() << "start ms list: " << m_start_ms_list;
    qInfo() << "end ms list: " << m_end_ms_list;
    qInfo() << "duration list: " << m_duration_list;
    qInfo() << "time before start list: " << m_time_before_start_list;
    qInfo() << m_qml_track_list;
    qInfo() << Qt::endl << Qt::endl << Qt::endl;

    // Delete the track object
    track_to_delete->delete_file();
    delete track_to_delete;
    track_object_list.removeAt(index);

    tracks_loaded--;

    if (tracks_loaded != 0) {
        load_tracks();
    } else {
        setmusic_loaded (false);

        m_engine->rootContext()->setContextProperty("audio_player", nullptr);
        delete m_audio_player;

        m_audio_player = new audio_player(this); // Recreate the audio player
        m_engine->rootContext()->setContextProperty("audio_player", m_audio_player);
    }

    for (QVariant solo : m_solo_list) {
        if (solo.toBool()) {
            setprevent_mute(true);
            return;
        }
    }
    setprevent_mute(false); // if a soloed track is deleted remove the prevent mute

}


QVariantList track_manager::qml_track_list() const
{
    return m_qml_track_list;
}

void track_manager::setqml_track_list(const QVariantList &newQml_track_list)
{
    if (m_qml_track_list == newQml_track_list)
        return;
    m_qml_track_list = newQml_track_list;
    emit qml_track_listChanged();
}

QVariantList track_manager::mute_list() const
{
    return m_mute_list;
}

void track_manager::setmute_list(const QVariantList &newMute_list)
{
    if (m_mute_list == newMute_list)
        return;
    m_mute_list = newMute_list;
    emit mute_listChanged();
}

QVariantList track_manager::solo_list() const
{
    return m_solo_list;
}

void track_manager::setmolo_list(const QVariantList &newSolo_list)
{
    if (m_solo_list == newSolo_list)
        return;
    m_solo_list = newSolo_list;
    emit solo_listChanged();
}

QVariantList track_manager::volume_list() const
{
    return m_volume_list;
}

void track_manager::setvolume_list(const QVariantList &newVolume_list)
{
    if (m_volume_list == newVolume_list)
        return;
    m_volume_list = newVolume_list;
    emit volume_listChanged();
}

QVariantList track_manager::start_ms_list() const
{
    return m_start_ms_list;
}

void track_manager::setstart_ms_list(const QVariantList &newStart_ms_list)
{
    if (m_start_ms_list == newStart_ms_list)
        return;
    m_start_ms_list = newStart_ms_list;
    emit start_ms_listChanged();
}

QVariantList track_manager::end_ms_list() const
{
    return m_end_ms_list;
}

void track_manager::setend_ms_list(const QVariantList &newEnd_ms_list)
{
    if (m_end_ms_list == newEnd_ms_list)
        return;
    m_end_ms_list = newEnd_ms_list;
    emit end_ms_listChanged();
}

QVariantList track_manager::duration_list() const
{
    return m_duration_list;
}

void track_manager::setduration_list(const QVariantList &newDuration_list)
{
    if (m_duration_list == newDuration_list)
        return;
    m_duration_list = newDuration_list;
    emit duration_listChanged();
}

QVariantList track_manager::time_before_start_list() const
{
    return m_time_before_start_list;
}

void track_manager::settime_before_start_list(const QVariantList &newTime_before_start_list)
{
    if (m_time_before_start_list == newTime_before_start_list)
        return;
    m_time_before_start_list = newTime_before_start_list;
    emit time_before_start_listChanged();
}

track_manager::playback_states track_manager::get_playback_states()
{
    return m_playback_states;
}

bool track_manager::prevent_mute() const
{
    return m_prevent_mute;
}

void track_manager::setprevent_mute(bool newPrevent_mute)
{
    if (m_prevent_mute == newPrevent_mute)
        return;
    m_prevent_mute = newPrevent_mute;
    emit prevent_muteChanged();
}


QVariantList track_manager::qml_sync_points() const
{
    return m_qml_sync_points;
}

void track_manager::setqml_sync_points(QVariantList &new_qml_sync_points)
{
    if (m_qml_sync_points == new_qml_sync_points)
        return;
    m_qml_sync_points = new_qml_sync_points;
    emit qml_sync_pointsChanged();
}

QVector<main_options::sync_point> track_manager::sync_points() const
{
    return m_sync_points;
}

QVariantList track_manager::convert_sync_points_to_qml(const QVector<main_options::sync_point> &sync_points)
{
    QVariantList new_list;

    for (auto &point : sync_points) {
        new_list.append(QVariant::fromValue(QVariantList{point.time, point.measure, point.beat}));
    }

    qInfo() << "QMLed list: " << new_list;

    return new_list;
}
