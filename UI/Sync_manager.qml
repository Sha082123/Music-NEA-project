import QtQuick 2.15
import QtQuick.Controls

Window {
    id: sync_manager

    width: 300
    height: 600

    ScrollView {
        id: scroll_view

        anchors.fill: parent

        ListView {
            id: list_view

            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
                margins: 20
            }

            height: 500

            model: track_manager.qml_sync_points

            delegate: Rectangle {
                width: parent.width
                height: 30

                border.width: 1
                border.color: "#d1ebd8"

                Text {
                    text: "Time: " + modelData[0] + ", Measure: " + modelData[1] + ", Beat: " + modelData[2]
                    anchors.centerIn: parent
                }

                Button {
                    id: delete_button
                    visible: index == 0? false : true

                    width: 25

                    text: "X"

                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                    }

                    onClicked: {

                        track_manager.delete_sync_point(index)
                    }
                }
            }
        }
    }

    Button {
        id: apply_button
        text: "Apply"

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 20
        }

        height: 30

        onClicked: {
            track_manager.apply_sync_points()
            sync_manager.close()
        }
    }
}
