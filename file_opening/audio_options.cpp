#include "audio_options.h"

audio_options::audio_options(QObject *parent)
    : QObject{parent}
{
    m_file_path = "";
    m_options = new options();
}

audio_options::options audio_options::initialise_file(const QString &file_path)
{

    QString options_path;
    QFileInfo info(file_path);

    options_path = info.absolutePath() + "/options/" + info.completeBaseName() + ".txt";
    m_file_path = options_path;

    QFile file(options_path);
    if (!file.exists()) {
        qInfo() << "options file is being created";
        options default_options;
        write_options_file ();
        return default_options;
    } else {
        qInfo() << "options file is being read";
        return read_options_file();
    }

}

void audio_options::write_options_file()
{
    QFile file(m_file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open options file for writing:" << m_file_path;
        return;
    } else {
        QTextStream out(&file);
        out << "mute=" << m_options->mute << "\n";
        out << "solo=" << m_options->solo << "\n";
        out << "volume=" << m_options->volume << "\n";
        out << "start_ms=" << m_options->start_ms << "\n";
        out << "end_ms=" << m_options->end_ms << "\n";
        out << "duration=" << m_options->duration << "\n";
        out << "time_before_start=" << m_options->time_before_start << "\n";
        file.close();
    }
}



audio_options::options audio_options::read_options_file()
{
    QFile file(m_file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open options file for reading:" << m_file_path;
        return *m_options; // Return default options if file cannot be read
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split('=');
        if (parts.size() == 2) {
            if (parts[0] == "mute") {
                m_options->mute = (static_cast<bool>(parts[1].toInt()));
            } else if (parts[0] == "solo") {
                m_options->solo = (static_cast<bool>(parts[1].toInt()));
            } else if (parts[0] == "volume") {
                m_options->volume = parts[1].toFloat();
            } else if (parts[0] == "start_ms") {
                m_options->start_ms = parts[1].toInt();
            } else if (parts[0] == "end_ms") {
                m_options->end_ms = parts[1].toInt();
            } else if (parts[0] == "duration") {
                m_options->duration = parts[1].toInt();
            } else if (parts[0] == "time_before_start") {
                m_options->time_before_start = parts[1].toInt();
            } else {
                qWarning() << "Unknown option in file:" << parts[0];
            }
        }
    }
    file.close();

    options opt_copy = *m_options;

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Options read from file:" << m_file_path;
    // qInfo() << "Mute:" << opt_copy.mute;
    // qInfo() << "Solo:" << opt_copy.solo;
    // qInfo() << "Volume:" << opt_copy.volume;
    // qInfo() << "Start MS:" << opt_copy.start_ms;
    // qInfo() << "End MS:" << opt_copy.end_ms;
    // qInfo() << "Duration:" << opt_copy.duration;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    return opt_copy;
}
