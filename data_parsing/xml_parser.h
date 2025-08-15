#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <QObject>
#include <QDomDocument>
#include <QPointF>
#include <QRegularExpression>
#include <QDebug>

class xml_parser : public QObject
{
    Q_OBJECT
public:
    explicit xml_parser(QObject *parent = nullptr);
    std::string parse_xml(const QString &svg_data);

    Q_INVOKABLE QVariantList element_from_point(const QPointF &point, const int &page_number);
    QVector<int> coordinates_from_measure(int measure_number);

    void set_info_for_note(QString &id, int &measure_number, int &start_beat, int &end_beat, QString note_name, int staff_number);
    void set_info_for_rest(QString &id, int &measure_number, int &start_beat, int &end_beat, int staff_number);
    void set_info_for_reh(QString &id, int &measure_number, QString &mark_text);

    void set_end_beat_for_note(QString &id, int& end_beat);
    void set_end_beat_for_rest(QString &id, int& end_beat);

    void clear_data() {
        all_notes.clear();
        all_rests.clear();
        all_rehearsal_marks.clear();
        all_elements.clear();
    }

    struct SvgElementInfo {
        QString class_name;
        QString id;
        QPointF position;
        QString note_name = "N/A";
        int width;
        int height;
        int measure_number = 0;
        int start_beat = 0;
        int end_beat;
        int staff_number;
    };

    QVector<QVector<SvgElementInfo>> get_all_rehearsal_marks();
    QVector<QVector<SvgElementInfo>> get_all_elements();


signals:
private:
    QDomDocument document; // Document to hold the parsed XML data
    QString svg_data; // String to hold the SVG data



    QVector<SvgElementInfo> notes;
    QVector<SvgElementInfo> rests;
    QVector<SvgElementInfo> rehearsal_marks;

    QVector<QVector<SvgElementInfo>> all_notes;
    QVector<QVector<SvgElementInfo>> all_rests;
    QVector<QVector<SvgElementInfo>> all_rehearsal_marks;

    QVector<QVector<SvgElementInfo>> all_elements;

    QStringList target_classes;

    SvgElementInfo process_note(const QDomElement &element);
    SvgElementInfo process_rest(const QDomElement &element);
    SvgElementInfo process_mRest(const QDomElement &element);
    SvgElementInfo process_reh(const QDomElement &element);

    QStringList tab_dur_sym_list;

    void remove_tab_dur_sym(const QDomElement &element);
};

#endif // XML_PARSER_H
