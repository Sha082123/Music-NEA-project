#ifndef PART_OBJECT_H
#define PART_OBJECT_H

#include <QObject>
#include <QFile>
#include <QDomDocument>

#include "../data_parsing/mei_parser.h"
#include "../data_parsing/xml_parser.h"
#include "../data_parsing/parser_data.h"
#include "../render/render_file.h"
#include "../render/verovio_loader.h"
#include "../render/resvg_loader.h"

class part_object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList list_PNG_paths READ list_PNG_paths WRITE setlist_PNG_paths NOTIFY list_PNG_pathsChanged FINAL)
    Q_PROPERTY(QVariantList reh_y_coords READ reh_y_coords WRITE setReh_y_coords NOTIFY reh_y_coordsChanged FINAL)
    Q_PROPERTY(QVariantList break_list READ break_list WRITE setBreak_list NOTIFY break_listChanged FINAL)
    Q_PROPERTY(QVariantList part_list READ part_list WRITE setpart_list NOTIFY part_listChanged FINAL)
    Q_PROPERTY(QString saved READ saved WRITE setsaved NOTIFY savedChanged FINAL)

public:
    explicit part_object(QObject *parent = nullptr, QString part_name = "");

    Q_INVOKABLE void openFile(const QString &file_path, int mode); // mode: new for new files, existing for existing files
    Q_INVOKABLE void update();
    Q_INVOKABLE int new_break_item(QString input);
    Q_INVOKABLE int delete_break_item(int measure_number);
    Q_INVOKABLE int update_break_list(QString id, QString input);
    Q_INVOKABLE void apply_breaks();
    Q_INVOKABLE QVariantList element_from_point(const QPointF &point, const int &page_number);

    Q_INVOKABLE void save_file();

    void update_part_name(QString new_part_name);
    void update_part_staves(QVector<QPair<int, bool>> &part_existence, part_object* root_ptr);
    void delete_file();

    QString get_part_name();

    QDomDocument get_document() const;


    QStringList list_PNG_paths() const;
    void setlist_PNG_paths(const QStringList &newList_PNG_paths);

    parser_data *get_parser_data() const;

    QVariantList reh_y_coords() const;
    void setReh_y_coords(const QVariantList &newReh_y_coords);

    QVariantList break_list() const;
    void setBreak_list(const QVariantList &newBreak_list);

    QString file_path() const;
    void setfile_path(const QString &newFile_path);

    QVariantList part_list() const;
    void setpart_list(const QVariantList &newPart_list);

    QString saved() const;
    void setsaved(const QString &newSaved);

signals:



    void list_PNG_pathsChanged();

    void reh_y_coordsChanged();

    void break_listChanged();

    void part_listChanged();

    void savedChanged();

private:

    // Add all renderers and parsers
    mei_parser *m_mei_parser;
    xml_parser *m_xml_parser;
    parser_data *m_parser_data;

    render_file *m_render_file;
    verovio_loader *m_verovio_loader;
    resvg_loader *m_resvg_loader;


    QString m_part_name;

    QStringList m_list_PNG_paths;

    QVariantList m_reh_y_coords;
    QVariantList m_break_list;
    QString m_file_path;
    QVariantList m_part_list;

    QString m_saved;
};

#endif // PART_OBJECT_H

// extern image_provider* g_image_provider; // Global image provider instance
// extern verovio_loader* g_verovio_loader; // Global Verovio loader instance
// extern resvg_loader* g_resvg_loader; // Global Resvg loader instance
// extern xml_parser* g_xml_parser; // Global XML parser instance
// extern mei_parser* g_mei_parser; // Global MEI parser instance
// extern parser_data* g_parser_data; // Global parser data instance

