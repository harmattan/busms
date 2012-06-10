import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Rectangle {
    id: applicationHeader
    property alias title: title.text
    property alias titleColor: title.color
    property alias backgroundColor: applicationHeader.color

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    anchors.leftMargin: -UI.DEFAULT_MARGIN
    anchors.rightMargin: -UI.DEFAULT_MARGIN
    anchors.topMargin: -UI.DEFAULT_MARGIN

    height: 70
    z: 99
    Label {
        id: title
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 20
        font.pixelSize: 32
    }
}
