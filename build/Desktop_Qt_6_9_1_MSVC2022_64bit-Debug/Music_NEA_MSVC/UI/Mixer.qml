import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {

    signal reset_values()

    // onReset_values: {
    //     mixer_list_view.solo_list = []
    //     mixer_list_view.prevent_mute = false
    //     mute_button.active = false
    //     solo_button.active = false
    //     volume_slider.value = 1
    // }

    ScrollView {

        id: mixer_scroll_view

        anchors.fill: parent


        ListView {

            id: mixer_list_view
            clip: true

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            property var solo_list: []
            property bool prevent_mute: false



            model: track_manager.qml_track_list

            delegate: Rectangle {
                height: 80
                width: parent.width

                anchors.margins: 15

                radius: 5
                border.color: "#ebe1c7"
                border.width: 3

                Text {
                    id: track_name_text

                    text: modelData
                    width: parent.width - 50
                    height: 20
                    elide: Text.ElideRight

                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 10
                }

                Button {
                    id: remove_button

                    text: "X"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 10
                    width: 30
                    height: 30

                    onClicked: {
                        console.log(index)
                        track_manager.delete_track(index)
                    }
                }

                Rectangle {
                    id: mute_button

                    property bool active: track_manager.mute_list[index]

                    color: track_manager.mute_list[index]? "#aad5fa" : "#f2f5f7"
                    radius: 5

                    Text {
                        anchors.centerIn: parent
                        text: "Mute"
                    }

                    anchors.left: parent.left
                    anchors.top: track_name_text.bottom

                    height: 25
                    width: parent.width / 2

                    border.width: 2
                    border.color: "#ced5db"

                    visible: track_manager.prevent_mute ? false : true

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            track_manager.toggle_mute(index)
                        }
                    }
                }

                Rectangle {
                    id: solo_button

                    property bool active: false

                    color: track_manager.solo_list[index]? "#aad5fa" : "#f2f5f7"
                    radius: 5

                    Text {
                        anchors.centerIn: parent
                        text: "Solo"
                    }

                    anchors.left: mute_button.right
                    anchors.top: track_name_text.bottom

                    height: 25
                    width: parent.width / 2

                    border.width: 2
                    border.color: "#ced5db"

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {

                            track_manager.toggle_solo(index)

                            // if (solo_button.active === false) {
                            //     mixer_list_view.solo_list.push(index)
                            //     mixer_list_view.solo_list_refresh()
                            //     mute_button.active = false
                            // } else {
                            //     for (var index1 = 0; index1 < mixer_list_view.solo_list.length; index1++) {
                            //         if (mixer_list_view.solo_list[index1] === index) {
                            //             mixer_list_view.solo_list.splice(index1, 1)
                            //         }
                            //     }
                            //     mixer_list_view.solo_list_refresh()
                            // }

                            // solo_button.active = !solo_button.active
                        }
                    }
                }

                Slider {
                    id: volume_slider

                    anchors.top: solo_button.bottom
                    anchors.left: parent.left
                    anchors.margins: 10
                    from: 0
                    value: track_manager.volume_list[index]
                    to: 1
                    stepSize: 0.01
                    width: parent.width - 20

                    onMoved: {
                        track_manager.set_volume(index, value)
                    }
                }
            }

        }

    }

}
