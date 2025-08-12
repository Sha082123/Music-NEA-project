import QtQuick 2.15
import QtQuick.Controls

ScrollView{

    id: scrollView

    property alias viewer: viewer

    clip: true

    anchors.fill: parent


    ListView {
        id: viewer
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 100
        visible: true
        model: current_part.list_PNG_paths

        property real scale_factor: (viewer.width/3000) * frame.image_scale // 3000 is width

        delegate: Image {
            id: music
            width: parent.width * image_scale
            source: modelData
            anchors.horizontalCenter: parent.horizontalCenter
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

                    selection_view.measure_number = element_data[1]
                    selection_view.start_beat = (element_data[2] + 32) / 32
                    selection_view.end_beat = (element_data[3] + 32) / 32
                    selection_view.note_name = element_data[4]
                    selection_view.x_coords = element_data[5]
                    selection_view.y_coords = element_data[6]
                    selection_view.page_number = element_data[7]

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
