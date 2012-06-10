import QtQuick 1.1
import com.nokia.meego 1.1

import "UIConstants.js" as UI
import "Functions.js" as F

CustomPage {
    id: mapPage
    tools: commonTools
    hasHeader: false
    pageText: "Nearby Stops"
    orientationLock: lockInPortrait

    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        platformStyle: BusyIndicatorStyle { size: "large" }
        running: true
    }

    Timer {
        id: delay
        interval: 500
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            var mapView = F.loadObject(mapPage, "MapView.qml", {})
            indicator.running = false
            indicator.opacity = 0

        }
    }

    Component.onCompleted: {
        delay.start()
    }
}
