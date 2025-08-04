#ifndef MEI_PARSER_H
#define MEI_PARSER_H

#include <QObject>
#include <QDomDocument>
#include <QDebug>

class mei_parser : public QObject
{
    Q_OBJECT
public:
    explicit mei_parser(QObject *parent = nullptr);
    void parse_mei(QString mei_data);

    // EDITING DATA -----------------------------------------------------------------------------
    // EDITING DATA -----------------------------------------------------------------------------
    // EDITING DATA -----------------------------------------------------------------------------

    QString export_mei_data();

    Q_INVOKABLE void insert_break(QString id, int measure_number);
    Q_INVOKABLE void delete_break(int measure_number);

    // EDITING DATA -----------------------------------------------------------------------------
    // EDITING DATA -----------------------------------------------------------------------------
    // EDITING DATA -----------------------------------------------------------------------------

    // DATA STRUCTURES --------------------------------------------------------------------------
    // DATA STRUCTURES --------------------------------------------------------------------------
    // DATA STRUCTURES --------------------------------------------------------------------------

    struct note_rest_element {
        QString class_name;
        QString id;
        QString note_name = "rest";
        int layer_n;
        int staff_n;
        int measure_number;
        int start_beat;
        int end_beat;
    };

    struct break_element {
        QString id;
        int measure_number; // measure before the break
    };

    struct rehearsal_mark_element {
        QString id;
        QString mark_text;
        int measure_number;
    };

    struct tempo_element {
        int tempo = 100; //default to 100
        int measure_start = 1; //inclusive of this measure, default if no tempo described
    };

    struct time_signature_element {
        int numerator;
        int denominator;
        int measure_start; //inclusive of this measure
    };

    struct part_element {
        QString id;
        QString part_name;
        int n_value;
    };

    // DATA STRUCTURES --------------------------------------------------------------------------
    // DATA STRUCTURES --------------------------------------------------------------------------
    // DATA STRUCTURES --------------------------------------------------------------------------

    QVector<break_element> get_breaks();

signals:
private:

    QDomDocument document; // Document to hold the parsed MEI data



    // PARSING DATA -----------------------------------------------------------------------------
    // PARSING DATA -----------------------------------------------------------------------------
    // PARSING DATA -----------------------------------------------------------------------------


    QVector<note_rest_element> current_layer;
    QVector<QVector<note_rest_element>> current_staff;
    QVector<QVector<QVector<note_rest_element>>> current_measure;
    QVector<QVector<QVector<QVector<note_rest_element>>>> notes_and_rests;
    QVector<break_element> breaks;
    QVector<rehearsal_mark_element> rehearsal_marks;
    QVector<tempo_element> tempo_changes;
    QVector<time_signature_element> time_signature_changes;
    QVector<part_element> parts;

    part_element process_part(QDomElement &staff_element);
    break_element process_break(QDomElement &break_element);
    time_signature_element process_time_signature(QDomElement &score_def_element);
    tempo_element process_tempo(QDomElement &bpm_element);
    rehearsal_mark_element process_rehearsal_mark(QDomElement &reh_element);
    QVector<note_rest_element> process_layer(QDomElement &layer_element);



    note_rest_element process_layer_note(QDomElement &note_element,
                                         int &layer_n, int &staff_n,
                                         int &measure_number, int &current_beat);
    note_rest_element process_layer_rest(QDomElement &rest_element,
                                         int &layer_n, int &staff_n,
                                         int &measure_number, int &current_beat);
    note_rest_element process_layer_mRest(QDomElement &mRest_element,
                                         int &layer_n, int &staff_n,
                                         int &measure_number, int &current_beat);
    note_rest_element process_layer_space(QDomElement &space_element,
                                          int &layer_n, int &staff_n,
                                          int &measure_number, int &current_beat);
    QVector<note_rest_element> process_layer_chord(QDomElement &chord_element,
                                          int &layer_n, int &staff_n,
                                          int &measure_number, int &current_beat);
    QVector<note_rest_element> process_layer_tabGrp(QDomElement &tabGrp_element,
                                            int &layer_n, int &staff_n,
                                            int &measure_number, int &current_beat);

    int get_beats_per_measure(int &measure_number);

    void clear_data();

    // PARSING DATA -----------------------------------------------------------------------------
    // PARSING DATA -----------------------------------------------------------------------------
    // PARSING DATA -----------------------------------------------------------------------------





};

#endif // MEI_PARSER_H
