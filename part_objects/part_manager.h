#ifndef PART_MANAGER_H
#define PART_MANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "part_object.h"

class part_manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList part_name_list READ part_name_list WRITE setpart_name_list NOTIFY part_name_listChanged FINAL)
public:
    explicit part_manager(QObject *parent = nullptr, QQmlApplicationEngine *engine = nullptr);

    QVector<part_object*> get_part_object_list();
    Q_INVOKABLE void create_new_part(QString part_name);
    Q_INVOKABLE void set_current_part(int index);
    Q_INVOKABLE int list_size();
    part_object* get_current_part();

    Q_INVOKABLE void clear_parts();

    QStringList part_name_list() const;
    void setpart_name_list(const QStringList &newPart_name_list);

signals:

    void part_name_listChanged();

private:

    QVector<part_object*> part_object_list;
    part_object* current_part = nullptr;


    QQmlApplicationEngine* m_engine = nullptr; // Pointer to the QML engine, needed for context properties

    QStringList m_part_name_list;
};

#endif // PART_MANAGER_H
