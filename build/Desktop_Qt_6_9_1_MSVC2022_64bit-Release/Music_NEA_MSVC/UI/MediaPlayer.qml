import QtQuick 2.15
import QtQuick.Controls

Rectangle {



    width: 150
    anchors {
        top: parent.top
        bottom: parent.bottom
        centerIn: parent
    }



    Button {
        id: play_button

        property bool state: true

        anchors.centerIn: parent
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        text: ">"
        font.bold: true
        font.pointSize: 16

        onClicked: {
            if (state === true) {
                track_manager.play_tracks()
                state = !state
                text = "||"
            } else {
                track_manager.pause_tracks()
                state = !state
                text = ">"
            }

        }
    }

    Button {
        id: back_10s

        property bool state: true


        anchors.right: play_button.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        text: "<"

        onClicked: {
            track_manager.back_10s()
        }
    }

    Button {
        id: forward_10s

        property bool state: true


        anchors.left: play_button.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        text: ">"

        onClicked: {
            track_manager.forward_10s()
        }
    }


    Button {
        id: skip_to_start

        property bool state: true


        anchors.right: back_10s.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        text: "<"

        onClicked: {
            track_manager.back_10s()
        }
    }

    Button {
        id: skip_to_end

        property bool state: true


        anchors.left: forward_10s.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        text: ">"

        onClicked: {
            track_manager.forward_10s()
        }
    }
}
