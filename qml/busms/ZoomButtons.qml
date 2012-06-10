import QtQuick 1.1
//import com.nokia.meego 1.1

Rectangle {
    width: 50
    height: 100
    color: Qt.rgba(0.8,0.8,0.8,0.5)
    radius: 5
    border {width: 1}
    anchors.margins: 10
    opacity: 0.7
    signal zoomIn
    signal zoomOut

    Column {
        anchors.centerIn: parent
        anchors.margins: 10
        spacing: 0
        Image {
            id: button1
            source: "image://theme/icon-m-common-search"
            smooth: true
            scale: 1.2
            MouseArea {
                anchors.fill: parent
                onClicked: { zoomIn() }
            }
        }
        Image {
            id: button2
            source: "image://theme/icon-m-common-search"
            smooth: true
            scale: 0.8
            MouseArea {
                anchors.fill: parent
                onClicked: { zoomOut() }
            }
        }
    }

}
