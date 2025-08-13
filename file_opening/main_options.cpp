#include "main_options.h"

main_options::main_options(QObject *parent)
    : QObject{parent}
{
    m_sync_points = {sync_point()};
    m_file_path = "";
}

void main_options::update_options(QVector<sync_point> &new_sync_points)
{
    m_sync_points = new_sync_points;
    write_options();
}

QVector<main_options::sync_point> main_options::initialise_options(QString &file_path)
{
    QFile options_file(file_path);

    m_file_path = file_path;

    if (!options_file.exists()) {
        qInfo() << "Options file does not exist, creating new one at:" << m_file_path;
        m_sync_points = {sync_point()};
        write_options();
        return m_sync_points;
    } else {
        qInfo() << "Options file exists, reading from:" << m_file_path;
        return read_options ();
    }
}

void main_options::write_options()
{
    qInfo() << "writing main options file";
    qInfo() << m_file_path;
    QFile file(m_file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open options file for writing:" << m_file_path;
        return;
    } else {

        QTextStream out(&file);

        for (const auto &point : m_sync_points) {
            out << point.time << "=" << point.measure << ":" << point.beat << "\n";
        }

        file.close();
    }
}

QVector<main_options::sync_point> main_options::read_options()
{
    QFile file(m_file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open options file for reading:" << m_file_path;
        return m_sync_points; // Return default options if file cannot be read
    }

    m_sync_points.clear(); // Clear existing sync points before reading

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qInfo() << "Reading line from options file:" << line;
        QStringList parts = line.split('=');
        if (parts.size() == 2) {
            QStringList sync_parts = parts[1].split(':');
            if (sync_parts.size() == 2) {
                main_options::sync_point point;
                point.time = parts[0].toInt();
                point.measure = sync_parts[0].toInt();
                point.beat = sync_parts[1].toFloat();
                m_sync_points.append(point);

            }
        }
    }
    file.close();

    return m_sync_points;
}
