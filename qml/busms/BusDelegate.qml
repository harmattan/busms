import QtQuick 1.1
import com.nokia.meego 1.1

import "UIConstants.js" as UI

Item {
    id: root
    width: parent.width
    height: UI.LIST_ITEM_HEIGHT_DEFAULT*0.6
    Rectangle {
        height: 1
        color: "black"
        width: time*(parent.width-20)/35
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        id: busRect
        height: parent.height-5
        width: height*2.5
        color: time === 0 ? "#afffaf" : "#afafaf"
        opacity: timetable ? 0.3: 1.0
        radius: 6
        border {
            color: "#2f2f2f"
            width: 2
        }
        x: time*(parent.width-20)/35

        Text {
            anchors.centerIn: parent
            text: name;
            font.bold: true
            font.pixelSize: UI.FONT_SLARGE
        }
    }
    Text {
        visible: time > 0
        anchors.left: busRect.right
        anchors.verticalCenter: parent.verticalCenter

        text: " "+time+" min"
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: UI.FONT_XXSMALL
    }

}
