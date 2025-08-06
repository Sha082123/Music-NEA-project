#include "parser_data.h"
#include "globals.h"
#include "mei_parser.h"

parser_data::parser_data(QObject *parent, xml_parser *xml_parser, mei_parser *mei_parser)
    : QObject{parent}
{
    m_xml_parser = xml_parser;
    m_mei_parser = mei_parser;

}

void parser_data::parse()
{
    parse_reh_list();
    parse_break_list();
    parse_part_list();
}



QVariantList parser_data::reh_y_coords() const
{
    return m_reh_y_coords;
}


void parser_data::parse_reh_list()
{
    QVector<QVector<xml_parser::SvgElementInfo>> all_reh_list = m_xml_parser->get_all_rehearsal_marks();
    QVariantList temp_reh_y_coords;

    for (int index = 0; index < all_reh_list.size(); ++index) {
        QVector<xml_parser::SvgElementInfo> current_page_reh = all_reh_list[index];

        int page_index = index; // 1 less than page number, to get combined heights of all pages before
        //double cumulative_page_height = m_resvg_loader->get_page_height(page_number);

        for (const xml_parser::SvgElementInfo &reh : current_page_reh) {
            double y_coords = reh.position.y();
            QString reh_name = reh.note_name;
            QString reh_id = reh.id;

            temp_reh_y_coords << QVariant::fromValue(QVariantList{reh_id, reh_name, y_coords, page_index});
        }
    }

    m_reh_y_coords = temp_reh_y_coords; // Emit the signal to notify that the data has changed
}

void parser_data::parse_break_list()
{
    QVector<mei_parser::break_element> all_breaks = m_mei_parser->get_breaks();

    QVariantList temp_break_list;

    for (int index = 0; index < all_breaks.size(); ++index) {
        mei_parser::break_element current_break = all_breaks[index];

        int measure_number = current_break.measure_number;
        QString break_id = current_break.id;

        temp_break_list << QVariant::fromValue(QVariantList{break_id, measure_number});
    }

    m_break_list = temp_break_list;
}

void parser_data::parse_part_list()
{

    QVector<mei_parser::part_element> all_parts = m_mei_parser->get_parts();
    QVariantList temp_part_list;

    for (const mei_parser::part_element &part : all_parts) {
        QString part_id = part.id;
        QString part_name = part.part_name;
        int n_value = part.n_value;

        temp_part_list << QVariant::fromValue(QVariantList{part_id, part_name, n_value});
    }

    m_part_list = temp_part_list;
}

void parser_data::break_list_sort()
{
    //sort the break list

    std::sort(m_break_list.begin(), m_break_list.end(), [](const QVariant &a, const QVariant &b) {
        return a.toList()[1].toInt() < b.toList()[1].toInt();
    });

    // Emit the signal to notify that the data has changed
}

int parser_data::getNumber_of_measures() const
{
    return number_of_measures;
}

void parser_data::setNumber_of_measures(const int &newNumber_of_measures)
{
    number_of_measures = newNumber_of_measures;
}


QVariantList parser_data::break_list() const
{
    return m_break_list;
}

QVariantList parser_data::part_list() const
{
    return m_part_list;
}


int parser_data::update_break_list(QString id, QString input)
{
    if (input.toInt() <= 0) { // 0 also if not numeric
        return -1; // -1  for invalid input
    }

    else if (input.toInt() > number_of_measures) {
        return -2; // -2  for out of range
    }

    for (const QVariant &break_item : m_break_list) {
        if (break_item.toList()[1] == input.toInt()) {
            return -3; // Break with this measure number already exists
        }
    }

    for (int index = 0; index < m_break_list.size(); ++index) {
        QVariantList break_item = m_break_list[index].toList();
        if (break_item[0].toString() == id) {

            // add break element info for delete (!!BEFORE UPDATING THE ITEM!!)
            break_element break_struct;
            break_struct.id = break_item[0].toString();
            break_struct.measure_number = break_item[1].toInt();
            break_struct.mode = 0; // 0 for delete

            break_action_list.append(break_struct);

            qInfo() << "Break item added to be deleted: " << break_struct.id
                    << " with measure number: " << break_struct.measure_number << "mode: " << break_struct.mode;

            // Update the measure number
            break_item[1] = input.toInt();
            m_break_list[index] = QVariant::fromValue(break_item);
            break_list_sort();

            //qInfo() << m_break_list;

            // add break element info for new
            break_struct.measure_number = break_item[1].toInt();
            break_struct.mode = 1; // 1 for new
            break_action_list.append(break_struct);

            qInfo() << "Break item added to be edited: " << break_struct.id
                    << " with measure number: " << break_struct.measure_number << "mode: " << break_struct.mode;

            return 0; // No error
        }
    }

    return -3;

    // if the break is not found, then add a new break

    // m_break_list << QVariant::fromValue(QVariantList{id, input.toInt()});

    // break_list_sort();

    // return true;


}

int parser_data::delete_break_item(int measure_number)
{

    for (int index = 0; index < m_break_list.size(); ++index) {
        QVariantList break_item = m_break_list[index].toList();
        if (break_item[1].toInt() == measure_number) {

            // add break element info for delete (!!BEFORE REMOVING THE ITEM!!)
            break_element break_struct;
            break_struct.id = break_item[0].toString();
            break_struct.measure_number = break_item[1].toInt();
            break_struct.mode = 0; // 0 for delete

            break_action_list.append(break_struct);

            qInfo() << "Break item added to be deleted: " << break_struct.id
                    << " with measure number: " << break_struct.measure_number << "mode: " << break_struct.mode;

            // Update the measure number
            m_break_list.removeAt(index);
            //qInfo() << m_break_list;

            return 0; // No error
        }
    }

    return -1; // -1 for not found

}

int parser_data::new_break_item(QString input)
{
    if (input.toInt() <= 0) { // 0 also if not numeric
        return -1; // invalid input
    }

    else if (input.toInt() > number_of_measures) {
        return -2; // out of range
    }

    for (const QVariant &break_item : m_break_list) {
        if (break_item.toList()[1] == input.toInt()) {
            return -3; // Break with this measure number already exists
        }
    }

    QString id = "new_break_" + input; // Generate a new ID for the break

    m_break_list << QVariant::fromValue(QVariantList{id, input.toInt()});

    break_list_sort();

    break_element break_struct;
    break_struct.id = id;
    break_struct.measure_number = input.toInt();
    break_struct.mode = 1; // 1 for new

    break_action_list.append(break_struct);

    qInfo() << "Break item added: " << break_struct.id
            << " with measure number: " << break_struct.measure_number << "mode: " << break_struct.mode;

    return 0; // no error
}

void parser_data::apply_breaks()
{
    for (const break_element &break_action : break_action_list) {
        if (break_action.mode == 0) { // delete
            m_mei_parser->delete_break(break_action.measure_number);

            qInfo() << Qt::endl << Qt::endl << Qt::endl;
            qInfo() << "Break deleted from measure " << break_action.measure_number;
        }
        else if (break_action.mode == 1) { // new
            m_mei_parser->insert_break(break_action.id, break_action.measure_number);

            qInfo() << Qt::endl << Qt::endl << Qt::endl;
            qInfo() << "Break inserted with ID: " << break_action.id
                    << " at measure " << break_action.measure_number;
            qInfo() << Qt::endl << Qt::endl << Qt::endl;
        }
    }

    break_action_list.clear(); // Clear the action list after applying the breaks
}
