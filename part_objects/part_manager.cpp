#include "part_manager.h"

part_manager::part_manager(QObject *parent, QQmlApplicationEngine *engine)
    : QObject{parent}
{
    m_engine = engine;
}

QVector<part_object*> part_manager::get_part_object_list()
{
    return part_object_list;
}

void part_manager::create_new_part(QString part_name)
{
    part_object *new_part;
    new_part = new part_object(this, part_name);
    part_object_list.append(new_part);
    m_part_name_list.append(part_name);
    emit part_name_listChanged();
}

void part_manager::set_current_part(int index)
{
    current_part = part_object_list.at(index);
    m_engine->rootContext()->setContextProperty("current_part", current_part);
    emit part_name_listChanged();
}

int part_manager::list_size()
{
    return m_part_name_list.size();
}

part_object* part_manager::get_current_part()
{
    return current_part;
}

void part_manager::clear_parts()
{
    part_object_list.clear();
    m_part_name_list.clear();
    emit part_name_listChanged();
}

QStringList part_manager::part_name_list() const
{
    return m_part_name_list;
}

void part_manager::setpart_name_list(const QStringList &newPart_name_list)
{
    if (m_part_name_list == newPart_name_list)
        return;
    m_part_name_list = newPart_name_list;
    emit part_name_listChanged();
}
