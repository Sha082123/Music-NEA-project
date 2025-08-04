import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    property alias text: button_text.text
    property int originalWidth: 0;
    property int originalHeight: 0;


    signal clicked()

    id: fileButton
    width: 210
    height: 297
    color: "#c0fcf1"
    radius: 10
    border.width: 2
    border.color: "black"

    Component.onCompleted: {
        originalHeight = fileButton.height;
        originalWidth = fileButton.width;
    }

    Behavior on color {
        ColorAnimation {
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }


    Text {
        id: button_text
        anchors.centerIn: parent
        text: qsTr("Open File")
        font.pointSize: 12
        color: "black"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            fileButton.clicked()
        }

        onEntered: {
            fileButton.color = "#abe0d7"
            fileButton.width += 10
            fileButton.height += 10
        }
        onExited: {
            fileButton.color = "#c0fcf1"
            fileButton.width = originalWidth
            fileButton.height = originalHeight
        }
    }
}
