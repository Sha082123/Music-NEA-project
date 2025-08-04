import QtQuick 2.15
import QtQuick.VectorImage
import QtQuick.Controls

Rectangle {
    id: frame
    property string file_path: ""
    property string pngDirectory: ""
    property real image_scale: 1.0


    height: parent.height
    width: parent.width
    focus: true


    signal windowOpened()
    signal refreshed()

    border.width: 10

    border.color: focus ? "red" : "black"

    clip: true

    onWindowOpened: {
        console.log("Display window loaded with file path:", frame.file_path)
        frame.pngDirectory = render.output_path
        console.log("Music saved to:", frame.pngDirectory)
        console.log(render.list_PNG_paths)
    }

    onRefreshed: {
        clicker.clicked() // Trigger the click event to refresh the image source
    }

    Rectangle {
        id: toolbar
        height: 30
        width: parent.width
        color: "white"
        border.width: 1
        border.color: "black"
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        Button {
            id: refresh
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "R"

            onClicked: {
                mei_parser.insert_break(1)

                console.log("refreshing...")
                refreshed() // Trigger the click event to refresh the image source
            }
        }

        Button {
            id: zoom_in
            anchors.left: refresh.right
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "+"

            onClicked: {
                console.log("Zooming in")
                image_scale += 0.1
                image_scale = Math.min(image_scale, 1.0) // Prevent scale from going above 1.0
            }
        }

        Button {
            id: zoom_out
            anchors.left: zoom_in.right
            anchors.bottom: parent.bottom
            width: parent.height
            height: parent.height
            text: "-"

            onClicked: {
                console.log("Zooming out")
                image_scale -= 0.1
                image_scale = Math.max(image_scale, 0.1) // Prevent scale from going below 0.1
            }
        }

        Button {
            id: break_manager
            anchors.left: zoom_out.right
            anchors.bottom: parent.bottom
            width: 80
            height: parent.height
            text: "Breaks"

            onClicked: {
                break_window.visible = true
            }
        }
    }

    Rectangle {
        id: left_panel
        height: parent.height
        width: 200
        anchors.left: frame.left
        anchors.top: toolbar.bottom
        anchors.bottom: frame.bottom

        ComboBox {
            id: rehearsal_marks

            editable: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            displayText: "Rehearsal marks"

            model: parser_data.reh_y_coords

            delegate: ItemDelegate {
                text: modelData[1]
                width: parent.width
                onClicked: {
                    console.log("original contentY: ", viewer.contentY)

                    viewer.positionViewAtIndex(modelData[3], ListView.beginning)

                    viewer.contentY += (modelData[2] * viewer.scale_factor)

                    //viewer.contentY = (modelData[2] + viewer.originY) * viewer.scale_factor

                    console.log("changed contentY: ", viewer.contentY)
                    console.log("originY: ", viewer.originY)
                    // console.log(viewer.scale_factor)
                    // console.log(modelData[2])
                    // console.log(viewer.contentY)
                    // console.log(viewer.originY)
                    // jump to rehearsal mark

                    // LOGIC PROBLEM HERE!!! ORIGIN Y IS BEING ANNOYING
                    // RESIZING MAKES JUMPING NOT WORK
                }
            }
        }
    }

    Rectangle {
        id: border
        anchors.top: toolbar.bottom
        anchors.bottom: frame.bottom
        anchors.left: left_panel.right
        width: 8
        color: "black"

        property double old_scale_factor: 0
        property double old_contentY: 0

        MouseArea {

            anchors.fill: parent

            property bool dragging: false
            property real x_original
            property real change_x

            onPressed: {
                dragging = true
                x_original = mouseX

                border.old_scale_factor = viewer.scale_factor
                border.old_contentY = viewer.contentY
            }

            onPositionChanged: {
                // Check distance from press position
                change_x = mouseX - x_original


                if (dragging === true) {
                    // Do your drag logic here
                    //console.log("dragging", change_x)
                    if (change_x < 0) {
                        left_panel.width = Math.max(100, left_panel.width + change_x);
                    } else {
                        left_panel.width = Math.min(700, left_panel.width + change_x);
                    }
                }

                //viewer.contentY += viewer.originY

                console.log("contentY: ", viewer.contentY)
                console.log("originY: ", viewer.originY)
            }

            onReleased: {
                dragging = false

                //viewer.contentY = border.old_contentY * (viewer.scale_factor / border.old_scale_factor)
            }
        }
    }

    Rectangle {
        id: right_panel

        anchors.right: frame.right
        anchors.bottom: frame.bottom
        anchors.top: toolbar.bottom
        anchors.left: border.right

        property double old_scale_factor: 0

        // onWidthChanged: {
        //     old_scale_factor = viewer.scale_factor
        //     console.log("Right panel width changed to:", right_panel.width)
        //     console.log(viewer.contentY)
        //     if (viewer.contentY === undefined || viewer.contentY === null) {
        //         console.log("is null")
        //     } else {

        //         console.log("olde scale factor: " + old_scale_factor)
        //         console.log("current scale factor: "+viewer.scale_factor)
        //         viewer.contentY = viewer.contentY * (viewer.scale_factor/old_scale_factor)
        //     }
        // }

        Rectangle {
            id: key_logger
            focus: true
            Component.onCompleted: focus = true

            anchors.fill: parent
            color: "#00000000"
        }

        ScrollView{

            id: scrollView

            clip: true

            anchors.fill: parent


            ListView {
                id: viewer
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 100
                visible: true
                model: render.list_PNG_paths

                onContentYChanged: {
                    console.log("contentY changed to:", contentY)
                    console.log("originY changed to:", originY)
                }


                property real scale_factor: (viewer.width/3000) * frame.image_scale // 3000 is width

                delegate: Image {
                    id: music
                    width: parent.width * image_scale
                    source: modelData
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    antialiasing: true




                    MouseArea {
                        id: clicker

                        property real x_coords: 0
                        property real y_coords: 0

                        anchors.fill: parent
                        onClicked: {
                            //selection_handler.parse_page_xml(index+1)

                            x_coords = (mouse.x/viewer.scale_factor) - 50
                            y_coords = mouse.y/viewer.scale_factor

                            var point = music.mapToItem(viewer, 0, 0)
                            //console.log("Image top-left relative to ListView:", x_coords, y_coords)
                            //console.log("Image clicked: ", modelData)
                            // console.log(selection_handler.get_element_at_coords(1, clicker.x_coords, clicker.y_coords))
                            selection_handler.get_element_at_coords(index + 1, x_coords, y_coords, modelData)
                            console.log(x_coords, y_coords)

                            music.source = modelData + "/reloader"
                            music.source = modelData


                        }
                    }
                }

                footer: Rectangle {
                    width: parent.width
                    height: frame.height/1.5
                    color: "white"
                }
            }
        }
    }


    Break_manager {
        id: break_window
        visible: false
    }


}
