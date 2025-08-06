import QtQuick 2.12
import QtQuick.Controls
import QtQuick.Layouts

import "styles"
import "UI"

ApplicationWindow {
    id: file_selector
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")



    C_MenuBar {
        id: menuBar
    }

    TabBar {
        id: bar
        width: parent.width
        anchors.top: menuBar.bottom
        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("FileView")
            onPressed: {
                display_window.focus = true
            }

        }
    }

    StackLayout {

        anchors.top: bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: bar.currentIndex

        File_view {
                id: file_view

                onFileLoaded: (file_path) => {
                    loading.visible = true
                    loading.file_path = file_path
                    display_window.refreshed()
                }
        }



        Display_window {
            id: display_window
            visible: true



            //file_path: file_selector.file_path

            // anchors.left: file_selector.left
            // anchors.top: tab_bar.bottom


        }
    }

    Popup {
        property alias file_path: file_text.text

        id: loading
        visible: false
        modal: true
        width: 300
        height: 200
        anchors.centerIn: parent
        contentItem: Rectangle {
            anchors.centerIn: parent
            width: 200
            height: 100
            color: "white"
            Column {
                anchors.centerIn: parent
                spacing: 10
                Text {
                    text: "File finished loading"
                    font.pointSize: 20
                }
                Text {
                    id: file_text
                    width: 150
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}


