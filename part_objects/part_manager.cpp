#include "part_manager.h"

part_manager::part_manager(QObject *parent, QQmlApplicationEngine *engine)
    : QObject{parent}
{
    m_engine = engine;
    m_file_open = new file_open(nullptr, g_verovio_loader);
}

QVector<part_object*> part_manager::get_part_object_list()
{
    return part_object_list;
}

void part_manager::create_root_part(QString part_name)
{
    m_part_name_list.append(part_name);
    emit part_name_listChanged();

    setbuffer_part_name_list(m_part_name_list);

    part_object *root_part = new part_object(m_engine, part_name);
    part_object_list.append(root_part);

}

int part_manager::create_new_part(QString part_name)
{
    if (part_name.isEmpty() || part_name.contains(".") || part_name.contains("/") || part_name.contains("\\")) {
        return -1; // Invalid part name
    }

    for (const QString &existing_name : m_part_name_list) {
        if (existing_name == part_name) {
            return -2; // Part name already exists
        }
    }

    m_part_name_list.append(part_name);
    emit part_name_listChanged();

    part_element new_part_element;
    new_part_element.mode = 1; // 1 for new
    new_part_element.name = part_name;
    new_part_element.index = part_object_list.size() - 1; // Index of the newly created part (last)

    part_action_list.append(new_part_element);

    return 0;
}

int part_manager::delete_part(int index)
{
    if (index < 1 || index >= m_part_name_list.size()) { // cant delete the root main_score
        return -1; // Invalid index
    }

    part_element delete_part_element;
    delete_part_element.mode = 0; // 0 for delete
    delete_part_element.name = m_part_name_list[index];
    delete_part_element.index = index;

    part_action_list.append(delete_part_element);

    m_part_name_list.removeAt(index);

    emit part_name_listChanged();

    return 0;
}

int part_manager::update_part(int index, QString new_name)
{
    if (new_name.isEmpty() || new_name.contains(".") || new_name.contains("/") || new_name.contains("\\")) {
        return -1; // Invalid part name
    }

    for (const QString &existing_name : m_part_name_list) {
        if (existing_name == new_name) {
            return -2; // Part name already exists
        }
    }

    if (index < 0 || index >= m_part_name_list.size()) {
        return -3; // Invalid index
    }

    part_element update_part_element;
    update_part_element.mode = 2; // 2 for update
    update_part_element.name = new_name;
    update_part_element.index = index;
    part_action_list.append(update_part_element);

    m_part_name_list[index] = new_name;
    emit part_name_listChanged();

    return 0;
}

void part_manager::apply_part_actions()
{

    for (const part_element &part_action : part_action_list) {
        if (part_action.mode == 0) { // Delete
            if (part_action.index < part_object_list.size()) {

                part_object_list.at(part_action.index)->delete_file();

                part_object_list.removeAt(part_action.index);
            }
            else {
                qInfo() << "could not delete item at index: " << part_action.index
                        << " because it is out of range of the part_object_list size: " << part_object_list.size();
            }
        } else if (part_action.mode == 1) { // New
            part_object *new_part = new part_object(m_engine, part_action.name);
            part_object_list.append(new_part);

            QString part_file_path = m_file_open->createNewPart(part_object_list[0]->file_path(),
                                                                m_file_open->get_current_dir() + "/UserFiles/dump/" +
                                                                    m_file_open->name_from_project_files(part_object_list[0]->file_path())
                                                                    + "/", part_action.name);
            //qInfo() << "checkpoint part_file_path: " << part_file_path;
            new_part->openFile(part_file_path, 1); // Open the file for the new part
            //qInfo() << "checkpoint after openFile";
            set_current_part (get_part_object_list().size() - 1); // Set the newly created part as current
            //qInfo() << "checkpoint after set_current_part";
        } else if (part_action.mode == 2) { // Update
            if (part_action.index < part_object_list.size()) {
                part_object_list.at(part_action.index)->update_part_name(part_action.name);
            }
        }
    }
    part_action_list.clear(); // Clear the action list after applying actions
    setbuffer_part_name_list(m_part_name_list);
}


void part_manager::set_current_part(int index)
{
    current_part = part_object_list.at(index);
    m_engine->rootContext()->setContextProperty("current_part", current_part);
    emit part_name_listChanged();
}

// void part_manager::set_current_part(QString part_name)
// {
//     int index = get_index_for_part_name(part_name);
//     if (index != -1) {
//         current_part = part_object_list.at(index);
//         m_engine->rootContext()->setContextProperty("current_part", current_part);
//         emit part_name_listChanged();
//         return;
//     }
//     qInfo() << "failed to find part with name: " << part_name;
// }

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

// int part_manager::get_index_for_part_name(QString &part_name)
// {
//     for (int index = 0; index < m_part_name_list.size(); ++index) {
//         if (m_part_name_list[index] == part_name) {
//             return index;
//         }
//     }
//     return -1; // Not found
// }



QStringList part_manager::buffer_part_name_list() const
{
    return m_buffer_part_name_list;
}

void part_manager::setbuffer_part_name_list(const QStringList &newBuffer_part_name_list)
{
    if (m_buffer_part_name_list == newBuffer_part_name_list)
        return;
    m_buffer_part_name_list = newBuffer_part_name_list;
    emit buffer_part_name_listChanged();
}
