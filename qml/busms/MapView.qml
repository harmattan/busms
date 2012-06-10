import QtQuick 1.1
import com.nokia.meego 1.1
import QtMobility.location 1.2

import busmswidget 1.0

Item {
    anchors.fill: parent
    signal mapReady

    ListView {
        id: mapList
        model: searchModel
        delegate: StopDelegate {}
        anchors.bottom: parent.bottom
        height: 200
        width: parent.width
        focus: false
        currentIndex: -1
    }

    StopMap {
        id: map
        anchors.top: parent.top
        anchors.bottom: mapList.top
        width: parent.width

        onBoundingBoxChanged: {
            //console.debug(lat1, lon1, lat2, lon2)
            searchModel.queryByBox(lat1, lon1, lat2, lon2)
            mapList.currentIndex = -1
        }
        onStopClicked: {
            //console.log(index)
            mapList.currentIndex = index
        }
        ZoomButtons {
            id: zoomAction
            anchors.right: map.right
            anchors.top: map.top
            z: 2
            onZoomIn: { map.zoomLevel = map.zoomLevel+1 }
            onZoomOut: { map.zoomLevel = map.zoomLevel-1 }

        }
        Rectangle {
            id:  posUpdate
            anchors.right: map.right
            anchors.bottom: map.bottom
            z: 2
            border { width: 1 }
            width: 50
            height: 50
            color: Qt.rgba(0.8,0.8,0.8,0.5)
            radius: 5
            anchors.margins: 10
            opacity: 0.7
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    indicator.running = true

                    currentPosition.active = true
                }
            }
            Image {
                anchors.centerIn: parent
                anchors.margins: 5
                id: button2
                source: "image://theme/icon-m-toolbar-refresh1"
                smooth: true
            }
        }

    }
    PositionSource {
        id: currentPosition
        updateInterval: 5000
        active: false
        onPositionChanged: {
            if (!isSimulator) {
                map.setCurrentPosition(currentPosition.position.coordinate.latitude,
                                       currentPosition.position.coordinate.longitude)
            }
            mapReady()
            active = false
        }
    }

    Connections {
        target: searchModel
        onStopUpdated: { map.addStop(stop) }
    }
    Timer {
        id: delayPos
        interval: 500
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            currentPosition.active = true
        }
    }
    Component.onCompleted: {
        delayPos.start()
    }
}
