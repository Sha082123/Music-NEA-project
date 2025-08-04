import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    property string file_path: ""
    property string current_directory: ""

    id: root
    anchors.fill: parent
    color: "#00000000"

    Menu {
        id: contextMenu
        MenuItem {
            text: "Rename"
            onTriggered: {
                // Only open the dialog and set focus.
                myDialog.open()
                dialogInput.text = ""  // Optionally clear input
            }
        }
        MenuItem {
            text: "Delete"
            onTriggered: {
                file_open.delete_file_or_dir(file_path)
                scan_existing.scan_directory(current_directory)
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onPressed: {
            if (mouse.button === Qt.RightButton) {
                contextMenu.popup(mouse.x, mouse.y)
            }
        }
    }

    Dialog {
        id: myDialog
        modal: true
        contentItem: Column {
            spacing: 8
            TextInput {
                id: dialogInput
                width: 200
                wrapMode: TextInput.Wrap
            }
            Button {
                text: "OK"
                onClicked: {
                    // Only rename after user confirms
                    if (dialogInput.text.length > 0) {
                        file_open.rename_file_or_dir(file_path, dialogInput.text)
                        scan_existing.scan_directory(current_directory)
                        myDialog.close()
                    } else {

                    }


                }
            }
        }
    }
}
