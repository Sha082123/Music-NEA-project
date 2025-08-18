#include "part_object.h"
#include <QDebug>
#include "part_manager.h"

part_object::part_object(QObject *parent, QString part_name, part_manager *part_manager)
    : QObject{parent}
{
    //m_image_provider = new image_provider(); // Initialize the global image provider
    m_verovio_loader = new verovio_loader(this); // Initialize the global Verovio loader
    m_resvg_loader = new resvg_loader(this); // Initialize the global Resvg loader
    m_xml_parser = new xml_parser(this); // Initialize the global XML parser
    m_mei_parser = new mei_parser(this, m_xml_parser, nullptr); // Initialize the global MEI parser
    m_parser_data = new parser_data(this, m_xml_parser, m_mei_parser); // Initialize the global parser data

    m_mei_parser->set_parser_data(m_parser_data); // set the parser data for use

    m_part_manager = part_manager; // Set the part manager

    //qInfo() << "checkpoint1";

    m_render_file = new render_file(this, m_mei_parser, m_xml_parser,
                                    m_parser_data, m_verovio_loader, m_resvg_loader);

    //qInfo() << "checkpoint2";


    m_part_name = part_name;

    m_saved = "(saved)";

    m_sync_coordinates = {};

    settracker_info(QVariantList());
}


void part_object::openFile(const QString &file_path, int mode)
{
    setfile_path(file_path);

    m_resvg_loader->clear_page_heights(); // Clear the page heights before opening a new file

    m_render_file->openFile(file_path, mode); // Open the file using the render file object
    setlist_PNG_paths (m_render_file->list_PNG_paths ());

    setReh_y_coords (m_parser_data->reh_y_coords ());
    setBreak_list (m_parser_data->break_list ());
    setpart_list(m_parser_data->part_list());

    // for (const QString &path : m_list_PNG_paths) {
    //     qInfo() << "PNG Path: " << path; // Debugging output to check the paths
    // }
}

void part_object::update()
{
    m_resvg_loader->clear_page_heights(); // Clear the page heights before opening a new file
    m_render_file->update(); // Update the render file object
    setlist_PNG_paths (m_render_file->list_PNG_paths ());

    setReh_y_coords (m_parser_data->reh_y_coords ());
    setBreak_list (m_parser_data->break_list ());
    setpart_list(m_parser_data->part_list());

    // for (const QString &path : m_list_PNG_paths) {
    //     qInfo() << "PNG Path: " << path; // Debugging output to check the paths
    // }

}

int part_object::new_break_item(QString input)
{
    int output = m_parser_data->new_break_item (input);
    setBreak_list (m_parser_data->break_list ());
    return output;
}

int part_object::delete_break_item(int measure_number)
{
    int output = m_parser_data->delete_break_item(measure_number);
    setBreak_list (m_parser_data->break_list ());
    return output;
}

int part_object::update_break_list(QString id, QString input)
{
    int output = m_parser_data->update_break_list (id, input);
    setBreak_list (m_parser_data->break_list ());
    return output;
}

void part_object::apply_breaks()
{
    m_parser_data->apply_breaks ();
    setBreak_list (m_parser_data->break_list ());
    setsaved("(not saved)");

    update();
    m_part_manager->create_sync_coordinates();
}

QVariantList part_object::element_from_point(const QPointF &point, const int &page_number)
{
    return m_xml_parser->element_from_point (point, page_number);
}

QVariantList part_object::coordinates_from_measure(int measure_number)
{
    QVector<int> result = m_xml_parser->coordinates_from_measure(measure_number);
    QVariantList output;

    for (int integer : result) {
        output.append(integer);
    }

    qInfo() << "Coordinates for measure" << measure_number << ":" << output;

    return output;
}

int part_object::time_from_measure(int measure_number)
{
    if (measure_number < 1) {
        return 0; // start
    }

    if (measure_number > m_sync_coordinates.last().measure) {
        for (auto point : m_sync_coordinates) {
            qInfo() << point.measure;
        }
        qInfo() << m_sync_coordinates.last().measure << "is the last measure, returning end time";
        return m_sync_coordinates.last().time; // end
    }

    for (auto &element : m_sync_coordinates) {
        if (element.measure == measure_number) {
            return element.time;
        }
    }

    return -1; // not found
}

void part_object::update_part_name(QString new_part_name)
{
    m_part_name = new_part_name;

    QFile file(m_file_path);
    QFileInfo info(m_file_path);

    QString new_path = info.path() + "/" + new_part_name + "." + info.suffix();
    QFile::rename(m_file_path, new_path);
    qInfo() << "File renamed from" << m_file_path << "to" << new_path;

    setfile_path(new_path); // Update the file path after renaming
}

void part_object::update_part_staves(QVector<QPair<int, bool> > &part_existence, part_object *root_ptr)
{
    m_mei_parser->update_part_staves (part_existence, root_ptr);
}

void part_object::delete_file()
{
    QFile::remove(m_file_path);
}

void part_object::set_unsaved()
{
    setsaved("(not saved)");
}

void part_object::save_file()
{
    QString data = m_mei_parser->export_mei_data();

    QFile file(m_file_path);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
        qInfo() << "File saved successfully at" << m_file_path;

        setsaved("(saved)");
    } else {
        qWarning() << "Failed to save file at" << m_file_path;
    }
}

void part_object::calculate_sync_coordinates(QVector<main_options::sync_point> &sync_points)
{
    m_sync_coordinates.clear();

    QVector<QVector<xml_parser::SvgElementInfo>> all_elements = m_xml_parser->get_all_elements();

    // sort the list to ensure elements are in order //

    for (auto &page : all_elements) {
        std::sort(page.begin(), page.end(), [](const xml_parser::SvgElementInfo &a, const xml_parser::SvgElementInfo &b) {
            return a.measure_number < b.measure_number || (a.measure_number == b.measure_number && a.start_beat < b.start_beat);
        });
    }

    // calculate the y coordinates for each line, by finding the average y position of each element in the line

    QVector<mei_parser::break_element> all_breaks = m_mei_parser->get_breaks();

    QVector<QPair<float, float>> y_list; // top y coordinates, height of bar
    QVector<float> y_list_no_offset;
    int current_break_index = 0;
    int current_break_measure = all_breaks[current_break_index].measure_number;
    QVector<float> sum_list;

    // for (auto break_obj : all_breaks) {
    //     qInfo() << "Break ID: " << break_obj.id
    //             << ", Measure Number: " << break_obj.measure_number;
    // }

    // for (auto page : all_elements) {
    //     for (auto element : page) {
    //         qInfo() << "Element ID: " << element.id
    //                 << ", Measure Number: " << element.measure_number
    //                 << ", Start Beat: " << element.start_beat
    //                 << ", Position Y: " << element.position.y();
    //     }
    // }

    for (int page_index = 0; page_index < all_elements.size(); page_index++) {

        int page_offset = m_resvg_loader->get_page_height(page_index);

        for (int element_index = 0; element_index < all_elements[page_index].size(); element_index ++) {

            auto element = all_elements[page_index][element_index];
            xml_parser::SvgElementInfo next_element;

            if (element_index == all_elements[page_index].size() - 1) {
                if (page_index < all_elements.size() - 1) {
                    next_element = all_elements[page_index + 1][0];
                } else {
                    next_element = xml_parser::SvgElementInfo(); // empty element if last element last page
                }
            } else {
                next_element = all_elements[page_index][element_index + 1];
            }

            if (element.staff_number != 0) {
                sum_list.append(element.position.y());
            }




            if ((next_element.measure_number > current_break_measure) || (element.id == all_elements.last().last().id)) {
                // save the sum and get next break measure number, or if last then save the sum
                // y_list.append(sum/count);

                float max = std::ranges::max(sum_list);
                float min = std::ranges::min(sum_list);

                //qInfo() << max << min;

                y_list.append(qMakePair (min + page_offset, max - min));
                y_list_no_offset.append(min);

                sum_list.clear();

                if (current_break_index < all_breaks.size() - 1) {
                    current_break_index++;
                    current_break_measure = all_breaks[current_break_index].measure_number;
                } else {
                    current_break_measure = std::numeric_limits<int>::max(); // set to max if no more breaks
                }
            }
        }
    }

    // qInfo() << "list of y coordinates: " << y_list;
    // qInfo() << "Number of y coordinates: " << y_list.size();

    //----------------------------------------------------------------------------------//

    QVector<mei_parser::tempo_element> tempo_elements = m_mei_parser->get_tempo_changes();
    QVector<mei_parser::time_signature_element> time_signature_elements = m_mei_parser->get_time_signature_changes();

    int current_tempo = 100; // Default tempo
    int current_time_signature = 128; // Default time signature (128 = 4 x 32 = 4 crotchets)
    int tempo_element_index = 0;
    int time_signature_element_index = 0;
    mei_parser::tempo_element current_tempo_element = tempo_elements[tempo_element_index];
    mei_parser::time_signature_element current_time_signature_element = time_signature_elements[time_signature_element_index];

    int current_page_index = 0;
    int current_element_index = 0;

    int previous_element_measure = 0;
    int previous_element_start_beat = 0;

    current_break_index = 0;
    current_break_measure = all_breaks[current_break_index].measure_number;

    float ms_per_beat = 0;
    float ms_per_measure = 0;

    int time_difference = 0;
    int time_to_add = 0;

    xml_parser::SvgElementInfo current_element;
    xml_parser::SvgElementInfo previous_element;

    previous_element.measure_number = 1;

    for (int index = 0; index < sync_points.size(); ++index) {

        if (index >= sync_points.size() - 1) { // check if at end
            //extrapolate according to tempo and time signatures

            // qInfo() << "current page index: " << current_page_index;
            // qInfo() << "current element index: " << current_element_index;
            // qInfo() << "current measure: " << all_elements[current_page_index][current_element_index].measure_number;
            // qInfo() << "current beat: " << all_elements[current_page_index][current_element_index].start_beat;


            for (int page_index = current_page_index; page_index < all_elements.size(); page_index++) {

                for (int element_index = 0; element_index < all_elements[page_index].size(); element_index++) {
                    current_element = all_elements[page_index][element_index];

                    // qInfo() << "current element: " << current_element.start_beat << current_element.measure_number;
                    // qInfo() << "previous element: " << previous_element_start_beat << previous_element_measure;

                    if ((current_element.measure_number > previous_element_measure) ||
                        ((current_element.start_beat > previous_element_start_beat))  && (current_element.measure_number >= previous_element_measure) ) {
                        // Skip elements that are at the same beat as the last element

                        // qInfo() << "Processing element at measure:" << current_element.measure_number
                        //         << "start beat:" << current_element.start_beat;
                        // qInfo() << "Previous element at measure:" << previous_element_measure
                        //         << "start beat:" << previous_element_start_beat;


                        sync_coordinate new_sync_coordinate;

                        current_tempo = current_tempo_element.tempo;
                        current_time_signature = (128 / current_time_signature_element.denominator) * current_time_signature_element.numerator;

                        if (tempo_element_index < tempo_elements.size() - 1) {
                            if (current_element.measure_number == tempo_elements[tempo_element_index + 1].measure_start) {
                                tempo_element_index ++;
                                current_tempo_element = tempo_elements[tempo_element_index];
                                // qInfo() << "Tempo element index: " << tempo_element_index;
                                // qInfo() << "current tempo: " << current_tempo_element.tempo;
                            }
                        }

                        if (time_signature_element_index < time_signature_elements.size() - 1) {
                            if (current_element.measure_number == time_signature_elements[time_signature_element_index + 1].measure_start) {
                                time_signature_element_index ++;
                                current_time_signature_element = time_signature_elements[time_signature_element_index];
                                // qInfo() << "Time signature element index: " << time_signature_element_index;
                                // qInfo() << "current time signature: " << current_time_signature_element.numerator << "/" << current_time_signature_element.denominator;
                            }
                        }


                        int measure_difference = current_element.measure_number - previous_element.measure_number;
                        float beat_difference = float(current_element.start_beat + 32)/float(32.0f) - float(previous_element.start_beat + 32)/float(32.0f);

                        // qInfo() << "current measure: " << current_element.measure_number;
                        // qInfo() << "previous measure: " << previous_element.measure_number;
                        // qInfo() << Qt::endl;

                        ms_per_beat = 60000.0f / float(current_tempo); // milliseconds per beat
                        ms_per_measure = float(ms_per_beat) * float(current_time_signature)/32.0f; // milliseconds per measure

                        time_to_add = (measure_difference * ms_per_measure) + (beat_difference * ms_per_beat);
                        time_difference += time_to_add;

                        if (current_break_index < all_breaks.size()) { // no -1 because y coordinates can be more than breaks
                            if (current_element.measure_number > current_break_measure) { // add extra break coordinates
                                sync_coordinate new_break_coordinate;
                                // qInfo() << "time difference: " << time_difference;
                                new_break_coordinate.time = time_difference - (0.6 * time_to_add) ;  // leave some time to transition to next Y
                                // qInfo() << "Adding break coordinate at time: " << new_break_coordinate.time;
                                new_break_coordinate.coordinates.setX(previous_element.position.x() + 50);
                                new_break_coordinate.coordinates.setY(y_list[current_break_index].first);
                                new_break_coordinate.height = y_list[current_break_index].second;
                                new_break_coordinate.page_index = page_index;
                                new_break_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                                new_break_coordinate.measure = previous_element.measure_number;
                                new_break_coordinate.beat = 0; // 0 so that when matching to other parts it will always be included
                                m_sync_coordinates.append(new_break_coordinate);

                                current_break_index ++;
                                current_break_measure = all_breaks[current_break_index].measure_number;
                                // qInfo() << "all breaks: " << all_breaks.size();
                                // qInfo() << "Current break index: " << current_break_index;

                            }
                        }







                        new_sync_coordinate.time = time_difference;
                        new_sync_coordinate.coordinates.setX(current_element.position.x());
                        new_sync_coordinate.coordinates.setY(y_list[current_break_index].first);
                        new_sync_coordinate.height = y_list[current_break_index].second;
                        new_sync_coordinate.page_index = page_index;
                        new_sync_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                        new_sync_coordinate.measure = current_element.measure_number;
                        new_sync_coordinate.beat = float(current_element.start_beat + 32) / 32.0f;

                        if ((current_element.position.x() < previous_element.position.x()) && (new_sync_coordinate.coordinates.y() == m_sync_coordinates[m_sync_coordinates.size() - 1].coordinates.y())) {
                            m_sync_coordinates[m_sync_coordinates.size() - 1].coordinates.setX(current_element.position.x() - 20);
                        }
                        m_sync_coordinates.append(new_sync_coordinate);

                        previous_element_measure = current_element.measure_number;
                        previous_element_start_beat = current_element.start_beat;
                        previous_element = current_element;

                        // if ((page_index == all_elements.size() - 1) && (element_index == all_elements[page_index].size() - 1)) {
                        //     // If we are at the last element of the last page, add a final sync coordinate


                        //     // qInfo() << "final element processed";

                        // }
                    }
                }
            }

            sync_coordinate final_coordinate;
            int extra_time = ms_per_beat * (float(current_element.end_beat - current_element.start_beat)/32.0f);

            final_coordinate.time = time_difference + extra_time;
            final_coordinate.coordinates.setX(current_element.position.x() + 50);
            final_coordinate.coordinates.setY(y_list.last().first);
            final_coordinate.height = y_list.last().second;
            final_coordinate.page_index = all_elements.size() - 1;
            final_coordinate.y_no_offset = y_list_no_offset.last();
            final_coordinate.measure = current_element.measure_number;
            final_coordinate.beat = 0; // 0 so that when matching to other parts it will always be included
            m_sync_coordinates.append(final_coordinate);
        }

        else {

            // append normal times not justified to sync point first, then iterate through and times by a scale factor

            main_options::sync_point current_sync_point = sync_points[index];
            main_options::sync_point next_sync_point = sync_points[index + 1];

            int end_measure = next_sync_point.measure;
            int end_beat = next_sync_point.beat;

            bool end = false;
            bool stationary = false;

            if (end_measure == current_sync_point.measure && end_beat == current_sync_point.beat) {
                sync_coordinate new_sync_coordinate;
                new_sync_coordinate.time = next_sync_point.time;
                new_sync_coordinate.coordinates.setX(all_elements[current_page_index][current_element_index].position.x());
                new_sync_coordinate.coordinates.setY(y_list[current_break_index].first);
                new_sync_coordinate.height = y_list[current_break_index].second;
                new_sync_coordinate.page_index = current_page_index;
                new_sync_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                new_sync_coordinate.measure = current_sync_point.measure;
                new_sync_coordinate.beat = current_sync_point.beat;
                m_sync_coordinates.append(new_sync_coordinate);

                end = true;
                stationary = true;
            }

            int start_sync_index = m_sync_coordinates.size();
            float scale_factor;
            int sync_time_difference = next_sync_point.time - current_sync_point.time;

            int time_from_sync = 0;


            for (int page_index = current_page_index; page_index < all_elements.size() && end == false; page_index++) {

                for (int element_index = 0; element_index < all_elements[page_index].size(); element_index++) {
                    current_element = all_elements[page_index][element_index];

                    // qInfo() << "current element: " << current_element.start_beat << current_element.measure_number;
                    // qInfo() << "previous element: " << previous_element_start_beat << previous_element_measure;

                    if ((current_element.measure_number > previous_element_measure) ||
                        ((current_element.start_beat > previous_element_start_beat))  && (current_element.measure_number >= previous_element_measure) ) {
                        // Skip elements that are at the same beat as the last element

                        current_element_index = element_index;
                        current_page_index = page_index;

                        //qInfo() << element_index;



                        float current_beat = float(current_element.start_beat + 32) / 32.0f;

                        // qInfo() << "Current data: " << current_beat << current_element.measure_number;
                        // qInfo() << "End data: " << end_beat << end_measure;





                        sync_coordinate new_sync_coordinate;

                        current_tempo = current_tempo_element.tempo;
                        current_time_signature = (128 / current_time_signature_element.denominator) * current_time_signature_element.numerator;



                        if (tempo_element_index < tempo_elements.size() - 1) {
                            if (current_element.measure_number == tempo_elements[tempo_element_index + 1].measure_start) {
                                tempo_element_index ++;
                                current_tempo_element = tempo_elements[tempo_element_index];
                                // qInfo() << "Tempo element index: " << tempo_element_index;
                                // qInfo() << "current tempo: " << current_tempo_element.tempo;
                            }
                        }

                        if (time_signature_element_index < time_signature_elements.size() - 1) {
                            if (current_element.measure_number == time_signature_elements[time_signature_element_index + 1].measure_start) {
                                time_signature_element_index ++;
                                current_time_signature_element = time_signature_elements[time_signature_element_index];
                                // qInfo() << "Time signature element index: " << time_signature_element_index;
                                // qInfo() << "current time signature: " << current_time_signature_element.numerator << "/" << current_time_signature_element.denominator;
                            }
                        }

                        int measure_difference = current_element.measure_number - previous_element.measure_number;
                        float beat_difference = float(current_element.start_beat + 32)/float(32.0f) - float(previous_element.start_beat + 32)/float(32.0f);

                        ms_per_beat = 60000.0f / float(current_tempo); // milliseconds per beat
                        ms_per_measure = float(ms_per_beat) * float(current_time_signature)/32.0f; // milliseconds per measure

                        time_to_add = (measure_difference * ms_per_measure) + (beat_difference * ms_per_beat);
                        time_from_sync += time_to_add;

                        if ((current_beat >= end_beat) && (current_element.measure_number >= end_measure)) {
                            // if we reach the end of the sync point, then we can stop
                            end = true;
                            previous_element = current_element;
                            previous_element_measure = current_element.measure_number;
                            previous_element_start_beat = current_element.start_beat;

                            break;
                        }

                        if (current_break_index < all_breaks.size()) { // no -1 because y coordinates can be more than breaks
                            if (current_element.measure_number > current_break_measure) { // add extra break coordinates
                                sync_coordinate new_break_coordinate;
                                // qInfo() << "time difference: " << time_difference;
                                new_break_coordinate.time = (time_from_sync) - (0.6 * time_to_add) ;  // leave some time to transition to next Y
                                // qInfo() << "Adding break coordinate at time: " << new_break_coordinate.time;
                                new_break_coordinate.coordinates.setX(previous_element.position.x() + 50);
                                new_break_coordinate.coordinates.setY(y_list[current_break_index].first);
                                new_break_coordinate.height = y_list[current_break_index].second;
                                new_break_coordinate.page_index = page_index;
                                new_break_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                                new_break_coordinate.measure = previous_element.measure_number;
                                new_break_coordinate.beat = 0; // 0 so that when matching to root this coordinate is always included
                                m_sync_coordinates.append(new_break_coordinate);

                                // qInfo() << "Adding break coordinate at time: " << new_break_coordinate.time;
                                // qInfo() << "Break measure: " << current_break_measure;

                                current_break_index ++;
                                current_break_measure = all_breaks[current_break_index].measure_number;
                                // qInfo() << "all breaks: " << all_breaks.size();
                                // qInfo() << "Current break index: " << current_break_index;

                            }
                        }


                        //qInfo() << time_from_sync << time_difference << time_to_add;

                        new_sync_coordinate.time = time_from_sync;
                        new_sync_coordinate.coordinates.setX(current_element.position.x());
                        new_sync_coordinate.coordinates.setY(y_list[current_break_index].first);
                        new_sync_coordinate.height = y_list[current_break_index].second;
                        new_sync_coordinate.page_index = page_index;
                        new_sync_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                        new_sync_coordinate.measure = current_element.measure_number;
                        new_sync_coordinate.beat = float(current_element.start_beat + 32) / 32.0f;

                        if ((current_element.position.x() < previous_element.position.x()) && (new_sync_coordinate.coordinates.y() == m_sync_coordinates[m_sync_coordinates.size() - 1].coordinates.y())) {
                            m_sync_coordinates[m_sync_coordinates.size() - 1].coordinates.setX(current_element.position.x() - 20);
                        }
                        m_sync_coordinates.append(new_sync_coordinate);

                        previous_element_measure = current_element.measure_number;
                        previous_element_start_beat = current_element.start_beat;
                        previous_element = current_element;

                        // xml_parser::SvgElementInfo next_element;
                        // if (element_index == all_elements[page_index].size() - 1) {
                        //     if (page_index < all_elements.size() - 1) {
                        //         next_element = all_elements[page_index + 1][0];
                        //     } else {
                        //         next_element = xml_parser::SvgElementInfo(); // empty element if last element last page
                        //     }
                        // } else {
                        //     next_element = all_elements[page_index][element_index + 1];
                        // }





                    }
                }
            }

            if (stationary == false) {
                scale_factor = float(sync_time_difference) / float(time_from_sync);
                qInfo() << sync_time_difference << " / " << time_from_sync;

                for (int i = start_sync_index; i < m_sync_coordinates.size(); ++i) {
                    m_sync_coordinates[i].time = int(m_sync_coordinates[i].time * scale_factor) + time_difference;
                }

                time_difference += time_from_sync * scale_factor;

                //qInfo() << time_difference << " : " << next_sync_point.time;

                sync_coordinate new_sync_coordinate;

                new_sync_coordinate.time = time_difference;
                new_sync_coordinate.coordinates.setX(current_element.position.x());
                new_sync_coordinate.coordinates.setY(y_list[current_break_index].first);
                new_sync_coordinate.height = y_list[current_break_index].second;
                new_sync_coordinate.page_index = current_page_index;
                new_sync_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                new_sync_coordinate.measure = current_element.measure_number;
                new_sync_coordinate.beat = float(current_element.start_beat + 32) / 32.0f;

                m_sync_coordinates.append(new_sync_coordinate);
            } else {
                time_difference = next_sync_point.time;
            }




        }
    }

    for (auto sync_coordinate : m_sync_coordinates) {
        qInfo() << "Sync Coordinate: " << sync_coordinate.coordinates << ", Time: " << sync_coordinate.time;
        if (sync_coordinate.measure== 20) {
                return;
            }
    }
}

void part_object::match_sync_to_root()
{
    m_sync_coordinates.clear();

    QVector<QVector<xml_parser::SvgElementInfo>> all_elements = m_xml_parser->get_all_elements();

    // sort the list to ensure elements are in order //

    for (auto &page : all_elements) {
        std::sort(page.begin(), page.end(), [](const xml_parser::SvgElementInfo &a, const xml_parser::SvgElementInfo &b) {
            return a.measure_number < b.measure_number || (a.measure_number == b.measure_number && a.start_beat < b.start_beat);
        });
    }

    // calculate the y coordinates for each line, by finding the average y position of each element in the line

    QVector<mei_parser::break_element> all_breaks = m_mei_parser->get_breaks();

    QVector<QPair<float, float>> y_list; // top y coordinates, height of bar
    QVector<float> y_list_no_offset;
    int current_break_index = 0;
    int current_break_measure = all_breaks[current_break_index].measure_number;
    QVector<float> sum_list;

    // for (auto break_obj : all_breaks) {
    //     qInfo() << "Break ID: " << break_obj.id
    //             << ", Measure Number: " << break_obj.measure_number;
    // }

    // for (auto page : all_elements) {
    //     for (auto element : page) {
    //         qInfo() << "Element ID: " << element.id
    //                 << ", Measure Number: " << element.measure_number
    //                 << ", Start Beat: " << element.start_beat
    //                 << ", Position Y: " << element.position.y();
    //     }
    // }

    for (int page_index = 0; page_index < all_elements.size(); page_index++) {

        int page_offset = m_resvg_loader->get_page_height(page_index);

        for (int element_index = 0; element_index < all_elements[page_index].size(); element_index ++) {

            auto element = all_elements[page_index][element_index];
            xml_parser::SvgElementInfo next_element;

            if (element_index == all_elements[page_index].size() - 1) {
                if (page_index < all_elements.size() - 1) {
                    next_element = all_elements[page_index + 1][0];
                } else {
                    next_element = xml_parser::SvgElementInfo(); // empty element if last element last page
                }
            } else {
                next_element = all_elements[page_index][element_index + 1];
            }

            if (element.staff_number != 0) {
                sum_list.append(element.position.y());
            }




            if ((next_element.measure_number > current_break_measure) || (element.id == all_elements.last().last().id)) {
                // save the sum and get next break measure number, or if last then save the sum
                // y_list.append(sum/count);

                float max = std::ranges::max(sum_list);
                float min = std::ranges::min(sum_list);

                //qInfo() << max << min;

                y_list.append(qMakePair (min + page_offset, max - min));
                y_list_no_offset.append(min);

                sum_list.clear();

                if (current_break_index < all_breaks.size() - 1) {
                    current_break_index++;
                    current_break_measure = all_breaks[current_break_index].measure_number;
                } else {
                    current_break_measure = std::numeric_limits<int>::max(); // set to max if no more breaks
                }
            }
        }
    }

    // qInfo() << "list of y coordinates: " << y_list;
    // qInfo() << "Number of y coordinates: " << y_list.size();

    //----------------------------------------------------------------------------------//

    current_break_index = 0;
    current_break_measure = all_breaks[current_break_index].measure_number;

    xml_parser::SvgElementInfo current_element;
    xml_parser::SvgElementInfo previous_element;

    int previous_element_measure = 0;
    int previous_element_start_beat = 0;


    for (int page_index = 0; page_index < all_elements.size(); page_index++) {

        for (int element_index = 0; element_index < all_elements[page_index].size(); element_index++) {

            current_element = all_elements[page_index][element_index];
            if ((current_element.measure_number > previous_element_measure) ||
                ((current_element.start_beat > previous_element_start_beat))  && (current_element.measure_number >= previous_element_measure) ) {

                if (current_break_index < all_breaks.size()) { // no -1 because y coordinates can be more than breaks
                    if (current_element.measure_number > current_break_measure) { // add extra break coordinates
                        sync_coordinate new_break_coordinate;

                        new_break_coordinate.coordinates.setX(previous_element.position.x() + 50);
                        new_break_coordinate.coordinates.setY(y_list[current_break_index].first);
                        new_break_coordinate.height = y_list[current_break_index].second;
                        new_break_coordinate.page_index = page_index;
                        new_break_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                        new_break_coordinate.measure = previous_element.measure_number;
                        new_break_coordinate.beat = 0; // 0 so that when matching to other parts it will always be included
                        m_sync_coordinates.append(new_break_coordinate);

                        current_break_index ++;
                        current_break_measure = all_breaks[current_break_index].measure_number;
                        // qInfo() << "all breaks: " << all_breaks.size();
                        // qInfo() << "Current break index: " << current_break_index;

                    }
                }

                sync_coordinate new_sync_coordinate;

                new_sync_coordinate.coordinates.setX(current_element.position.x());
                new_sync_coordinate.coordinates.setY(y_list[current_break_index].first);
                new_sync_coordinate.height = y_list[current_break_index].second;
                new_sync_coordinate.page_index = page_index;
                new_sync_coordinate.y_no_offset = y_list_no_offset[current_break_index];
                new_sync_coordinate.measure = current_element.measure_number;
                new_sync_coordinate.beat = float(current_element.start_beat + 32) / 32.0f;

                if ((current_element.position.x() < previous_element.position.x()) && (new_sync_coordinate.coordinates.y() == m_sync_coordinates[m_sync_coordinates.size() - 1].coordinates.y())) {
                    m_sync_coordinates[m_sync_coordinates.size() - 1].coordinates.setX(current_element.position.x() - 20);
                }
                m_sync_coordinates.append(new_sync_coordinate);

                previous_element_measure = current_element.measure_number;
                previous_element_start_beat = current_element.start_beat;
                previous_element = current_element;



            }
        }
    }

    sync_coordinate final_coordinate;

    final_coordinate.coordinates.setX(current_element.position.x() + 50);
    final_coordinate.coordinates.setY(y_list.last().first);
    final_coordinate.height = y_list.last().second;
    final_coordinate.page_index = all_elements.size() - 1;
    final_coordinate.y_no_offset = y_list_no_offset.last();
    final_coordinate.measure = current_element.measure_number;
    final_coordinate.beat = 0; // 0 so that when matching to other parts it will always be included
    m_sync_coordinates.append(final_coordinate);

    QVector<sync_coordinate> root_sync_coordinate = m_part_manager->get_root_part()->get_sync_coordinates();

    int sync_index = 0;
    int measure_to_find = m_sync_coordinates[sync_index].measure;
    float beat_to_find = m_sync_coordinates[sync_index].beat;

    for (auto &sync_coordinate : root_sync_coordinate) {

        if (sync_coordinate.measure == measure_to_find && sync_coordinate.beat == beat_to_find) {
            // if we find the same measure and beat, then we can use the time
            m_sync_coordinates[sync_index].time = sync_coordinate.time;
            sync_index++;

            if (sync_index < m_sync_coordinates.size()) {
                measure_to_find = m_sync_coordinates[sync_index].measure;
                beat_to_find = m_sync_coordinates[sync_index].beat;
            } else {
                break; // no more coordinates to match
            }
        }

    }

}

void part_object::set_coordinates_from_time(int time)
{
    //qInfo() << m_sync_coordinates.size();


    QVariantList tracker_info;
    for (int index = 0; index < m_sync_coordinates.size(); ++index) {

        sync_coordinate previous_element;

        if (index != 0) {
            previous_element = m_sync_coordinates[index - 1];
        } else {
            previous_element = m_sync_coordinates[index];
        }

        sync_coordinate current_element = m_sync_coordinates[index];

        if (m_sync_coordinates[index].time == time) {
            if (index == 0) {
                // if we are at the first element, return it
                qInfo() << "checkpoint";
                tracker_info = QVariantList({current_element.coordinates, current_element.height,
                                             current_element.page_index, current_element.y_no_offset, 0.0f});
                settracker_info (tracker_info);
                return;
            }
            tracker_info = QVariantList({current_element.coordinates, current_element.height,
                                         current_element.page_index, current_element.y_no_offset,
                                         previous_element.coordinates.y()});
            settracker_info (tracker_info);
            return;

        } else if (m_sync_coordinates[index].time > time) {
            if (index > 0) {

                sync_coordinate more_previous_element = m_sync_coordinates[index - 2];

                float x_coordinate = previous_element.coordinates.x() +
                                    (float(time - previous_element.time) / float(current_element.time - previous_element.time))
                                    * (current_element.coordinates.x() - previous_element.coordinates.x());
                float y_coordinate = previous_element.coordinates.y() +
                                    (float(time - previous_element.time) / float(current_element.time - previous_element.time))
                                    * (current_element.coordinates.y() - previous_element.coordinates.y());

                QPointF coordinates = previous_element.coordinates;
                coordinates.setX(x_coordinate);
                coordinates.setY(y_coordinate);


                tracker_info = QVariantList({coordinates, previous_element.height,
                                             previous_element.page_index, previous_element.y_no_offset,
                                             more_previous_element.coordinates.y()});
                settracker_info (tracker_info);
                return;
            } else {
                tracker_info = QVariantList({current_element.coordinates, current_element.height,
                                             current_element.page_index, current_element.y_no_offset,
                                             0.0f});
                settracker_info (tracker_info);
                return;
            }

        } else if (index == m_sync_coordinates.size() - 1) {
            // if we reach the end of the list, then return the last element
            tracker_info = QVariantList({m_sync_coordinates[index].coordinates, m_sync_coordinates[index].height,
                                         m_sync_coordinates[index].page_index, m_sync_coordinates[index].y_no_offset});
            settracker_info (tracker_info);
            return;
        }

    }
}

QVector<part_object::sync_coordinate> part_object::get_sync_coordinates()
{
    return m_sync_coordinates;
}



QString part_object::get_part_name()
{
    return m_part_name;
}

QDomDocument part_object::get_document() const
{
    return m_mei_parser->get_document();
}



QStringList part_object::list_PNG_paths() const
{
    return m_list_PNG_paths;
}

void part_object::setlist_PNG_paths(const QStringList &newList_PNG_paths)
{
    if (m_list_PNG_paths == newList_PNG_paths)
        return;
    m_list_PNG_paths = newList_PNG_paths;
    emit list_PNG_pathsChanged();
}

parser_data *part_object::get_parser_data() const
{
    return m_parser_data;
}

QVariantList part_object::reh_y_coords() const
{
    return m_reh_y_coords;
}

void part_object::setReh_y_coords(const QVariantList &newReh_y_coords)
{
    if (m_reh_y_coords == newReh_y_coords)
        return;
    m_reh_y_coords = newReh_y_coords;
    emit reh_y_coordsChanged();
}

QVariantList part_object::break_list() const
{
    return m_break_list;
}

void part_object::setBreak_list(const QVariantList &newBreak_list)
{
    if (m_break_list == newBreak_list)
        return;
    m_break_list = newBreak_list;
    emit break_listChanged();
}

QString part_object::file_path() const
{
    return m_file_path;
}

void part_object::setfile_path(const QString &newFile_path)
{
    if (m_file_path == newFile_path)
        return;
    m_file_path = newFile_path;
    emit file_pathChanged();
}

QVariantList part_object::part_list() const
{
    return m_part_list;
}

void part_object::setpart_list(const QVariantList &newPart_list)
{
    if (m_part_list == newPart_list)
        return;
    m_part_list = newPart_list;
    emit part_listChanged();
}


QString part_object::saved() const
{
    return m_saved;
}

void part_object::setsaved(const QString &newSaved)
{
    if (m_saved == newSaved)
        return;
    m_saved = newSaved;
    emit savedChanged();
}

QVariantList part_object::tracker_info() const
{
    return m_tracker_info;
}

void part_object::settracker_info(const QVariantList &newTracker_info)
{
    //qInfo() << "Setting tracker info: " << newTracker_info;
    if (m_tracker_info == newTracker_info)
        return;
    m_tracker_info = newTracker_info;
    emit tracker_infoChanged();
}
