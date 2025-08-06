import QtQuick 2.15
import QtQuick.Controls

Window {
    id: part_manager_window

    property string error_message: "";

    height: 600
    width: 400

    Text {
        anchors {
            top: parent.top
            left: parent.left
            margins: 25
        }

        text: "Message: " + part_manager_window.error_message
    }

    Text {
        text: "Available parts:"

        anchors.bottom: scroll_view.top
        anchors.left: scroll_view.left
    }

    ScrollView {
        id: scroll_view

        height: 200

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 25
            topMargin: 70
        }


        ListView {
            id: list_view
            clip: true
            z: 999

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            model: current_part.part_list

            delegate: Rectangle {

                width: parent.width
                height: 25

                border.width: 1
                color: "#d1ebd8"

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 10
                    text: modelData[1] + ", Staff no. " + modelData[2]
                }
            }
        }
    }

    Text {
        text: "Existing parts:"

        anchors.bottom: scroll_view_1.top
        anchors.left: scroll_view_1.left
    }

    ScrollView {
        id: scroll_view_1

        height: 200

        anchors {
            left: parent.left
            right: parent.right
            top: scroll_view.bottom
            margins: 25
            topMargin: 40
        }

        ListView {
            id: list_view_1
            clip: true

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            property var parts: part_manager.part_name_list


            model: part_manager.part_name_list

            delegate: Rectangle {

                width: parent.width
                height: 25

                border.width: 1
                color: "#d1ebd8"

                Button {
                    visible: index === 0 ? false : true // Hide delete button for main score
                    id: delete_button
                    anchors {
                        right: parent.right
                        top: parent.top
                        bottom: parent.bottom
                    }

                    height: parent.height
                    width: parent.hieght

                    text: "X"

                    onClicked: {
                        console.log("Delete part: " + modelData + " at index: " + index);

                        switch(part_manager.delete_part(index)) {
                            case 0:
                                console.log("Part deleted successfully");
                                part_manager_window.error_message = "Deleted part: " + modelData;
                                break;
                            case -1:
                                console.log("error code -1");
                                part_manager_window.error_message = "Index out of range";
                                break;
                        }
                    }
                }

                TextInput {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 10
                    text: modelData

                    readOnly: index === 0 ? true : false // Make main score read-only

                    onEditingFinished: {
                        console.log("Editing finished for part: " + text);
                        console.log("index: " + index)

                        switch(part_manager.update_part(index, text)) {
                            case 0:
                                console.log("Part renamed successfully");
                                part_manager_window.error_message = "Renamed part to: " + text;
                                break;
                            case -1:
                                console.log("error code -1");
                                part_manager_window.error_message = "Invalid input";
                                text = modelData
                                break;
                            case -2:
                                console.log("error code -2");
                                part_manager_window.error_message = "Part already exists";
                                text = modelData
                                break;
                            case -3:
                                console.log("error code -3");
                                part_manager_window.error_message = "Index out of range";
                                text = modelData
                                break;
                        }
                    }
                }
            }
        }
    }

    Button {
        id: create_part

        text: "Create part"

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 25
        }

        width: 175
        height: 30

        onClicked: {
            popup.open()
        }
    }

    Button {
        id: apply

        text: "Apply"

        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 25
        }

        width: 175
        height: 30

        onClicked: {
            part_manager.apply_part_actions()
        }
    }



    Popup {
        id: popup

        property bool editing_finished_guard: false

        anchors.centerIn: parent
        width: 200
        height: 70
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Text {
            id: text_instructions
            text: "Enter part name: "
            verticalAlignment: Text.AlignVCenter
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }
        }

        TextInput {
            id: text_input
            text: "-"
            verticalAlignment: Text.AlignVCenter
            anchors {
                left: text_instructions.right
                top: parent.top
                bottom: parent.bottom
            }

            onEditingFinished: {

                if (popup.editing_finished_guard === true) {
                    return // prevent multiple calls from focus loss
                }

                popup.editing_finished_guard = true

                console.log("run through")

                switch (part_manager.create_new_part(text)) {
                    case 0:
                        console.log("Part added successfully");
                        part_manager_window.error_message = "Added new part: " + text;
                        popup.close();
                        break;
                    case -1:
                        console.log("error code -1");
                        part_manager_window.error_message = "Invalid input";
                        popup.close();
                        break;
                    case -2:
                        console.log("error code -2");
                        part_manager_window.error_message = "Part already exists";
                        popup.close();
                        break;
                }
            }

            onActiveFocusChanged: {
                if (activeFocus === false) {
                    popup.editing_finished_guard = false; // Reset guard when focus is lost
                }
            }
        }
    }
}
