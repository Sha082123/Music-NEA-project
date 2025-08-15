#include "xml_parser.h"

xml_parser::xml_parser(QObject *parent)
    : QObject{parent}
{
    notes = QVector<SvgElementInfo>();
    rests = QVector<SvgElementInfo>();
    rehearsal_marks = QVector<SvgElementInfo>();

    all_notes = QVector<QVector<SvgElementInfo>>();
    all_rests = QVector<QVector<SvgElementInfo>>();
    all_rehearsal_marks = QVector<QVector<SvgElementInfo>>();

    all_elements = QVector<QVector<SvgElementInfo>>();

    tab_dur_sym_list = {};

    target_classes = {"note", "rest", "mRest", "reh"};

}

std::string xml_parser::parse_xml(const QString &svg_data)
{
    document.setContent(svg_data);

    notes.clear();
    rests.clear();
    rehearsal_marks.clear();

    QVector<SvgElementInfo> temp_elements;

    //qInfo() << "xml parser called";

    //qInfo() << notes.size() << " : size of notes before parsing";

    QDomElement root = document.documentElement();
    QDomNodeList node_list = root.elementsByTagName("g");

    //qInfo() << "checkpoint";

    for (int index = 0; index < node_list.count(); ++index) {
        QDomElement current_element = node_list.at(index).toElement();
        if (current_element.hasAttribute("class")) {
            QString class_name = current_element.attribute("class");

            if (class_name == "note") {
                SvgElementInfo element = process_note(current_element);
                temp_elements.append(element);
                notes.append(element);
            }


            else if (class_name == "rest") {
                SvgElementInfo element = process_rest(current_element);
                temp_elements.append(element);
                rests.append(element);
            }


            else if (class_name == "mRest") {
                SvgElementInfo element = process_mRest(current_element);
                temp_elements.append(element);
                rests.append(element);
            }


            else if (class_name == "reh") {
                SvgElementInfo element = process_reh(current_element);
                temp_elements.append(element);
                rehearsal_marks.append(element);
            }


            else if (class_name == "tabDurSym") {
                remove_tab_dur_sym (current_element);
                index--; // Decrement index to account for the removed element
            }
        }
    }

    //qInfo() << Qt::endl << Qt::endl << Qt::endl;
    //qInfo() << notes[0].class_name << notes[0].id << notes[0].position << notes[0].width << notes[0].height;

    all_rests.append(rests);
    all_notes.append(notes);
    all_rehearsal_marks.append(rehearsal_marks);

    all_elements.append(temp_elements);

    QString xml_data = document.toString();
    return xml_data.toStdString ();
}

QVariantList xml_parser::element_from_point(const QPointF &point, const int &page_number)
{
    //qInfo() << "Searching for element at point: " << point;
    //qInfo() << "Searching page : " << page_number;

    QVector<SvgElementInfo> current_notes = all_notes[page_number - 1];
    QVector<SvgElementInfo> current_rests = all_rests[page_number - 1];
    QVector<SvgElementInfo> current_rehearsal_marks = all_rehearsal_marks[page_number - 1];

    //qInfo() << all_notes.size();
    //qInfo() << current_notes.size();

    // for (const SvgElementInfo &note : current_notes) {
    //     qInfo() << "Checking note: " << note.id << " at position: " << note.position
    //             << " at measure: " << note.measure_number;
    // }

    // for (const SvgElementInfo &rest : current_rests) {
    //     qInfo() << "Checking rest: " << rest.id << " at position: " << rest.position
    //             << " at measure: " << rest.measure_number;
    // }

    int total_notes_rests;
    for (QVector<SvgElementInfo> &notes_page : all_notes) {
        total_notes_rests += notes_page.size();
    }
    for (QVector<SvgElementInfo> &rests_page : all_rests) {
        total_notes_rests += rests_page.size();
    }

    for (const SvgElementInfo &note : current_notes) {
        if (note.position.x() <= point.x() && note.position.x() + note.width >= point.x() &&
            note.position.y() <= point.y() && note.position.y() + note.height >= point.y()) {

            qInfo() << "Found note id: " << note.id << " at position: " << note.position << Qt::endl
                    << " at measure: " << note.measure_number << " start beat: " << note.start_beat
                    << " end beat: " << note.end_beat << " note name: " << note.note_name;
            return QVariantList{note.id, note.measure_number, note.start_beat, note.end_beat, note.note_name,
                                note.position.x(), note.position.y(), page_number, note.staff_number};
        }
    }

    for (const SvgElementInfo &rest : current_rests) {
        if (rest.position.x() <= point.x() && rest.position.x() + rest.width >= point.x() &&
            rest.position.y() <= point.y() && rest.position.y() + rest.height >= point.y()) {

            qInfo() << "Found rest id: " << rest.id << " at position: " << rest.position << Qt::endl
                    << " at measure: " << rest.measure_number << " start beat: " << rest.start_beat
                    << " end beat: " << rest.end_beat;
            return QVariantList{rest.id, rest.measure_number, rest.start_beat, rest.end_beat, rest.note_name,
                                rest.position.x(), rest.position.y(), page_number, rest.staff_number};
        }
    }

    for (const SvgElementInfo &reh : current_rehearsal_marks) {
        if (reh.position.x() <= point.x() && reh.position.x() + reh.width >= point.x() &&
            reh.position.y() <= point.y() && reh.position.y() + reh.height >= point.y()) {

            qInfo() << "Found rehearsal mark id: " << reh.id << " at position: " << reh.position << Qt::endl
                    << " at measure: " << reh.measure_number;
            return QVariantList{reh.id, reh.measure_number, reh.start_beat, reh.end_beat, reh.note_name,
                                reh.position.x(), reh.position.y(), page_number, reh.staff_number};
        }
    }

    return QVariantList{}; // No element found at the given point
}

QVector<int> xml_parser::coordinates_from_measure(int measure_number)
{
    QVector<int> output;

    SvgElementInfo last_element = all_notes[all_notes.count()-1][all_notes[all_notes.count()-1].count()-1];

    if (measure_number <= 0) {
        qWarning() << "Measure number must be greater than 0.";
        output.append(0);
        output.append(0);
        return output; // Return position of start if less than 1
    }

    else if (measure_number > last_element.measure_number) {
        qWarning() << "Measure number exceeds number of measures in the score";
        output.append(all_notes.count()-1);
        output.append(last_element.position.y());
        return output; // Return position of end if too big
    }

    for (int page_index = 0; page_index < all_elements.count(); page_index++) {
        QVector<SvgElementInfo> &page_elements = all_elements[page_index];

        qInfo() << page_elements[page_elements.count()-1].measure_number << " is the last measure number on page: " << page_index;
        if (page_elements[page_elements.count()-1].measure_number >= measure_number) {

            for (SvgElementInfo &element : page_elements) {
                if (element.measure_number == measure_number) {
                    // qInfo() << "Found note at measure: " << measure_number << " with id: " << note.id;
                    output.append(page_index);
                    output.append(element.position.y());

                    return output;
                }
            }
        }
    }

    output.append(-1);

    return output;
}

void xml_parser::set_info_for_note(QString &id, int &measure_number, int &start_beat, int &end_beat, QString note_name, int staff_number)
{
    bool found = false;
    for (int page_index = 0; page_index < all_notes.size() && !found; ++page_index) {
        QVector<SvgElementInfo> &current_page_notes = all_notes[page_index];
        for (SvgElementInfo &note : current_page_notes) {
            if (note.id == id) {
                found = true;

                note.measure_number = measure_number;
                note.start_beat = start_beat;
                note.end_beat = end_beat;
                note.note_name = note_name;
                note.staff_number = staff_number;

                // qInfo() << "Updated note with id: " << id << " to measure number: " << measure_number
                //         << " start beat: " << start_beat << " end beat: " << end_beat;

                break; // Exit after finding and updating the note
            }
        }
    }

    for (auto &page : all_elements) {
        for (auto &element : page) {
            if (element.id == id) {
                element.measure_number = measure_number;
                element.start_beat = start_beat;
                element.end_beat = end_beat;
                element.note_name = note_name;
                element.staff_number = staff_number;

                // qInfo() << "Updated element with id: " << id << " to measure number: " << measure_number
                //         << " start beat: " << start_beat << " end beat: " << end_beat;

                return; // Exit after finding and updating the note
            }
        }
    }
}

void xml_parser::set_info_for_rest(QString &id, int &measure_number, int &start_beat, int &end_beat, int staff_number)
{
    bool found = false;

    for (int page_index = 0; page_index < all_rests.size() && !found; ++page_index) {
        QVector<SvgElementInfo> &current_page_rests = all_rests[page_index];
        for (SvgElementInfo &rest : current_page_rests) {
            if (rest.id == id) {
                found = true;

                rest.measure_number = measure_number;
                rest.start_beat = start_beat;
                rest.end_beat = end_beat;
                rest.staff_number = staff_number;

                // qInfo() << "Updated rest with id: " << id << " to measure number: " << measure_number
                //         << " start beat: " << start_beat << " end beat: " << end_beat;

                break; // Exit after finding and updating the note
            }
        }
    }

    for (auto &page : all_elements) {
        for (auto &element : page) {
            if (element.id == id) {
                element.measure_number = measure_number;
                element.start_beat = start_beat;
                element.end_beat = end_beat;
                element.staff_number = staff_number;

                // qInfo() << "Updated element with id: " << id << " to measure number: " << measure_number
                //         << " start beat: " << start_beat << " end beat: " << end_beat;

                return; // Exit after finding and updating the note
            }
        }
    }
}

void xml_parser::set_info_for_reh(QString &id, int &measure_number, QString &mark_text)
{
    bool found = false;

    for (int page_index = 0; page_index < all_rehearsal_marks.size() && !found; ++page_index) {
        QVector<SvgElementInfo> &current_page_reh = all_rehearsal_marks[page_index];
        for (SvgElementInfo &reh : current_page_reh) {
            if (reh.id == id) {
                found = true;

                reh.measure_number = measure_number;
                reh.note_name = mark_text; // Using note_name to store the rehearsal mark text
                reh.staff_number = 0;

                // qInfo() << "Updated reh with id: " << id << " to measure number: " << measure_number

                break; // Exit after finding and updating the note
            }
        }
    }

    for (auto &page : all_elements) {
        for (auto &element : page) {
            if (element.id == id) {
                element.measure_number = measure_number;
                element.start_beat = 0;
                element.note_name = mark_text;
                element.staff_number = 0;

                // qInfo() << "Updated element with id: " << id << " to measure number: " << measure_number
                //         << " start beat: " << start_beat << " end beat: " << end_beat;

                return; // Exit after finding and updating the note
            }
        }
    }
}

void xml_parser::set_end_beat_for_note(QString &id, int &end_beat)
{
    for (int page_index = 0; page_index < all_notes.size(); ++page_index) {
        QVector<SvgElementInfo> &current_page_notes = all_notes[page_index];
        for (SvgElementInfo &note : current_page_notes) {
            if (note.id == id) {
                note.end_beat = end_beat;

                // qInfo() << "Updated note with id: " << id << " end beat: " << end_beat;

                return; // Exit after finding and updating the note
            }
        }
    }
}

void xml_parser::set_end_beat_for_rest(QString &id, int &end_beat)
{
    for (int page_index = 0; page_index < all_rests.size(); ++page_index) {
        QVector<SvgElementInfo> &current_page_rests = all_rests[page_index];
        for (SvgElementInfo &rest : current_page_rests) {
            if (rest.id == id) {
                rest.end_beat = end_beat;

                // qInfo() << "Updated rest with id: " << id << " to measure number: " << measure_number
                //         << " start beat: " << start_beat << " end beat: " << end_beat;

                return; // Exit after finding and updating the note
            }
        }
    }
}

QVector<QVector<xml_parser::SvgElementInfo>> xml_parser::get_all_elements()
{
    return all_elements;
}

QVector<QVector<xml_parser::SvgElementInfo> > xml_parser::get_all_rehearsal_marks()
{
    return all_rehearsal_marks;
}

void xml_parser::remove_tab_dur_sym(const QDomElement &element)
{
    QDomNode parent_node = element.parentNode();
    parent_node.removeChild(element);
}

xml_parser::SvgElementInfo xml_parser::process_reh(const QDomElement &element)
{
    SvgElementInfo element_info;

    QString id_name = element.attribute("id");
    QDomNodeList children = element.childNodes();
    for (int index = 0; index < children.count(); ++index) {
        QDomElement current_child = children.at(index).toElement();
        if (current_child.tagName() == "rect") {

            double x = current_child.attribute("x").toDouble() / 10.0;
            double y = current_child.attribute("y").toDouble() / 10.0;
            double width = current_child.attribute("width").toDouble() / 10.0;
            double height = current_child.attribute("height").toDouble() / 10.0;

            element_info.id = id_name;
            element_info.class_name = "Rehearsal mark";
            element_info.position = QPointF(x, y);
            element_info.width = width;
            element_info.height = height;

            return element_info;
        }
        else if (current_child.tagName() == "text") {
            double x = current_child.attribute("x").toDouble() / 10.0;
            double y = current_child.attribute("y").toDouble() / 10.0;

            element_info.id = id_name;
            element_info.class_name = "Rehearsal mark";
            element_info.position = QPointF(x - 60, y - 60);
            element_info.width = 100; //default values
            element_info.height = 60; //default values

            return element_info;
        }
    }

    return element_info;
}

//--------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------//

xml_parser::SvgElementInfo xml_parser::process_mRest(const QDomElement &element)
{
    SvgElementInfo element_info;

    QString id_name = element.attribute("id");
    QDomNodeList children = element.childNodes();
    for (int index = 0; index < children.count(); ++index) {
        QDomElement current_child = children.at(index).toElement();
        if (current_child.tagName() == "use") {
            QString transform_attr = current_child.attribute("transform");

            QRegularExpression re(R"(translate\(\s*(\d+)\s*,\s*(\d+)\s*\))");
            QRegularExpressionMatch match = re.match(transform_attr);

            if (match.hasMatch ()) {
                double x = match.captured(1).toDouble() / 10.0; // in svg the coordinates are multiplied by 10
                double y = match.captured(2).toDouble() / 10.0;
                //qInfo() << "rest coordinates found : " << x << ", " << y;

                element_info.class_name = "rest";
                element_info.id = id_name;
                element_info.position = QPointF(x-50, y-30); //make position more accurate
                element_info.width = 100;
                element_info.height = 60; //default values

                return element_info;

            } else {
                qInfo() << "Could not find match, an error has probably occurred";
                qInfo() << "transform attr : " << transform_attr;
                return element_info;
            }
        }
    }

    return element_info;
}

//--------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------//

xml_parser::SvgElementInfo xml_parser::process_rest(const QDomElement &element)
{
    SvgElementInfo element_info;

    QString id_name = element.attribute("id");
    QDomNodeList children = element.childNodes();
    for (int index = 0; index < children.count(); ++index) {
        QDomElement current_child = children.at(index).toElement();
        if (current_child.tagName() == "use") {

            QString transform_attr = current_child.attribute("transform");

            QRegularExpression re(R"(translate\(\s*(\d+)\s*,\s*(\d+)\s*\))");
            QRegularExpressionMatch match = re.match(transform_attr);

            if (match.hasMatch ()) {
                double x = match.captured(1).toDouble() / 10.0; // in svg the coordinates are multiplied by 10
                double y = match.captured(2).toDouble() / 10.0;
                //qInfo() << "rest coordinates found : " << x << ", " << y;

                element_info.class_name = "rest";
                element_info.id = id_name;
                element_info.position = QPointF(x-5, y-30); //make position more accurate
                element_info.width = 30;
                element_info.height = 60; //default values

                return element_info;

            } else {
                qInfo() << "Could not find match, an error has probably occurred";
                qInfo() << "transform attr : " << transform_attr;
                return element_info;
            }
        }
    }

    return element_info;
}

//--------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------//

xml_parser::SvgElementInfo xml_parser::process_note(const QDomElement &element)
{
    SvgElementInfo element_info;

    QString id_name = element.attribute("id");
    QDomNodeList children = element.childNodes();
    for (int index = 0; index < children.count(); ++index) {
        QDomElement current_child = children.at(index).toElement();
        QString class_name = current_child.attribute("class"); //notehead for note or text for tab
        QString tag_name = current_child.tagName(); //use for notehead or text for tab

        if (class_name == "notehead") {

            QDomNode notehead_child = current_child.firstChild();
            if (notehead_child.toElement().tagName() == "use") {
                QDomElement use_element = notehead_child.toElement();
                // process the element
                QString transform_attr = use_element.attribute("transform");

                QRegularExpression re(R"(translate\(\s*(\d+)\s*,\s*(\d+)\s*\))");
                QRegularExpressionMatch match = re.match(transform_attr);

                if (match.hasMatch ()) {
                    double x = match.captured(1).toDouble() / 10.0; // in svg the cordinates are timed by 10
                    double y = match.captured(2).toDouble() / 10.0;
                    //qInfo() << "coordinates found : " << x << ", " << y;

                    element_info.class_name = class_name;
                    element_info.id = id_name;
                    element_info.position = QPointF(x - 5, y - 10); //make position more accurate
                    element_info.width = 30;
                    element_info.height = 30; //default values

                    return element_info;

                } else {
                    qInfo() << "Could not find match, an error has probably occured";
                    qInfo() << "transform attr : " << transform_attr;
                    return element_info;
                }
            }
        }

        else if (tag_name == "text") {
            QDomElement text_element = current_child.toElement();
            double x = text_element.attribute("x").toDouble() / 10.0;
            double y = text_element.attribute("y").toDouble() / 10.0;
            //qInfo() << "tab coordinates found : " << x << ", " << y;

            element_info.class_name = "tab";
            element_info.id = id_name;
            element_info.position = QPointF(x - 15, y - 30);
            element_info.width = 40;
            element_info.height = 36; //default values

            return element_info;
        }

        else {
            qInfo() << "class name : " << class_name << " is not part of anything";
            return element_info;
        } // if the child is neither "use" nor "text", return empty info
    }

    return element_info;
}
