#ifndef PARSER_DATA_H
#define PARSER_DATA_H

#include <QObject>
#include <QVariant>
#include <QDebug>

class parser_data : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList reh_y_coords READ reh_y_coords WRITE setReh_y_coords NOTIFY reh_y_coordsChanged FINAL)
    Q_PROPERTY(QVariantList break_list READ break_list WRITE setBreak_list NOTIFY break_listChanged FINAL)
public:
    explicit parser_data(QObject *parent = nullptr);

    void parse();
    Q_INVOKABLE int update_break_list(QString id, QString input); // 0 = no error, -1 = invalid input,
                                                                  // -2 = out of range, -3 = already exists

    Q_INVOKABLE int delete_break_item(QString id); // 0 = no error, -1 = could not find break id

    Q_INVOKABLE int new_break_item(QString input);  // 0 = no error, -1 = invalid input,
                                                    // -2 = out of range, -3 = already exists

    Q_INVOKABLE void apply_breaks(); // Applies the break action list

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
