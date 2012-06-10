import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

CustomPage {
    id: mapPage
    hasHeader: false
    pageText: "Next Buses"
    property string errorText

    Text {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottomMargin: 10
        width: parent.width
        text: busModel.title
        font.pixelSize: UI.FONT_DEFAULT
        font.bold: true
        elide: Text.ElideRight
        color: UI.COLOR_FOREGROUND
    }

    ListView {
        id: busView
        anchors.top: title.bottom
        width: parent.width
        height: parent.height-title.height
        model: busModel
        delegate: BusDelegate {}
        visible: errorText === ""
        spacing: 10
    }
    Text {
        anchors.top: parent.top
        width: parent.width
        height: parent.height-title.height

        color: UI.COLOR_FOREGROUND
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: UI.FONT_LARGE
        text: errorText
        visible: errorText !== ""
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop();
        }
    }
}

