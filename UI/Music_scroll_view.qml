import QtQuick 2.15
import QtQuick.Controls



ScrollView{

    id: scrollView

    property alias viewer: viewer
    property alias tracker: tracker

    clip: true

    anchors.fill: parent

    ScrollBar.vertical.policy: snap_to_tracker.active? ScrollBar.AlwaysOff : ScrollBar.AlwaysOn


    ListView {
        cacheBuffer: 50000
        id: viewer
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 100
        visible: true
        model: current_part.list_PNG_paths

        property real scale_factor: (viewer.width/3000) * frame.image_scale // 3000 is width

        onContentYChanged: {
            console.log("Content Y Changed:", viewer.contentY);
            //tracker.y = current_part.tracker_info[0].y * viewer.scale_factor - viewer.contentY;
            console.log(current_part.tracker_info)
        }


        Rectangle {
            id: tracker

            x: (current_part.tracker_info[0].x + 50) * viewer.scale_factor
            y: (current_part.tracker_info[0].y - 100) * viewer.scale_factor - (viewer.contentY - viewer.originY)

            height: (current_part.tracker_info[1] + 160) * viewer.scale_factor
            width: 4
            radius: 3
            color: "red"

            onYChanged: {
                if (snap_to_tracker.active) {

                    viewer.positionViewAtBeginning()
                    viewer.contentY += (current_part.tracker_info[0].y - 200) * viewer.scale_factor

                    //viewer.contentY += (current_part.tracker_info[0].y - current_part.tracker_info[4]) * viewer.scale_factor;

                    tracker.y = (current_part.tracker_info[0].y - 100) * viewer.scale_factor - viewer.contentY;

                    tracker.y = Qt.binding(function() {
                        return (current_part.tracker_info[0].y - 100) * viewer.scale_factor - (viewer.contentY - viewer.originY);
                    })
                }
            }
        }


        delegate: Image {
            id: music
            width: parent.width * image_scale
            source: modelData
            //anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            antialiasing: true




            MouseArea {
                id: clicker

                property real x_coords: 0
                property real y_coords: 0

                property var element_data: []

                focus: true

                anchors.fill: parent
                onClicked: {
                    clicker.focus = true

                    clicker.forceActiveFocus()

                    x_coords = (mouse.x/viewer.scale_factor) - 50
                    y_coords = mouse.y/viewer.scale_factor

                    element_data = current_part.element_from_point(Qt.point(x_coords, y_coords), index + 1)
                    console.log(x_coords, y_coords)

                    console.log(element_data)

                    if (element_data.length === 0) {
                        console.log("No element found at point");

                        selection_view.measure_number = 0
                        selection_view.start_beat = 0
                        selection_view.end_beat = 0
                        selection_view.note_name = "None selected"
                        selection_view.x_coords = 0
                        selection_view.y_coords = 0
                        selection_view.page_number = 0
                        selection_view.staff_number = 0
                        selection_view.selected = false

                        return;
                    }

                    selection_view.measure_number = element_data[1]
                    selection_view.start_beat = (element_data[2] + 32) / 32
                    selection_view.end_beat = (element_data[3] + 32) / 32
                    selection_view.note_name = element_data[4]
                    selection_view.x_coords = element_data[5]
                    selection_view.y_coords = element_data[6]
                    selection_view.page_number = element_data[7]
                    selection_view.staff_number = element_data[8]
                    selection_view.selected = true

                }
            }
        }

        footer: Rectangle {
            width: parent.width
            height: frame.height/1.5
            color: "white"
        }
    }
}

