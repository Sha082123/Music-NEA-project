#ifndef PARSER_DATA_H
#define PARSER_DATA_H

#include <QObject>
#include <QVariant>
#include <QDebug>
class mei_parser; // forward declaration to resolve circular dependency
#include "xml_parser.h"

class parser_data : public QObject
{
    Q_OBJECT
public:
    explicit parser_data(QObject *parent = nullptr, xml_parser *xml_parser = nullptr, mei_parser *mei_parser = nullptr);

    void parse();
    int update_break_list(QString id, QString input); // 0 = no error, -1 = invalid input,
                                                                  // -2 = out of range, -3 = already exists

    int delete_break_item(int measure_number); // 0 = no error, -1 = could not find break id

    int new_break_item(QString input);  // 0 = no error, -1 = invalid input,
                                                    // -2 = out of range, -3 = already exists

    void apply_breaks(); // Applies the break action list

    int getNumber_of_measures() const;
    QVariantList reh_y_coords() const;
    QVariantList break_list() const;

    void setNumber_of_measures(const int &newNumber_of_measures);
    void setBreak_list(const QVariantList &newBreak_list);
    void setReh_y_coords(const QVariantList &newReh_y_coords);

signals:
    void reh_y_coordsChanged();
    void break_listChanged();

private:

    xml_parser* m_xml_parser;
    mei_parser* m_mei_parser;

    QVariantList m_reh_y_coords;

    QVariantList m_break_list;

    struct break_element {
        int mode; // 0 for delete, 1 for new
        QString id;
        int measure_number;
    };

    QVector<break_element> break_action_list; // stores actions in order

    void parse_reh_list();
    void parse_break_list();
    void break_list_sort();

    int number_of_measures; // total number of measures in the score, used to validate break input
};

#endif // PARSER_DATA_H
