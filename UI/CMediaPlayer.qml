import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {

    height: 30
    width: 200
    color: "#00000000"

    signal reset_values()
    signal snap_to_time(int time)

    onReset_values: {
        playback.state = true
        play_button.text = ">"
    }


    Rectangle {
        id: playback

        height: 20
        width: 200
        color: "#00000000"

        anchors.top: parent.top

        property bool state: true

        Button {
            id: play_button

            anchors.centerIn: parent
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 40
            text: ">"
            font.bold: true
            font.pointSize: 12

            onClicked: {
                if (playback.state === true) {
                    audio_player.play_audio()
                    playback.state = !playback.state
                    text = "||"
                } else {
                    audio_player.pause_audio()
                    playback.state = !playback.state
                    text = ">"
                }

            }
        }

        Button {
            id: back_10s



            anchors.right: play_button.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 30
            text: "<"

            onClicked: {
                audio_player.back_10s()
            }
        }

        Button {
            id: forward_10s



            anchors.left: play_button.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 30
            text: ">"

            onClicked: {
                audio_player.forward_10s()
            }
        }


        Button {
            id: skip_to_start



            anchors.right: back_10s.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 30
            text: "<<"

            onClicked: {
                audio_player.skip_to_start()
            }
        }

        Button {
            id: skip_to_end



            anchors.left: forward_10s.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 30
            text: ">>"

            onClicked: {
                audio_player.skip_to_end()
                playback.state = !playback.state
                play_button.text = ">"
            }
        }

        Button {
            id: refresh_devices

            anchors.left: skip_to_end.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            text: "Ref"

            onClicked: {
                console.log("Refreshing devices...")
                audio_player.refresh()
            }
        }
    }




    Rectangle {

        anchors.bottom: parent.bottom

        height: 10

        Slider {
            id: slider
            width: 200
            from: 0
            to: audio_player.total_ms
            value: audio_player.qml_position
            stepSize: 1

            onMoved: {
                console.log("Slider value:", value)
                audio_player.set_position(value)
                snap_to_time(value)
            }
        }

    }

}
