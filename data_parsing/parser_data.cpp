#include "parser_data.h"
#include "globals.h"

parser_data::parser_data(QObject *parent)
    : QObject{parent}
{}

void parser_data::parse()
{
    parse_reh_list();
    parse_break_list ();
}



QVariantList parser_data::reh_y_coords() const
{
    return m_reh_y_coords;
}

void parser_data::setReh_y_coords(const QVariantList &newReh_y_coords)
{
    if (m_reh_y_coords == newReh_y_coords)
        return;
    m_reh_y_coords = newReh_y_coords;

    // qInfo() << "Rehearsal marks Y coordinates updated:" << m_reh_y_coords;

    emit reh_y_coordsChanged();
}

void parser_data::parse_reh_list()
{
    QVector<QVector<xml_parser::SvgElementInfo>> all_reh_list = g_xml_parser->get_all_rehearsal_marks();
    QVariantList temp_reh_y_coords;

    for (int index = 0; index < all_reh_list.size(); ++index) {
        QVector<xml_parser::SvgElementInfo> current_page_reh = all_reh_list[index];

        int page_index = index; // 1 less than page number, to get combined heights of all pages before
        //double cumulative_page_height = g_resvg_loader->get_page_height(page_number);

        for (const xml_parser::SvgElementInfo &reh : current_page_reh) {
            double y_coords = reh.position.y();
            QString reh_name = reh.note_name;
            QString reh_id = reh.id;

            temp_reh_y_coords << QVariant::fromValue(QVariantList{reh_id, reh_name, y_coords, page_index});
        }
    }

    setReh_y_coords (temp_reh_y_coords); // Emit the signal to notify that the data has changed
    qInfo() << m_reh_y_coords.size() << "rehearsal marks parsed and Y coordinates updated.";
}

void parser_data::parse_break_list()
{
    QVector<mei_parser::break_element> all_breaks = g_mei_parser->get_breaks();

    QVariantList temp_break_list;

    for (int index = 0; index < all_breaks.size(); ++index) {
        mei_parser::break_element current_break = all_breaks[index];

        int measure_number = current_break.measure_number;
        QString break_id = current_break.id;

        temp_break_list << QVariant::fromValue(QVariantList{break_id, measure_number});
    }

    setBreak_list(temp_break_list); // Emit the signal to notify that the data has changed
}

void parser_data::break_list_sort()
{
    //sort the break list

    std::sort(m_break_list.begin(), m_break_list.end(), [](const QVariant &a, const QVariant &b) {
        return a.toList()[1].toInt() < b.toList()[1].toInt();
    });

    // Emit the signal to notify that the data has changed
    emit break_listChanged();
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

void parser_data::setBreak_list(const QVariantList &newBreak_list)
{
    if (m_break_list == newBreak_list)
        return;
    m_break_list = newBreak_list;
    emit break_listChanged();
}

bool parser_data::update_break_list(QString id, QString input)
{
    if (input.toInt() <= 0) { // 0 also if not numeric
        return false;
    }

    else if (input.toInt() > number_of_measures) {
        return false;
    }

    for (int index = 0; index < m_break_list.size(); ++index) {
        QVariantList break_item = m_break_list[index].toList();
        if (break_item[0].toString() == id) {
            // Update the measure number
            break_item[1] = input.toInt();
            m_break_list[index] = QVariant::fromValue(break_item);
            break_list_sort();
            emit break_listChanged();

            //qInfo() << m_break_list;

            return true; // No error
        }
    }

    // if the break is not found, then add a new break

    m_break_list << QVariant::fromValue(QVariantList{id, input.toInt()});

    break_list_sort();

    return true;


}

bool parser_data::delete_break_item(QString id)
{

    for (int index = 0; index < m_break_list.size(); ++index) {
        QVariantList break_item = m_break_list[index].toList();
        if (break_item[0].toString() == id) {
            // Update the measure number
            m_break_list.removeAt(index);
            emit break_listChanged();

            //qInfo() << m_break_list;

            return true; // No error
        }
    }

    return false;

}
