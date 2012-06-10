import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI


CustomPage {
    id: settingsPage

    pageText: "Settings"


    Row {
        id: row1
        anchors {
            top: headerBottom
            left: parent.left
            right: parent.right
            topMargin: 20
            leftMargin: 10
            rightMargin: 10
        }
        Label {
            width: parent.width - delSwitch.width - parent.spacing
            height: delSwitch.height
            verticalAlignment: Text.AlignVCenter
            text: "Remove messages from conversation"

        }
        Switch {
            id: delSwitch
            checked: settings.getSetting("deleteMessage")

            onCheckedChanged: {
                console.log("CHanGED")
                settings.setSetting("deleteMessage", checked)
            }
        }

    }
    Row {
        id: row2
        anchors {
            top: row1.bottom
            left: parent.left
            right: parent.right
            topMargin: 20
            leftMargin: 10
            rightMargin: 10
        }
        Label {
            width: parent.width - vodaSwitch.width - parent.spacing
            height: delSwitch.height
            verticalAlignment: Text.AlignVCenter
            text: "Vodafone send workaround"

        }
        Switch {
            id: vodaSwitch
            checked: settings.getSetting("vodaHack")
            onCheckedChanged: {
                settings.setSetting("vodaHack", checked)
            }
        }
    }
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop();
        }
    }
}

