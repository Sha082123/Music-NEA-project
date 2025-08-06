import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: frame
    property string file_path: ""
    //property string part_directory: file_open.get_current_dir() + "/UserFiles/dump/" + file_open.name_from_project_files(current_part.file_path)
    property real image_scale: 1.0


    height: parent.height
    width: parent.width
    focus: true


    signal windowOpened()
    signal refreshed()

    onRefreshed: {
        part_selection.currentIndex = 0 // Switch to the newly created part
        music_stack.currentIndex = 0 // Switch to the newly created part
    }

    border.width: 10

    border.color: focus ? "red" : "black"

    clip: true


    Rectangle {
        id: toolbar
        height: 30
        width: parent.width
        color: "white"
        border.width: 1
        border.color: "black"
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        Button {
            id: refresh
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "R"

            onClicked: {
                current_part.new_break_item(selection_view.measure_number)
                current_part.apply_breaks()
                current_part.update()

                console.log("refreshing...")

                music_stack.itemAt(music_stack.currentIndex).viewer.positionViewAtIndex(selection_view.page_number - 1, ListView.Beginning)
                music_stack.itemAt(music_stack.currentIndex).viewer.contentY += (selection_view.y_coords * viewer.scale_factor) // add y offset
            }
        }

        Button {
            id: zoom_in
            anchors.left: refresh.right
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "+"

            onClicked: {
                console.log("Zooming in")
                image_scale += 0.1
                image_scale = Math.min(image_scale, 1.0) // Prevent scale from going above 1.0
            }
        }

        Button {
            id: zoom_out
            anchors.left: zoom_in.right
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "-"

            onClicked: {
                console.log("Zooming out")
                image_scale -= 0.1
                image_scale = Math.max(image_scale, 0.1) // Prevent scale from going below 0.1
            }
        }

        Button {
            id: break_manager
            anchors.left: zoom_out.right
            anchors.bottom: parent.bottom
            width: 80
            height: parent.height
            text: "Breaks"

            onClicked: {
                break_window.visible = true
            }
        }

        Button {
            id: delete_break
            anchors.left: break_manager.right
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "X"

            onClicked: {
                current_part.delete_break_item(selection_view.measure_number)
                current_part.apply_breaks()
                current_part.update()

                console.log("deleting...")
                music_stack.itemAt(music_stack.currentIndex).viewer.positionViewAtIndex(selection_view.page_number - 1, ListView.Beginning)
                music_stack.itemAt(music_stack.currentIndex).viewer.contentY += (selection_view.y_coords * viewer.scale_factor) // add y offset

            }
        }

        Button {
            id: refresh_button
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: delete_break.right
            width: parent.height
            height: parent.height
            text: "refresh"

            onClicked: {
                refreshed()

            }
        }
    }

    Rectangle {
        id: left_panel
        height: parent.height
        width: 200
        anchors.left: frame.left
        anchors.top: toolbar.bottom
        anchors.bottom: frame.bottom

        ComboBox {
            id: rehearsal_marks

            editable: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            displayText: "Rehearsal marks"

            model: current_part.reh_y_coords

            delegate: ItemDelegate {
                text: modelData[1]
                width: parent.width
                onClicked: {
                    console.log("original contentY: ", music_stack.itemAt(music_stack.currentIndex).viewer.contentY)
                    console.log("Scale factor: ", music_stack.itemAt(music_stack.currentIndex).viewer.scale_factor)

                    music_stack.itemAt(music_stack.currentIndex).viewer.positionViewAtIndex(modelData[3], ListView.beginning) // jump to page

                    music_stack.itemAt(music_stack.currentIndex).viewer.contentY +=
                            (modelData[2] * music_stack.itemAt(music_stack.currentIndex).viewer.scale_factor) // add y offset
                }
            }
        }

        Button {
            property string part_file_path: ""

            anchors {
                top: rehearsal_marks.bottom
                left: parent.left
                right: parent.right
            }

            height: 60

            text: "Create part"

            onClicked: {

                part_maker.visible = true

                // part_manager.create_new_part("test_2")

                // part_selection.currentIndex = part_manager.list_size() - 1 // Switch to the newly created part
                // music_stack.currentIndex = part_manager.list_size() - 1 // Switch to the newly created part

                // part_manager.set_current_part(0) // create part relative to the root part

                // part_file_path = file_open.createNewPart(current_part.file_path,
                //                         file_open.get_current_dir() + "/UserFiles/dump/" +
                //                         file_open.name_from_project_files(current_part.file_path) + "/")

                // part_manager.set_current_part(part_manager.list_size() - 1) // Set the newly created part as current
                // current_part.openFile(part_file_path, 1)


            }
        }


        Rectangle {
            id: selection_view

            property int measure_number: 0
            property real start_beat: 0
            property real end_beat: 0
            property string note_name: ""
            property int x_coords: 0
            property int y_coords: 0
            property int page_number: 0

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                margins: 10
            }

            height: 50

            Text {
                text: "Measure: " + selection_view.measure_number +
                      "\n" + "Beats: " + selection_view.start_beat + "-" + selection_view.end_beat +
                      "\n" + "Note: " + selection_view.note_name
            }
        }
    }

    Rectangle {
        id: border
        anchors.top: toolbar.bottom
        anchors.bottom: frame.bottom
        anchors.left: left_panel.right
        width: 8
        color: "black"

        property double old_scale_factor: 0
        property double old_contentY: 0

        MouseArea {

            anchors.fill: parent

            property bool dragging: false
            property real x_original
            property real change_x

            onPressed: {
                dragging = true
                x_original = mouseX

                border.old_scale_factor = music_stack.itemAt(music_stack.currentIndex).viewer.scale_factor
                border.old_contentY = music_stack.itemAt(music_stack.currentIndex).viewer.contentY
            }

            onPositionChanged: {
                // Check distance from press position
                change_x = mouseX - x_original


                if (dragging === true) {
                    // Do your drag logic here
                    //console.log("dragging", change_x)
                    if (change_x < 0) {
                        left_panel.width = Math.max(100, left_panel.width + change_x);
                    } else {
                        left_panel.width = Math.min(700, left_panel.width + change_x);
                    }
                }

                //viewer.contentY += viewer.originY

                console.log("contentY: ", music_stack.itemAt(music_stack.currentIndex).viewer.contentY)
                console.log("originY: ", music_stack.itemAt(music_stack.currentIndex).viewer.originY)
            }

            onReleased: {
                dragging = false

                //viewer.contentY = border.old_contentY * (viewer.scale_factor / border.old_scale_factor)
            }
        }
    }



    TabBar {
        id: part_selection
        anchors {
            bottom: parent.bottom
            left: border.right
            right: parent.right
        }

        z: 999

        Repeater {
            model: part_manager.buffer_part_name_list

            TabButton {
                text: modelData
                onPressed: {
                    part_manager.set_current_part(index)
                    console.log("Current part set to:", modelData, index)
                    console.log("file path", current_part.file_path)

                    console.log("number of parts", part_manager.list_size())

                    //music_stack.itemAt(music_stack.currentIndex).viewer.positionViewAtIndex(0, ListView.Beginning) // Reset view to the beginning
                }
            }
        }
    }


    Rectangle {
        id: right_panel

        anchors.right: frame.right
        anchors.bottom: part_selection.top
        anchors.top: toolbar.bottom
        anchors.left: border.right


        StackLayout {
            anchors.fill: parent

            id: music_stack

            currentIndex: part_selection.currentIndex

            Repeater {
                model: part_manager.buffer_part_name_list

                Music_scroll_view {
                    id: scrollView
                }
            }
        }
    }


    Break_manager {
        id: break_window
        visible: false

    }

    Part_maker {
        id: part_maker
        visible: false
    }



}
