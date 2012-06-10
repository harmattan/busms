import QtQuick 1.1
import com.nokia.meego 1.1

import "UIConstants.js" as UI
import "Functions.js" as F

Rectangle {
    id: root
    width: parent.width
    height: UI.LIST_ITEM_HEIGHT_DEFAULT-10
    signal favouriteChanged
    radius: 5
    color: root.ListView.isCurrentItem ? "#aabbff" : Qt.rgba(0,0,0,0)

    Rectangle {
        height: 2
        anchors.top: parent.top
        width: parent.width
        color: "#aaaaaa"
        visible: true
    }
    SendDialog { id: sendDialog }
    MouseArea {
        anchors.fill: parent
        onClicked: {
                sendDialog.stopName = name
                sendDialog.open()

        }

    }
    Row {
        ToolIcon {
            iconId: favouriteStops.contains(name) ? "toolbar-favorite-mark":"toolbar-favorite-unmark"
            onClicked: {
                if (favouriteStops.contains(name)) {
                    favouriteStops.remove(name)
                } else {
                    favouriteStops.add(name)
                }
                iconId = F.switchIcon(iconId)
                root.favouriteChanged()
            }

        }
        Column {
            id: col
            Text {
                text: name;
                font.bold: true
                font.pixelSize: UI.FONT_SLARGE
            }
            Text {
                text: description;
                elide: Text.ElideRight
                font.pixelSize: UI.FONT_DEFAULT
            }
        }
    }
}
