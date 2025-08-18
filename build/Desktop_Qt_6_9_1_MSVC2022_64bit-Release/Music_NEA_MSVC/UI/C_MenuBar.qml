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
                            track_manager.save_playback_states()
                     }
              }

       }
       Action {
              text: qsTr("Save &All")

              onTriggered: {
                     part_manager.save_all()
                     track_manager.save_playback_states()
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
