import QtQuick 2.15
import QtQuick.Controls

Window {
    id: part_manager_window

    property string error_message: "";



    height: 600
    width: 400

    // Text {
    //     anchors {
    //         top: parent.top
    //         left: parent.left
    //         margins: 25
    //     }

    //     text: "Message: " + part_manager_window.error_message
    // }

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

            model: root_part.part_list

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
            signal emit_error_message(string message)

            onEmit_error_message: {
                part_manager_window.error_message = message;
            }


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
                                break;
                            case -1:
                                console.log("error code -1");
                                part_manager_window.error_message = "Index out of range";
                                break;
                        }
                    }
                }

                Button {
                    id: edit_staff_button

                    visible: index === 0 ? false : true // Hide delete button for main score

                    anchors.right: delete_button.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 10

                    width: 100

                    text: "Manage staves"

                    onClicked: {
                        list_view_1.currentIndex = index
                        console.log("curentIndex updated to: " + list_view_1.currentIndex)
                        staff_editor_loader.active = true
                        staff_editor_loader.item.open()
                    }

                }


                TextInput {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 10
                    text: modelData

                    width: 100
                    readOnly: index === 0 ? true : false // Make main score read-only

                    onEditingFinished: {
                        console.log("Editing finished for part: " + text);
                        console.log("index: " + index)

                        switch(part_manager.update_part(index, text, modelData[index])) {
                            case 0:
                                console.log("Part renamed successfully");
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
            part_manager.create_sync_coordinates()
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


    Loader {
        id: staff_editor_loader
        active: false
        anchors.centerIn: parent
        sourceComponent: staff_editor_component
    }


    Component {
        id: staff_editor_component

        Popup {
            id: staff_editor_popup

            anchors.centerIn: parent
            width: 300
            height: 430
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            property var part_existence: []
            property var original_part_existence: []
            property int number_of_parts: 0

            ListView {

                id: staff_list_view

                anchors {
                    left: parent.left
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: 30
                }

                model: root_part.part_list

                delegate: Rectangle {
                    id: staff_delegate

                    signal refresh_color()


                    onRefresh_color: {
                        color = staff_editor_popup.part_existence[index][1] === true ? "green" : "red"
                    }

                    property bool part_exists: part_manager.staff_exists_in_part(modelData[2],
                                                              part_manager.part_name_list[list_view_1.currentIndex])

                    Component.onCompleted: {
                        staff_editor_popup.part_existence.push([modelData[2], part_exists])
                        staff_editor_popup.original_part_existence = staff_editor_popup.part_existence.map(function(row) { return row.slice(); });
                        console.log("staff_editor_popup.part_existence: " + staff_editor_popup.part_existence)
                        console.log(part_manager.part_name_list[list_view_1.currentIndex])
                        refresh_color()

                        if (part_exists === true) {
                            staff_editor_popup.number_of_parts += 1
                        }
                    }

                    color: staff_editor_popup.part_existence[index][1] === true ? "green" : "red"

                    width: parent.width
                    height: 25

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 10
                        text: modelData[1] + ", Staff no. " + modelData[2]
                        width: 150
                    }

                    // CheckBox {
                    //     checked: part_manager.staff_exists_in_part(modelData[0],
                    //                                                part_manager.part_name_list[list_view_1.currentIndex])
                    //                                               ? "green" : "red"

                    //     onClicked: {

                    //     }
                    // }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {

                            if (staff_editor_popup.number_of_parts <= 1 && staff_editor_popup.part_existence[index][1] === true) {
                                text_message.error_message = "Has to have at least one active staff";

                            } else {
                                staff_editor_popup.part_existence[index][1] = !staff_editor_popup.part_existence[index][1]

                                if (staff_editor_popup.part_existence[index][1] === true) {
                                    staff_editor_popup.number_of_parts += 1
                                } else {
                                    staff_editor_popup.number_of_parts -= 1
                                }
                            }



                            staff_delegate.refresh_color()

                            console.log(staff_editor_popup.part_existence)
                            console.log(staff_editor_popup.number_of_parts)

                        }
                    }
                }
            }

            Text {
                id: text_message

                property string error_message: ""

                text: "Message: " + error_message

                anchors {
                    left: parent.left
                    top: parent.bottom
                    margins: 10
                }
            }

            Button {
                id: close_button

                width: 150
                height: 30

                text: "Close"

                anchors {
                    top: staff_list_view.bottom
                    left: parent.left
                }

                onClicked: {
                    console.log(staff_editor_popup.part_existence)
                    staff_editor_popup.part_existence = staff_editor_popup.original_part_existence.map(function(row) { return row.slice(); });
                    staff_editor_loader.active = false
                }
            }

            Button {
                id: apply_button

                width: 150
                height: 30

                text: "Apply"

                anchors {
                    top: staff_list_view.bottom
                    right: parent.right
                    left: close_button.right
                }

                onClicked: {
                    switch (part_manager.update_part_staves(staff_editor_popup.part_existence, list_view_1.currentIndex)) {
                        case 0:
                            console.log("Staves updated successfully");
                            text_message.error_message = "Updated staves for part: " + part_manager.part_name_list[list_view_1.currentIndex];
                            staff_editor_loader.active = false
                            break;
                        case -1:
                            console.log("error code -1");
                            text_message.error_message = "Has to have at least one active staff";
                            break;
                        case -3:
                            console.log("error code -3");
                            text_message.error_message = "Index out of range";
                            break;
                    }
                }
            }
        }
    }
}
