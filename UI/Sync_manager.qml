import QtQuick 2.15
import QtQuick.Controls

Window {

    width: 300
    height: 600

    ScrollView {

        anchors.fill: parent

        ListView {
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }

            model: track_manager.qml_sync_points

            delegate: Rectangle {
                width: parent.width
                height: 30

                Text {
                    text: modelData[0]
                    anchors.centerIn: parent
                }
            }
        }
    }

}
