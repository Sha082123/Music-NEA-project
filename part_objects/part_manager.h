#ifndef PART_MANAGER_H
#define PART_MANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "part_object.h"
#include "../file_opening/file_open.h"

class part_manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList part_name_list READ part_name_list WRITE setpart_name_list NOTIFY part_name_listChanged FINAL)
    Q_PROPERTY(QStringList buffer_part_name_list READ buffer_part_name_list WRITE setbuffer_part_name_list NOTIFY buffer_part_name_listChanged FINAL)
public:
    explicit part_manager(QObject *parent = nullptr, QQmlApplicationEngine *engine = nullptr);

    QVector<part_object*> get_part_object_list();

    Q_INVOKABLE void create_root_part(QString part_name);

    Q_INVOKABLE int create_new_part(QString part_name); // 0 = success, -1 = invalid input, -2 = exists already
    Q_INVOKABLE int delete_part(int index); // 0 = success, -1 = index out of range
    Q_INVOKABLE int update_part(int index, QString new_name); // 0 = success, -1 = invalid input,
                                                              // -2 = name already exists,
                                                              // -3 = index out of range
    Q_INVOKABLE void apply_part_actions();

    Q_INVOKABLE void set_current_part(int index);
    //Q_INVOKABLE void set_current_part(QString part_name); // Sets the current part by name
    Q_INVOKABLE int list_size();

    part_object* get_current_part();

    Q_INVOKABLE void clear_parts();

    QStringList part_name_list() const;
    void setpart_name_list(const QStringList &newPart_name_list);


    QStringList buffer_part_name_list() const;
    void setbuffer_part_name_list(const QStringList &newBuffer_part_name_list);

signals:

    void part_name_listChanged();


    void buffer_part_name_listChanged();

private:

    struct part_element {
        int mode; // 0 for delete, 1 for new, 2 for update
        QString name;
        int index;
    };

    QVector<part_element> part_action_list;

    QVector<part_object*> part_object_list;
    part_object* current_part = nullptr;


    QQmlApplicationEngine* m_engine = nullptr; // Pointer to the QML engine, needed for context properties
    file_open *m_file_open;

    QStringList m_part_name_list;


    //int get_index_for_part_name(QString &part_name);
    int m_part_object_list_size;
    QStringList m_buffer_part_name_list;
};

#endif // PART_MANAGER_H
