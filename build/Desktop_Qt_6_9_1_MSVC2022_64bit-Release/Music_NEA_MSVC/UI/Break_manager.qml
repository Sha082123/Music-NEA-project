import QtQuick 2.15
import QtQuick.Controls

Window {
    id: break_manager

    width: 300
    height: 600

    property string error_message: "";

    Text {
        text: "Message: " + error_message
    }

    Button {
        id: save_button
        text: "Save"

        width: 135
        height: 25

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }
    }

    Button {
        id: cancel_button
        text: "Cancel"

        width: 135
        height: 25

        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: 10
        }
    }

    Button {
        id: new_break
        text: "New Break"

        width: 280
        height: 25

        anchors {
            left: parent.left
            right: parent.right
            bottom: cancel_button.top
            margins: 10
        }



        onClicked: {
            // Handle new break creation
            popup.open()

            console.log("Creating new break");
        }
    }

    Rectangle {
        id:frame

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: 20
            margins: 10
        }

        height: 500

        ScrollView {
            anchors.fill: parent

            ListView {
                id: list_view
                clip: true

                anchors {
                    left: parent.left
                    right:parent.right
                    top: parent.top
                }

                model: parser_data.break_list

                delegate: Rectangle {

                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: 30

                    border.width: 1
                    border.color: "#d1ebd8"

                    // MouseArea {
                    //     anchors.fill: parent
                    //     onEntered: {
                    //         delete_button.visible = true
                    //     }
                    //     onExited: {
                    //         delete_button.visible = false
                    //     }
                    // }

                    Button {
                        id: delete_button
                        visible: true

                        width: 25

                        ColorAnimation {
                            from: "white"
                            to: "red"
                            duration: 200
                        }

                        text: "X"

                        anchors {
                            left: parent.left
                            top: parent.top
                            bottom: parent.bottom
                        }

                        onClicked: {
                            if (parser_data.delete_break_item(modelData[0]) === true) {
                                console.log("Break removed successfully");
                                break_manager.error_message = "Removed from buffer: " + modelData[0];
                            } else {
                                console.log("Failed to remove break");
                                break_manager.error_message = "Failed to remove break";
                            };
                        }

                    }

                    TextInput {
                        id: input_field

                        anchors.centerIn: parent
                        text: modelData[1]

                        onEditingFinished: {
                            if (parser_data.update_break_list(modelData[0], text) === true) {
                                console.log("Break updated successfully");
                                break_manager.error_message = "Added to buffer: measure " + text
                            } else {
                                console.log("Failed to update break");
                                break_manager.error_message = "Failed to update break";
                                text = modelData[1]; // Reset to original value
                            };
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: popup
        anchors.centerIn: parent
        width: 200
        height: 70
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Text {
            id: text_instructions
            text: "Enter measure number: "
            verticalAlignment: Text.AlignVCenter
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }
        }

        TextInput {
            text: "Enter here"
            verticalAlignment: Text.AlignVCenter
            anchors {
                left: text_instructions.right
                top: parent.top
                bottom: parent.bottom
            }

            onEditingFinished: {
                popup.close()
                if (parser_data.update_break_list(("break_at_" + text), text) === true) {
                    console.log("Break added successfully");
                    break_manager.error_message = "Added to buffer: measure " + text
                } else {
                    console.log("Failed to update break");
                    break_manager.error_message = "Invalid input";
                };
            }
        }
    }
}
