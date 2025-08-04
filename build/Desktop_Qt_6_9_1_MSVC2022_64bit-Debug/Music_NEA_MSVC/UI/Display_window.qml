import QtQuick 2.15

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("New file")

    property string file_path: ""
    signal windowOpened()

    onWindowOpened: {
        console.log("Display window loaded with file path:", file_path)
        render.openFile(file_path)
    }
}
