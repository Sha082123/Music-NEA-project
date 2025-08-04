#include "mei_parser.h"
#include "globals.h"

mei_parser::mei_parser(QObject *parent)
    : QObject{parent}
{}

void mei_parser::parse_mei(QString mei_data)
{
    clear_data (); // Clear previous data

    document.setContent(mei_data);

    QDomElement root = document.documentElement();

    // First, process header contents (staffGrp) to get starting time signature and part data

    QDomNodeList time_signature_elements = root.elementsByTagName("meterSig");

    if (time_signature_elements.isEmpty ()) {
        time_signature_elements = root.elementsByTagName("meter");
    }

    QDomElement starting_time_signature_element = time_signature_elements.at(0).toElement(); //only needs one
    // add function !!! to process time signature element (DONE!)

    if (!starting_time_signature_element.isNull()) {
        time_signature_element starting_time_signature;

        // case 1: normal
        starting_time_signature.numerator = starting_time_signature_element.attribute("count").toInt();
        starting_time_signature.denominator = starting_time_signature_element.attribute("unit").toInt();

        // case 2: different
        if (starting_time_signature.numerator == 0) {
            starting_time_signature.numerator = starting_time_signature_element.attribute("meter.count").toInt();
        }

        if (starting_time_signature.denominator == 0) {
            starting_time_signature.denominator = starting_time_signature_element.attribute("meter.unit").toInt();
        }

        starting_time_signature.measure_start = 1; //start of the score

        time_signature_changes.append(starting_time_signature);
    } else {
        time_signature_element default_time_signature;
        default_time_signature.numerator = 4; //default to 4/4
        default_time_signature.denominator = 4;
        default_time_signature.measure_start = 1; //start of the score

        time_signature_changes.append(default_time_signature);
    }

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Starting time signature numerator: " << time_signature_changes[0].numerator;
    // qInfo() << "Starting time signature denominator: " << time_signature_changes[0].denominator;
    // qInfo() << "Starting time signature measure: " << time_signature_changes[0].measure_start;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    QDomNodeList staff_group = root.elementsByTagName("staffDef");
    for (int i = 0; i < staff_group.count(); ++i) {
        QDomElement staff_element = staff_group.at(i).toElement();
        if (!staff_element.isNull()) {
            // add function to process staff element (DONE!)
            part_element part_info = process_part(staff_element);
            parts.append(part_info);
        }
    }
    //------------------------------------------------------------------------------//
    //------------------------------------------------------------------------------//
    //------------------------------------------------------------------------------//

    // now process elements under <section/> tag (everything else)

    QDomNodeList section_element_list = root.elementsByTagName("section");
    if (!section_element_list.isEmpty ()) {
        QDomElement section_element = section_element_list.at(0).toElement();
        QDomNodeList break_list = section_element.elementsByTagName("sb");

        for (int i = 0; i < break_list.count(); ++i) {
            QDomElement break_element = break_list.at(i).toElement();
            if (!break_element.isNull()) {
                //add function to process the break element (DONE!)
                breaks.append(process_break(break_element));
            }
        }

        QDomNodeList time_signature_changes_list = section_element.elementsByTagName("scoreDef");
        for (int i = 0; i < time_signature_changes_list.count(); ++i) {
            QDomElement score_def = time_signature_changes_list.at(i).toElement();

            time_signature_element time_signature_info = process_time_signature(score_def);

            if ((time_signature_info.numerator != 0) && (time_signature_info.denominator != 0)) {
                // Only append if the time signature is valid
                time_signature_changes.append(time_signature_info);
            }

        }

        bool has_tempo = false;
        QDomNodeList measure_element_list = section_element.elementsByTagName("measure");
        for (int i = 0; i < measure_element_list.count(); ++i) {
            QDomElement current_measure_element = measure_element_list.at(i).toElement();

            QDomNodeList tempo_elements = current_measure_element.elementsByTagName("tempo");

            for (int j = 0; j < tempo_elements.count(); ++j) {
                QDomElement tempo_el = tempo_elements.at(j).toElement();
                if (!tempo_el.isNull()) {
                    // add function to process the tempo element (DONE!)
                    tempo_changes.append(process_tempo(tempo_el));
                    has_tempo = true;
                }
            }

            if (tempo_changes[0].measure_start != 1 || !has_tempo) { // if no tempo defined at measure 1
                tempo_element default_tempo;
                tempo_changes.prepend(default_tempo); // add default tempo to front
            }

            // for (tempo_element &tempo : tempo_changes) {
            //     qInfo() << tempo.measure_start << " " << tempo.tempo;
            // }

            QDomNodeList rehearsal_mark_elements = current_measure_element.elementsByTagName("reh");
            for (int j = 0; j < rehearsal_mark_elements.count(); ++j) {
                QDomElement reh_element = rehearsal_mark_elements.at(j).toElement();
                if (!reh_element.isNull()) {
                    // add function to process the rehearsal mark element (DONE!)
                    rehearsal_marks.append(process_rehearsal_mark(reh_element));
                }
            }

            QDomNodeList staff_elements = current_measure_element.elementsByTagName("staff");
            current_measure.clear();

            for (int staff_index = 0; staff_index < staff_elements.count(); ++staff_index) {
                QDomElement current_staff_element = staff_elements.at(staff_index).toElement();
                QDomNodeList layer_elements = current_staff_element.elementsByTagName("layer");
                current_staff.clear();

                for (int layer_index = 0; layer_index < layer_elements.count(); ++layer_index) {
                    QDomElement current_layer_element = layer_elements.at(layer_index).toElement();

                    current_layer.clear();
                    current_layer = process_layer(current_layer_element);
                    current_staff.append(current_layer);
                }
                current_measure.append(current_staff);
            }
            notes_and_rests.append(current_measure);
        }
    }

    // NOW, LOOP THROUGH THE LIST TO MAKE SURE EACH LAYER HAS THE COORECT TIME FOR THE BAR

    for (int measure_index = 0; measure_index < notes_and_rests.size(); ++measure_index) {
        QVector<QVector<QVector<note_rest_element>>> &current_measure = notes_and_rests[measure_index];

        int measure_number = current_measure[0][0][0].measure_number;
        int correct_beat = get_beats_per_measure(measure_number);

        for (int staff_index = 0; staff_index < current_measure.size(); ++staff_index) {
            QVector<QVector<note_rest_element>> &current_staff = current_measure[staff_index];
            for (int layer_index = 0; layer_index < current_staff.size(); ++layer_index) {
                QVector<note_rest_element> &current_layer = current_staff[layer_index];

                int index = current_layer.count() - 1;


                while (current_layer[index].end_beat > correct_beat) {
                    // If the last note's end beat is greater than the correct beat, remove it
                    current_layer.removeAt(index);
                    index--;
                }

                int stop_searching_beat = current_layer[index].start_beat;


                while ((current_layer[index].end_beat < correct_beat) && (index >= 0)) {
                    // If the last note's end beat is less than the correct beat, force the end beat to the end
                    if (current_layer[index].end_beat == stop_searching_beat) {
                        break; // If the end beat is already at the stop searching beat, break
                    }
                    else {
                        // qInfo() << "Correcting end beat for note: " << current_layer[index].id
                        //        << " from " << current_layer[index].end_beat
                        //        << " to " << correct_beat;
                        // current_layer[index].end_beat = correct_beat;

                        g_xml_parser->set_end_beat_for_note(current_layer[index].id, correct_beat);

                        index--;
                    }
                }
            }
        }
    }

    g_parser_data->setNumber_of_measures(notes_and_rests.size());

    // DEBUG INFO ------------------------------------------------------------------------------------------
    // DEBUG INFO ------------------------------------------------------------------------------------------
    // DEBUG INFO ------------------------------------------------------------------------------------------

    int note_rest_count = 0;

    for (const auto &measure : notes_and_rests) {
        for (const auto &staff : measure) {
            for (const auto &layer : staff) {
                note_rest_count += layer.size();
            }
        }
    }

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Number of parts: " << parts.size();
    // qInfo() << "Number of breaks: " << breaks.size();
    // qInfo() << "Number of rehearsal marks: " << rehearsal_marks.size();
    // qInfo() << "Number of tempo changes: " << tempo_changes.size();
    // qInfo() << "Number of time signature changes: " << time_signature_changes.size();
    // qInfo() << "Number of measures: " << notes_and_rests.size();
    // qInfo() << "Number of notes and rests: " << note_rest_count;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    // DEBUG INFO ------------------------------------------------------------------------------------------
    // DEBUG INFO ------------------------------------------------------------------------------------------
    // DEBUG INFO ------------------------------------------------------------------------------------------

}

int mei_parser::get_beats_per_measure(int &measure_number)
{
    for (int bar = measure_number; bar > 0; --bar) {
        for (const auto &time_signature : time_signature_changes) {
            if (time_signature.measure_start == bar) {
                int numerator = time_signature.numerator;
                int denominator = time_signature.denominator;
                int beats_per_measure = (128/denominator) * numerator;
                //qInfo() << "Beats per measure for measure " << bar << ": " << beats_per_measure;
                return beats_per_measure;
            }
        }
    }
    return 128; // Default to 128 if no time signature found, which is a whole note
}


QVector<mei_parser::note_rest_element> mei_parser::process_layer_tabGrp(QDomElement &tabGrp_element, int &layer_n, int &staff_n, int &measure_number, int &current_beat)
{
    QVector<note_rest_element> tabGrp_notes;

    int duration = tabGrp_element.attribute("dur").toInt();
    int dots = tabGrp_element.attribute("dots").toInt();
    int time = (128/duration) * pow((3.0/2.0), dots); // 128 is the default duration of a whole note

    QDomNodeList note_elements = tabGrp_element.elementsByTagName("note");

    if (note_elements.isEmpty ()) {
        note_rest_element rest_info;
        rest_info.class_name = "rest";
        rest_info.id = tabGrp_element.attribute("xml:id");
        rest_info.start_beat = current_beat;
        rest_info.end_beat = current_beat + time;
        rest_info.layer_n = layer_n;
        rest_info.staff_n = staff_n;
        rest_info.measure_number = measure_number;

        qInfo() << Qt::endl << Qt::endl << Qt::endl;
        qInfo() << "Tab Note ID: " << rest_info.id;
        qInfo() << "Tab Note start beat: " << rest_info.start_beat;
        qInfo() << "Tab Note end beat: " << rest_info.end_beat;
        qInfo() << "Tab Note layer: " << rest_info.layer_n;
        qInfo() << "Tab Note staff: " << rest_info.staff_n;
        qInfo() << "Tab Note measure number: " << rest_info.measure_number;
        qInfo() << Qt::endl << Qt::endl << Qt::endl;

        tabGrp_notes.append(rest_info);
    }

    else {
        for (int index = 0; index < note_elements.count(); ++index) {
            QDomElement note_element = note_elements.at(index).toElement();

            note_rest_element note_info;

            note_info.class_name = "note";
            note_info.id = note_element.attribute("xml:id");
            note_info.start_beat = current_beat;
            note_info.end_beat = current_beat + time;
            note_info.layer_n = layer_n;
            note_info.staff_n = staff_n;
            note_info.measure_number = measure_number;

            QString fret = note_element.attribute("tab.fret");
            QString string = note_element.attribute("tab.course");

            note_info.note_name = "fret: " + fret + ", string: " + string;

            // qInfo() << Qt::endl << Qt::endl << Qt::endl;
            // qInfo() << "Tab Note ID: " << note_info.id;
            // qInfo() << "Tab Note start beat: " << note_info.start_beat;
            // qInfo() << "Tab Note end beat: " << note_info.end_beat;
            // qInfo() << "Tab Note layer: " << note_info.layer_n;
            // qInfo() << "Tab Note staff: " << note_info.staff_n;
            // qInfo() << "Tab Note measure number: " << note_info.measure_number;
            // qInfo() << Qt::endl << Qt::endl << Qt::endl;

            g_xml_parser->set_info_for_note(note_info.id, measure_number, note_info.start_beat, note_info.end_beat, note_info.note_name);

            tabGrp_notes.append(note_info);
        }
    }
    current_beat += time;

    return tabGrp_notes;
}

QVector<mei_parser::note_rest_element> mei_parser::process_layer_chord(QDomElement &chord_element, int &layer_n, int &staff_n, int &measure_number, int &current_beat)
{
    QVector<note_rest_element> chord_notes;

    int duration = chord_element.attribute("dur").toInt();
    int dots = chord_element.attribute("dots").toInt();
    int time = (128/duration) * pow((3.0/2.0), dots); // 128 is the default duration of a whole note

    QDomNodeList note_elements = chord_element.elementsByTagName("note");

    for (int index = 0; index < note_elements.count(); ++index) {
        QDomElement note_element = note_elements.at(index).toElement();

        note_rest_element note_info;

        note_info.class_name = "note";
        note_info.id = note_element.attribute("xml:id");
        note_info.start_beat = current_beat;
        note_info.end_beat = current_beat + time;
        note_info.layer_n = layer_n;
        note_info.staff_n = staff_n;
        note_info.measure_number = measure_number;
        note_info.note_name = note_element.attribute("pname") + note_element.attribute("oct");

        // qInfo() << Qt::endl << Qt::endl << Qt::endl;
        // qInfo() << "Chord Note ID: " << note_info.id;
        // qInfo() << "Chord Note start beat: " << note_info.start_beat;
        // qInfo() << "Chord Note end beat: " << note_info.end_beat;
        // qInfo() << "Chord Note layer: " << note_info.layer_n;
        // qInfo() << "Chord Note staff: " << note_info.staff_n;
        // qInfo() << "Chord Note measure number: " << note_info.measure_number;
        // qInfo() << Qt::endl << Qt::endl << Qt::endl;

        g_xml_parser->set_info_for_note(note_info.id, measure_number, note_info.start_beat, note_info.end_beat, note_info.note_name);

        chord_notes.append(note_info);
    }
    current_beat += time;

    return chord_notes;
}

mei_parser::note_rest_element mei_parser::process_layer_space(QDomElement &space_element, int &layer_n, int &staff_n, int &measure_number, int &current_beat)
{
    struct note_rest_element rest_info;

    int duration = space_element.attribute("dur").toInt();
    int dots = space_element.attribute("dots").toInt();
    int time = (128/duration) * pow((3.0/2.0), dots); // 128 is the default duration of a whole note

    rest_info.class_name = "space";
    rest_info.id = space_element.attribute("xml:id");
    rest_info.start_beat = current_beat;
    rest_info.end_beat = current_beat + time;
    rest_info.layer_n = layer_n;
    rest_info.staff_n = staff_n;
    rest_info.measure_number = measure_number;
    current_beat += time;

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Space ID: " << rest_info.id;
    // qInfo() << "Space start beat: " << rest_info.start_beat;
    // qInfo() << "Space end beat: " << rest_info.end_beat;
    // qInfo() << "Space layer: " << rest_info.layer_n;
    // qInfo() << "Space staff: " << rest_info.staff_n;
    // qInfo() << "Space measure number: " << rest_info.measure_number;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    return rest_info;
}

mei_parser::note_rest_element mei_parser::process_layer_mRest(QDomElement &mRest_element, int &layer_n, int &staff_n, int &measure_number, int &current_beat)
{
    note_rest_element rest_info;

    int beats_in_bar = get_beats_per_measure(measure_number);

    rest_info.class_name = "rest";
    rest_info.id = mRest_element.attribute("xml:id");
    rest_info.start_beat = current_beat;
    rest_info.end_beat = beats_in_bar;
    rest_info.layer_n = layer_n;
    rest_info.staff_n = staff_n;
    rest_info.measure_number = measure_number;

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Rest ID: " << rest_info.id;
    // qInfo() << "Rest start beat: " << rest_info.start_beat;
    // qInfo() << "Rest end beat: " << rest_info.end_beat;
    // qInfo() << "Rest layer: " << rest_info.layer_n;
    // qInfo() << "Rest staff: " << rest_info.staff_n;
    // qInfo() << "Rest measure number: " << rest_info.measure_number;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    g_xml_parser->set_info_for_rest(rest_info.id, measure_number, rest_info.start_beat, rest_info.end_beat);

    return rest_info;
}



mei_parser::note_rest_element mei_parser::process_layer_rest(QDomElement &rest_element, int &layer_n, int &staff_n, int &measure_number, int &current_beat)
{
    struct note_rest_element rest_info;

    int duration = rest_element.attribute("dur").toInt();
    int dots = rest_element.attribute("dots").toInt();
    int time = (128/duration) * pow((3.0/2.0), dots); // 128 is the default duration of a whole note

    rest_info.class_name = "rest";
    rest_info.id = rest_element.attribute("xml:id");
    rest_info.start_beat = current_beat;
    rest_info.end_beat = current_beat + time;
    rest_info.layer_n = layer_n;
    rest_info.staff_n = staff_n;
    rest_info.measure_number = measure_number;
    current_beat += time;

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Note ID: " << note_info.id;
    // qInfo() << "Note start beat: " << note_info.start_beat;
    // qInfo() << "Note end beat: " << note_info.end_beat;
    // qInfo() << "Note layer: " << note_info.layer_n;
    // qInfo() << "Note staff: " << note_info.staff_n;
    // qInfo() << "Note measure number: " << note_info.measure_number;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    g_xml_parser->set_info_for_rest(rest_info.id, measure_number, rest_info.start_beat, rest_info.end_beat);

    return rest_info;
}

mei_parser::note_rest_element mei_parser::process_layer_note(QDomElement &note_element, int &layer_n, int &staff_n, int &measure_number, int &current_beat)
{
    struct note_rest_element note_info;

    int duration = note_element.attribute("dur").toInt();
    int dots = note_element.attribute("dots").toInt();
    int time = (128/duration) * pow((3.0/2.0), dots); // 128 is the default duration of a whole note

    note_info.class_name = "note";
    note_info.id = note_element.attribute("xml:id");
    note_info.start_beat = current_beat;
    note_info.end_beat = current_beat + time;
    note_info.layer_n = layer_n;
    note_info.staff_n = staff_n;
    note_info.measure_number = measure_number;
    note_info.note_name = note_element.attribute("pname") + note_element.attribute("oct");
    current_beat += time;

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Note ID: " << note_info.id;
    // qInfo() << "Note start beat: " << note_info.start_beat;
    // qInfo() << "Note end beat: " << note_info.end_beat;
    // qInfo() << "Note layer: " << note_info.layer_n;
    // qInfo() << "Note staff: " << note_info.staff_n;
    // qInfo() << "Note measure number: " << note_info.measure_number;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    g_xml_parser->set_info_for_note(note_info.id, measure_number, note_info.start_beat, note_info.end_beat, note_info.note_name);

    return note_info;
}

QVector<mei_parser::note_rest_element> mei_parser::process_layer(QDomElement &layer_element)
{
    QVector<note_rest_element> layer_list;

    QDomElement parent_staff = layer_element.parentNode().toElement();
    QDomElement grandparent_measure = parent_staff.parentNode().toElement();

    int layer_n = layer_element.attribute("n").toInt();
    int staff_n = parent_staff.attribute("n").toInt();
    int measure_number = grandparent_measure.attribute("n").toInt();

    QDomNodeList children = layer_element.childNodes();

    int current_beat = 0;

    for (int child_index = 0; child_index < children.count(); ++child_index) {
        QDomElement child_element = children.at(child_index).toElement();
        QString tag_name = child_element.tagName();

        // if child is note
        // if child is rest
        // if child is mRest
        // if child is space
        // if child is mSpace !!! (added due to testing with Live_2)
        // if child is chord
        // if child is tabGrp
        // if child is beam

        if (tag_name == "note") {
            // process the note (DONE!)
            note_rest_element current_element = process_layer_note(child_element, layer_n, staff_n, measure_number, current_beat);
            layer_list.append(current_element);
        }

        else if (tag_name == "rest") {
            // process the rest (DONE!)
            note_rest_element current_element = process_layer_rest(child_element, layer_n, staff_n, measure_number, current_beat);
            layer_list.append(current_element);
        }

        else if (tag_name == "mRest") {
            // process the mRest (DONE!)
            note_rest_element current_element = process_layer_mRest(child_element, layer_n, staff_n, measure_number, current_beat);
            layer_list.append(current_element);
        }

        else if (tag_name == "space") {
            // process the space (DONE!)
            note_rest_element current_element = process_layer_space(child_element, layer_n, staff_n, measure_number, current_beat);
            layer_list.append(current_element);
        }

        else if (tag_name == "mSpace") {
            // process the mSpace (using mRest for now)
            note_rest_element current_element = process_layer_mRest(child_element, layer_n, staff_n, measure_number, current_beat);
            layer_list.append(current_element);
        }

        else if (tag_name == "chord") {
            // process the chord (DONE!)
            QVector<note_rest_element> note_list = process_layer_chord(child_element, layer_n, staff_n, measure_number, current_beat);
            for (const auto &note : note_list) {
                layer_list.append(note);
            }
        }

        else if (tag_name == "tabGrp") {
            // process the tabGrp (DONE!)
            QVector<note_rest_element> note_list = process_layer_tabGrp(child_element, layer_n, staff_n, measure_number, current_beat);
            for (const auto &note : note_list) {
                layer_list.append(note);
            }
        }

        else if (tag_name == "beam") {
            // process the beam (DONE!)
            QDomNodeList beam_children = child_element.childNodes();

            for (int beam_index = 0; beam_index < beam_children.count(); ++beam_index) {
                QDomElement beam_child = beam_children.at(beam_index).toElement();
                if (beam_child.tagName() == "note") {
                    note_rest_element current_element = process_layer_note(beam_child, layer_n, staff_n, measure_number, current_beat);
                    layer_list.append(current_element);
                }
                else if (beam_child.tagName() == "chord") {
                    QVector<note_rest_element> note_list = process_layer_chord(beam_child, layer_n, staff_n, measure_number, current_beat);
                    for (const auto &note : note_list) {
                        layer_list.append(note);
                    }
                }
            }
        }
    }

    return layer_list;
}


mei_parser::rehearsal_mark_element mei_parser::process_rehearsal_mark(QDomElement &reh_element)
{
    rehearsal_mark_element reh_info;

    QDomElement child_rend = reh_element.firstChildElement("rend");

    QDomElement parent_measure = reh_element.parentNode().toElement();

    reh_info.id = reh_element.attribute("xml:id");
    reh_info.mark_text = child_rend.text();
    reh_info.measure_number = parent_measure.attribute("n").toInt();

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Rehearsal mark id: " << reh_info.id;
    // qInfo() << "Rehearsal mark text: " << reh_info.mark_text;
    // qInfo() << "Rehearsal mark measure number: " << reh_info.measure_number;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    g_xml_parser->set_info_for_reh(reh_info.id, reh_info.measure_number, reh_info.mark_text);

    return reh_info;
}

mei_parser::tempo_element mei_parser::process_tempo(QDomElement &bpm_element)
{
    struct tempo_element tempo_info;

    int bpm = bpm_element.attribute("midi.bpm").toInt();
    if (bpm == 0) {
        bpm = 100; // default tempo if tempo = 0
    }

    tempo_info.tempo = bpm;

    QDomElement parent_measure = bpm_element.parentNode().toElement();

    tempo_info.measure_start = parent_measure.attribute("n").toInt();

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Tempo: " << tempo_info.tempo;
    // qInfo() << "Measure starting with tempo: " << tempo_info.measure_start;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    return tempo_info;
}

mei_parser::time_signature_element mei_parser::process_time_signature(QDomElement &score_def_element)
{
    struct time_signature_element time_signature_info;

    QDomElement meter_element = score_def_element.firstChildElement("meter");
    if (meter_element.isNull ()) {
        meter_element = score_def_element.firstChildElement("meterSig");
    }

    // case 1: normal
    time_signature_info.numerator = meter_element.attribute("count").toInt();
    time_signature_info.denominator = meter_element.attribute("unit").toInt();

    // case 2: different names
    if (time_signature_info.numerator == 0) {
        time_signature_info.numerator = meter_element.attribute("meter.count").toInt();
    }

    if (time_signature_info.denominator == 0) {
        time_signature_info.denominator = meter_element.attribute("meter.unit").toInt();
    }

    // case 3: inside scoreDef normal names
    if (time_signature_info.numerator == 0) {
        time_signature_info.numerator = score_def_element.attribute("count").toInt();
    }

    if (time_signature_info.denominator == 0) {
        time_signature_info.denominator = score_def_element.attribute("unit").toInt();
    }

    // case 4: inside scoreDef different names
    if (time_signature_info.numerator == 0) {
        time_signature_info.numerator = score_def_element.attribute("meter.count").toInt();
    }

    if (time_signature_info.denominator == 0) {
        time_signature_info.denominator = score_def_element.attribute("meter.unit").toInt();
    }

    QDomElement next_measure = score_def_element.nextSiblingElement("measure");

    time_signature_info.measure_start = next_measure.attribute("n").toInt();

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Starting time signature numerator: " << time_signature_info.numerator;
    // qInfo() << "Starting time signature denominator: " << time_signature_info.denominator;
    // qInfo() << "Starting time signature measure: " << time_signature_info.measure_start;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    return time_signature_info;
}

mei_parser::break_element mei_parser::process_break(QDomElement &break_element)
{
    struct break_element break_info;

    break_info.id = break_element.attribute("xml:id");

    QDomElement previous_measure = break_element.previousSiblingElement ("measure");

    break_info.measure_number = previous_measure.attribute("n").toInt();

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Break id: " << break_info.id;
    // qInfo() << "Measure number: " << break_info.measure_number;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    return break_info;
}


mei_parser::part_element mei_parser::process_part(QDomElement &staff_element)
{
    struct part_element part_info;

    QDomElement label_element = staff_element.previousSiblingElement ("label");

    if (!label_element.isNull()) {
        part_info.part_name = label_element.text();
    } else {
        part_info.part_name = "Untitled"; // Default name if no label is found
    }

    part_info.id = staff_element.attribute("xml:id");
    part_info.n_value = staff_element.attribute("n").toInt();

    // qInfo() << Qt::endl << Qt::endl << Qt::endl;
    // qInfo() << "Part ID: " << part_info.id;
    // qInfo() << "Part Name: " << part_info.part_name;
    // qInfo() << "Part N Value: " << part_info.n_value;
    // qInfo() << Qt::endl << Qt::endl << Qt::endl;

    return part_info;
}

void mei_parser::clear_data()
{
    current_layer.clear();
    current_staff.clear();
    current_measure.clear();
    notes_and_rests.clear();
    breaks.clear();
    rehearsal_marks.clear();
    tempo_changes.clear();
    time_signature_changes.clear();
    parts.clear();
}









QString mei_parser::export_mei_data()
{
    return document.toString();
}

void mei_parser::insert_break(QString id, int measure_number)
{
    QDomElement root = document.documentElement();

    QDomNodeList measure_elements = root.elementsByTagName("measure");

    for (int index = 0; index < measure_elements.count(); ++index) {
        QDomElement measure_element = measure_elements.at(index).toElement();

        if (measure_element.attribute("n").toInt() == measure_number) {
            QDomElement break_element = document.createElement("sb");
            break_element.setAttribute("xml:id", id);

            QDomNode parent_node = measure_element.parentNode();
            parent_node.insertAfter(break_element, measure_element);

            return;
        }
    }
}

void mei_parser::delete_break(int measure_number)
{
    QDomElement root = document.documentElement();

    QDomNodeList measure_elements = root.elementsByTagName("measure");

    for (int index = 0; index < measure_elements.count(); ++index) {
        QDomElement measure_element = measure_elements.at(index).toElement();

        if (measure_element.attribute("n").toInt() == measure_number) {
            QDomNode break_node = measure_element.nextSibling();

            QDomNode parent_node = measure_element.parentNode();

            parent_node.removeChild(break_node);

            qInfo() << Qt::endl << Qt::endl << Qt::endl;
            qInfo() << "Break deleted from measure " << measure_number;
            qInfo() << Qt::endl << Qt::endl << Qt::endl;

            return;
        }
    }
}

QVector<mei_parser::break_element> mei_parser::get_breaks()
{
    return breaks;
}
