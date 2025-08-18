import QtQuick 2.15
import QtQuick.Controls

MenuBar {
    id: menuBar

    Menu {
       title: qsTr("&File")
       Action {
              text: qsTr("&Save")

              onTriggered: {
                     if (current_part != null) {
                            current_part.save_file()
                     }
              }

       }
       Action {
              text: qsTr("Save &All")

              onTriggered: {
                     part_manager.save_all()
              }
       }
       MenuSeparator { }
       Action {
              text: qsTr("&Quit")

              onTriggered: {
                     Qt.quit()
              }
       }
    }
}
