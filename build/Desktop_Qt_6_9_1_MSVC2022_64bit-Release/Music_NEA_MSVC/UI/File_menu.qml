import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    property string file_path: ""

    id: root
    anchors.fill: parent
    color: "#00000000"

    Menu {
        id: contextMenu
        MenuItem { text: "Rename" }
        MenuItem { text: "Delete" }
        // Add more MenuItems as needed
    }

    MouseArea {
        anchors.fill: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            if (mouse.button === Qt.RightButton) {
                contextMenu.popup(mouse.x, mouse.y)
            }
        }
    }
}
