import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

CustomPage {
    id: searchPage
    tools: commonTools
    pageText: "Search"
    hasHeader: false
    anchors.margins: 0

    Rectangle {
        id: header
        height: 80 // like the Search application
        width: parent.width
        anchors.top: parent.top
        z: 1
        color: UI.COLOR_BACKGROUND

        TextField {
            id: searchField
            anchors { left: parent.left; right: parent.right }
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: UI.DEFAULT_MARGIN
            height: 50

            platformSipAttributes: SipAttributes {
                actionKeyLabel: "Ready"
                actionKeyHighlighted: true
             // actionKeyEnabled: true
            }

            placeholderText: "Search"
            Image {
                id: searchButton
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: searchField.text ? "image://theme/icon-m-input-clear" : "image://theme/icon-m-common-search"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (searchField.text) {
                            searchModel.queryByPattern(searchField.text)
                            searchField.text = ""
                         }
                     }
                 }
             }

             Keys.onPressed: {
                 if (event.key === Qt.Key_Return
                         || event.key === Qt.Key_Enter) {
                     if (searchField.text != "") {
                         searchModel.queryByPattern(searchField.text)

                     }

                 }
             }
         }

    }

    ListView {
        width: parent.width
        height: parent.height - header.height
        anchors.top: header.bottom
        anchors.left: parent.left
        model: searchModel

        delegate: StopDelegate {}
        currentIndex: -1

    }

}
