import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI
import "Functions.js" as F

Item {
    id: root
    width: parent.width
    height: UI.LIST_ITEM_HEIGHT_DEFAULT
    signal favouriteChanged

    Rectangle {
        height: 2
        width: parent.width
        color: "#ffffff"
        visible: ListView.currentIndex > 1
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("CLIIIIICKED")
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
                text: descr;
                font.pixelSize: UI.FONT_DEFAULT
            }
        }
    }

}
