import QtQuick 2.15
import QtQuick.Controls
import "styles"

ScrollView{
    //color: "black"
    id: file_selector
    width: parent.width
    height: 500

    property string file_path: ""
    property string current_directory: file_open.get_current_dir() + "/UserFiles/project_files/"
    signal fileLoaded(string file_path)

    Component.onCompleted: {
        scan_existing.scan_directory(file_open.get_current_dir() + "/UserFiles/project_files/")
        console.log("Files found:", scan_existing.list_existing_files)
    }

    onCurrent_directoryChanged: {
        console.log("Current directory changed to:", current_directory);
    }


    Column {

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 100

        spacing: 150

        Row { //row for new file, new folder
            height: 297
            anchors.left: parent.left
            anchors.leftMargin: 10
            spacing: 20

            Rectangle {
                width: 210
                height: 297
                color: "#00000000"
                FileButton { //new file button

                    text: "Add a New File"

                    onClicked: {
                        file_selector.file_path = file_open.openNewFile(file_selector.current_directory)
                        console.log("New file selected:", file_selector.file_path)

                        part_manager.clear_parts();
                        track_manager.clear()
                        part_manager.create_root_part("main_score");
                        part_manager.set_current_part(0);
                        current_part.openFile(file_selector.file_path, 0)
                        part_manager.scan_part_directory(modelData)
                        scan_existing.scan_directory(file_selector.current_directory) // Scan the directory for files to update
                        //file_selector.close()
                        //display_window.show()
                        display_window.windowOpened();
                        fileLoaded(file_selector.file_path)
                    }
                }
            }

            Rectangle {
                width: 210
                height: 297
                color: "#00000000"
                FileButton {
                    width: 200

                    text: "New Folder" //new folder button

                    onClicked: {
                        file_open.openNewDirectory(file_selector.current_directory+ "/New Folder/")
                        console.log("New folder created :", file_selector.current_directory + "/New Folder/")
                        scan_existing.scan_directory(file_selector.current_directory)
                    }
                }
            }
        }

        Rectangle { // container for file explorer
            id: file_explorer
            width: (file_selector.width - 300) < 200 ? 200 : (file_selector.width - 300)

            height: 500
            clip: true
            color: "#e0d6b8"

            anchors.left: parent.left
            anchors.leftMargin: 10

            ScrollView{

                anchors.fill: parent

                GridView {
                    id: existing_grid

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 20

                    cellWidth: 120
                    cellHeight: 200




                    model: scan_existing.list_existing_files
                    delegate: Rectangle {
                        color: "#00000000"
                        width: existing_grid.cellWidth - 30
                        height: existing_grid.cellHeight - 30

                        FileButton { //button for each existing file and directory
                            file_path: modelData
                            current_directory: file_selector.current_directory
                            text: file_open.get_base_name(modelData)
                            width: 100
                            height: 100
                            anchors.centerIn: parent


                            Image {
                                id: preview_image

                                property string file_name: file_open.name_from_project_files(modelData)
                                anchors.left: parent.left
                                source: "file:///" + file_open.get_current_dir() + "/UserFiles/previews/" +
                                        file_name + ".png"
                                width: 50
                                fillMode: Image.PreserveAspectFit
                            }

                            onClicked: {
                                if (file_open.isDir(modelData)) {
                                    console.log("Directory clicked")
                                    file_selector.current_directory = modelData
                                    console.log("Current directory:", file_selector.current_directory)
                                    scan_existing.scan_directory(modelData) // Scan the directory for files


                                } else {
                                    file_selector.file_path = modelData

                                    part_manager.clear_parts();
                                    track_manager.clear()
                                    part_manager.create_root_part("main_score")
                                    part_manager.set_current_part(0);
                                    current_part.openFile(modelData, 0)
                                    part_manager.scan_part_directory(modelData)
                                    //file_selector.close()
                                    //display_window.show()
                                    file_selector.fileLoaded(modelData)
                                }
                            }
                        }
                    }
                }
            }

            Row {
                anchors.top: parent.top
                Rectangle {
                    width: 400
                    height: 30
                    PreviousDirectoryButton { //button to go back a directory
                        anchors.fill: parent
                        text: file_selector.current_directory ===
                               scan_existing.get_previous_directory(file_selector.current_directory) ?
                                   "This is the root directory" : "Go to previous directory"
                        onClicked: {
                            file_selector.current_directory = scan_existing.get_previous_directory(file_selector.current_directory)
                            scan_existing.scan_directory(file_selector.current_directory)
                        }
                    }
                }
                Rectangle {
                    width: 1920
                    height: 30
                    color: "white"

                    Text {
                        text: file_selector.current_directory
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 10
                        width: 500
                        wrapMode: Text.Wrap
                    }
                }
            }
        }

        Rectangle{
            color: "#00000000"
            height: 400
            width: 1000
            //anchors.top: file_explorer.bottom
        }
    }
}
