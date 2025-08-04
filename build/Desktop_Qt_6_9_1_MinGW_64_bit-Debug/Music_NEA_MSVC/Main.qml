import QtQuick
import QtQuick.Controls

import "styles"
import "UI"

Window {
    id: file_selector
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")

    property string file_path: ""

    //create a button which says open file

    FileButton {
        text: qsTr("Open File")
        anchors {
            left: parent.left
            top: parent.top
            margins: 50
        }

        onClicked: {
            file_selector.file_path = file_open.openFile()
            file_selector.close()
            display_window.show()
            display_window.windowOpened();
        }
    }



    Display_window {
        id: display_window
        visible: false

        file_path: file_selector.file_path
    }
}


