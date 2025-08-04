import QtQuick 2.15

import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property alias text: button_text.text
    property int originalWidth: 0;
    property int originalHeight: 0;


    signal clicked()

    id: fileButton
    width: 400
    height: 30
    color: "#c0fcf1"
    radius: 0
    border.width: 2
    border.color: "black"
    anchors.centerIn: parent

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
        width: originalWidth
        anchors.centerIn: parent
        font.pointSize: 12
        color: "black"
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
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
