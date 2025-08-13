#ifndef TRACK_MANAGER_H
#define TRACK_MANAGER_H

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QDirIterator>
#include <QFile>
#include "../globals.h"
#include "../file_opening/file_open.h"
#include "track_object.h"
#include "../render/audio_player.h"

class track_manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool music_loaded READ music_loaded WRITE setmusic_loaded NOTIFY music_loadedChanged FINAL)
    Q_PROPERTY(QVariantList qml_track_list READ qml_track_list WRITE setqml_track_list NOTIFY qml_track_listChanged FINAL)

    Q_PROPERTY(QVariantList mute_list READ mute_list WRITE setmute_list NOTIFY mute_listChanged FINAL)
    Q_PROPERTY(QVariantList solo_list READ solo_list WRITE setmolo_list NOTIFY solo_listChanged FINAL)
    Q_PROPERTY(QVariantList volume_list READ volume_list WRITE setvolume_list NOTIFY volume_listChanged FINAL)
    Q_PROPERTY(QVariantList start_ms_list READ start_ms_list WRITE setstart_ms_list NOTIFY start_ms_listChanged FINAL)
    Q_PROPERTY(QVariantList end_ms_list READ end_ms_list WRITE setend_ms_list NOTIFY end_ms_listChanged FINAL)
    Q_PROPERTY(QVariantList duration_list READ duration_list WRITE setduration_list NOTIFY duration_listChanged FINAL)
    Q_PROPERTY(QVariantList time_before_start_list READ time_before_start_list WRITE settime_before_start_list NOTIFY time_before_start_listChanged FINAL)

    Q_PROPERTY(bool prevent_mute READ prevent_mute WRITE setprevent_mute NOTIFY prevent_muteChanged FINAL)

public:
    explicit track_manager(QObject *parent = nullptr, QQmlApplicationEngine *engine = nullptr);

    Q_INVOKABLE void clear();

    Q_INVOKABLE int get_track_count();

    void load_tracks();

    void add_audio_track(QString &file_path);
    void scan_audio_directory(QString &directory); // input is the full path to target directory

    bool music_loaded() const;
    void setmusic_loaded(bool newMusic_loaded);

    void track_loaded();

    int get_total_tracks();
    int get_tracks_loaded();



    Q_INVOKABLE void toggle_mute(int index);
    Q_INVOKABLE void toggle_solo(int index);
    Q_INVOKABLE void set_volume(int index, float volume);

    Q_INVOKABLE void save_playback_states();

    Q_INVOKABLE void open_new_track(QString root_path); // path of main score
    Q_INVOKABLE void delete_track(int index);


    QVariantList qml_track_list() const;
    void setqml_track_list(const QVariantList &newQml_track_list);

    QVariantList mute_list() const;
    void setmute_list(const QVariantList &newMute_list);

    QVariantList solo_list() const;
    void setmolo_list(const QVariantList &newSolo_list);

    QVariantList volume_list() const;
    void setvolume_list(const QVariantList &newVolume_list);

    QVariantList start_ms_list() const;
    void setstart_ms_list(const QVariantList &newStart_ms_list);

    QVariantList end_ms_list() const;
    void setend_ms_list(const QVariantList &newEnd_ms_list);

    QVariantList duration_list() const;
    void setduration_list(const QVariantList &newDuration_list);

    QVariantList time_before_start_list() const;
    void settime_before_start_list(const QVariantList &newTime_before_start_list);

    struct playback_states {
        QVector<bool> mute_list;
        QVector<bool> solo_list;
        QVector<float> volume_list;
        QVector<int> start_ms_list;
        QVector<int> end_ms_list;
        QVector<int> duration_list;
        QVector<int> time_before_start_list;
    };


    playback_states get_playback_states();

    bool prevent_mute() const;
    void setprevent_mute(bool newPrevent_mute);

signals:

    void music_loadedChanged();

    void qml_track_listChanged();

    void mute_listChanged();

    void solo_listChanged();

    void volume_listChanged();

    void start_ms_listChanged();

    void end_ms_listChanged();

    void duration_listChanged();

    void time_before_start_listChanged();

    void prevent_muteChanged();

private:
    QQmlApplicationEngine *m_engine; // Pointer to the QML engine
    QVector<track_object*> track_object_list; // List of track objects

    file_open* m_file_open = nullptr;
    audio_player* m_audio_player = nullptr;
    bool m_music_loaded;

    int tracks_loaded;
    QVariantList m_qml_track_list;



    playback_states m_playback_states; // store playback states to save the options of the tracks

    QVariantList m_mute_list;
    QVariantList m_solo_list;
    QVariantList m_volume_list;
    QVariantList m_start_ms_list;
    QVariantList m_end_ms_list;
    QVariantList m_duration_list;
    QVariantList m_time_before_start_list;
    bool m_prevent_mute;
};

#endif // TRACK_MANAGER_H
