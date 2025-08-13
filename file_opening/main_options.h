#ifndef MAIN_OPTIONS_H
#define MAIN_OPTIONS_H

#include <QObject>
#include <QDir>
#include <QFile>

class main_options : public QObject
{
    Q_OBJECT
public:
    explicit main_options(QObject *parent = nullptr);



    struct sync_point {
        int time = 0;
        int measure = 1;
        float beat = 1.0f;
    };



    QVector<sync_point> initialise_options(QString &file_path);
    void update_options(QVector<sync_point> &new_sync_points);

signals:



private:
    QVector<sync_point> m_sync_points; // milliseconds : point in score (e.g. 1000: 1.1) (1000 to bar 1 beat 1)
    QString m_file_path;

    void write_options();
    QVector<sync_point> read_options();

};

#endif // MAIN_OPTIONS_H
