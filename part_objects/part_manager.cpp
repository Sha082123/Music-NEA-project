#include "part_manager.h"
#include "../track_objects/track_manager.h"

part_manager::part_manager(QObject *parent, QQmlApplicationEngine *engine, track_manager *track_manager)
    : QObject{parent}
{

    m_engine = engine;
    m_track_manager = track_manager;
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

    part_object *root_part = new part_object(this, part_name, this);
    part_object_list.append(root_part);

    m_engine->rootContext()->setContextProperty("root_part", root_part);

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

    for (int index = 0; index < part_object_list[0]->part_list().size(); ++index) {
        new_part_element.parts_active.append(qMakePair(
            part_object_list[0]->part_list().at(index).toList().at(2).toInt(), // should be the part n_value
            true)); // Initialize all parts as active
    }

    part_action_list.append(new_part_element);

    return 0;
}

void part_manager::create_sync_coordinates()
{
    QVector<main_options::sync_point> sync_points = m_track_manager->sync_points();

    for (int index = 0; index < part_object_list.size(); ++index) {
        if (index == 0) {
            part_object_list.at(index)->calculate_sync_coordinates (sync_points);
        } else {
            part_object_list.at(index)->match_sync_to_root();
        }

    }

    set_tracker_time(0);

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

int part_manager::update_part(int index, QString new_name, QString old_name)
{
    if (new_name.isEmpty() || new_name.contains(".") || new_name.contains("/") || new_name.contains("\\")) {
        return -1; // Invalid part name
    }

    old_name = m_part_name_list[index]; // Get the old name from the list

    if (old_name == new_name) {
        return -2; // Part name already exists
    }

    // if (index < 0 || index >= m_part_name_list.size()) {
    //     return -3; // Invalid index
    // }

    // part_element update_part_element;
    // update_part_element.mode = 2; // 2 for update
    // update_part_element.name = new_name;
    // update_part_element.index = index;
    // part_action_list.append(update_part_element);


    for (int index = 0; index < part_object_list.size(); ++index) {
        qInfo() << part_object_list[index]->get_part_name() << " to " << new_name << "  " << old_name;
        if (part_object_list[index]->get_part_name() == old_name) {

            part_object_list[index]->update_part_name(new_name); // Update the part name in the part object
        }
    }



    for (int index = 0; index < part_action_list.count(); ++index) {
        if (part_action_list[index].name == old_name) {
            part_action_list[index].name = new_name; // Update the name in the action list
        }
    }



    m_part_name_list[index] = new_name;
    emit part_name_listChanged();

    // if already in buffer name list, then update. if not, then dont update

    for (int index = 0; index < m_buffer_part_name_list.size(); ++index) {
        if (m_buffer_part_name_list[index] == old_name) {
            setbuffer_part_name_list(m_part_name_list);
            break; // Exit loop after updating the first occurrence
        }
    }



    return 0;
}

int part_manager::update_part_staves(QVariantList part_existence, int index)
{
    if (index < 0 || index >= m_part_name_list.size()) {
        return -3; // index out of range
    }

    bool at_least_one_active = false;

    part_element update_part_staves_element;

    for (QVariant &part : part_existence) {

        QPair<int, bool> pair = qMakePair(part.toList().at(0).toInt(),part.toList().at(1).toBool());
        if (pair.second == true) {
            at_least_one_active = true; // at least one part is active
        }

        update_part_staves_element.parts_active.append(pair);
    }

    if (at_least_one_active == false) {
        qInfo() << "at least one part must be active";
        return -1; // invalid input
    }

    update_part_staves_element.mode = 3; // 3 for update staff
    update_part_staves_element.name = m_part_name_list[index];
    update_part_staves_element.index = index;
    part_action_list.append(update_part_staves_element);

    return 0; // success :O

}

void part_manager::apply_part_actions()
{

    for (const part_element &part_action : part_action_list) {
        if (part_action.mode == 0) { // Delete
            if (part_action.index < part_object_list.size()) {

                part_object_list.at(part_action.index)->delete_file();

                part_object_list.removeAt(part_action.index);

                set_current_part (0); // Set the current part to the main score after deletion
            }
            else {
                qInfo() << "could not delete item at index: " << part_action.index
                        << " because it is out of range of the part_object_list size: " << part_object_list.size();
            }
        } else if (part_action.mode == 1) { // New
            part_object *new_part = new part_object(this, part_action.name, this);
            part_object_list.append(new_part);

            QString part_file_path = m_file_open->createNewPart(part_object_list[0]->file_path(),
                                                                m_file_open->get_current_dir() + "/UserFiles/dump/" +
                                                                    m_file_open->name_from_project_files(part_object_list[0]->file_path())
                                                                    + "/", part_action.name);
            new_part->openFile(part_file_path, 1); // Open the file for the new part

        // } else if (part_action.mode == 2) { // Update
        //     if (part_action.index < part_object_list.size()) {
        //         part_object_list.at(part_action.index)->update_part_name(part_action.name);
        //     }
        } else if (part_action.mode == 3) { // Update Staff
            if (part_action.index < part_object_list.size()) {
                QVector<QPair<int, bool>> parts_active = part_action.parts_active;
                part_object_list.at(part_action.index)->update_part_staves(parts_active,
                                                                           part_object_list.at(0));
                part_object_list.at(part_action.index)->update();
                part_object_list.at(part_action.index)->setsaved("(not saved)");
            }
        } else {
            qInfo() << "Unknown action mode: " << part_action.mode;
        }
    }
    part_action_list.clear(); // Clear the action list after applying actions
    setbuffer_part_name_list(m_part_name_list);

    save_all();
    create_sync_coordinates ();
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

bool part_manager::staff_exists_in_part(int staff_n_value, QString part_name)
{
    //QVariantList part_list = part_object_list[part_index]->part_list();

    // look through the action list first

    for (int index = part_action_list.size() - 1; index >= 0; --index) {
        if (part_action_list[index].mode == 1 || part_action_list[index].mode == 3) { // 1 for new, 3 for update staff

            for (const QPair<int, bool> &part : part_action_list[index].parts_active) {
                if (part.first == staff_n_value) {

                    qInfo() << Qt::endl << Qt::endl << Qt::endl;
                    qInfo() << "part not yet initialised route taken" << part.first << part.second;
                    qInfo() << Qt::endl << Qt::endl << Qt::endl;
                    return part.second; // Staff ID exists in the part action list
                }
            }
        }
    }

    // if not found in action list, then access the part directly (part is existing)



    for (part_object *part : part_object_list) {
        if (part->get_part_name() == part_name) { // part exists
            QVariantList part_list = part->part_list();

            for (const QVariant &part_item : part_list) {
                if (part_item.toList().at(2).toInt() == staff_n_value) {
                    return true; // Staff n_value exists in the part
                }
            }

            return false; // Staff n_value does not exist in the part
        }
    }

    qInfo() << "part with name " << part_name << " not found in part_object_list";

    return false; // if all else fails :(


    // for (const QVariant &part_item : part_list) {
    //     if (part_item.toList().at(0).toString() == staff_id) {
    //         return true; // Staff ID exists in the part
    //     }
    // }

    // return false; // Staff ID does not exist in the part
}

void part_manager::scan_part_directory(QString source_path)
{

    QString target_dir = QDir::currentPath() + "/UserFiles/dump/" + m_file_open->name_from_project_files(source_path);

    QDirIterator file(target_dir, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); //add immediate files and immediate directories
    while (file.hasNext()) {
        QString next_file = file.next();
        QFileInfo next_info(next_file);
        if (next_info.isFile()) {
            part_object* new_part_object = new part_object(this, next_info.completeBaseName (), this);
            new_part_object->openFile(next_file, 1); // Open the file for the new part

            part_object_list.append(new_part_object);

            m_part_name_list.append(next_info.completeBaseName());
        }
    }

    // QString audio_directory = target_dir + "/tracks";
    // if (QDir(audio_directory).exists()) {
    //     qInfo() << "Scanning audio directory: " << audio_directory;
    //     m_track_manager->scan_audio_directory(audio_directory); // Scan the directory for audio tracks
    // }

    emit part_name_listChanged ();

    setbuffer_part_name_list(m_part_name_list);
}

void part_manager::save_all()
{
    for (int index = 0; index < part_object_list.size(); ++index) {
        part_object_list.at(index)->save_file();
    }

    create_sync_coordinates ();
}

void part_manager::set_unsaved()
{
    for (int index = 0; index < part_object_list.size(); ++index) {
        part_object_list.at(index)->set_unsaved();
    }
}


part_object* part_manager::get_current_part()
{
    return current_part;
}

part_object *part_manager::get_root_part()
{
    return part_object_list.at(0);
}

void part_manager::clear_parts()
{
    part_object_list.clear();
    m_part_name_list.clear();
    emit part_name_listChanged();
}

void part_manager::set_tracker_time(int time)
{
    //qInfo() << part_object_list.size();
    for (int index = 0; index < part_object_list.size(); ++index) {
        //qInfo() << "hello";
        part_object_list.at(index)->set_coordinates_from_time(time);
    }
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
