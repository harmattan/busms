import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Page {
    id: msgPage
    tools: commonTools
    anchors.margins: UI.DEFAULT_MARGIN

    property bool hasHeader: true
    property alias headerHeight: appHeader.height
    property string pageText: "Default"
    property alias headerBottom: appHeader.bottom

    ApplicationHeader {
         id: appHeader
         title: "BuSMS"
         titleColor: "white"
         backgroundColor: "#34be00"
         visible: hasHeader
     }
    Text {
        anchors.centerIn: parent
        font.pixelSize: UI.FONT_XLARGE
        color: UI.COLOR_SECONDARY_FOREGROUND
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: pageText
        opacity: 0.25
    }
}
