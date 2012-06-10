import QtQuick 1.1
import com.nokia.meego 1.1
//import com.nokia.extras 1.1

import "Functions.js" as F
import "UIConstants.js" as UI


PageStackWindow {
    id: appWindow

    initialPage: mainPage

    MainPage {
        id: mainPage
    }

    Connections {
        target: busModel
        onModelUpdated: {
            console.log("model update")
            F.loadPage("TimetablePage.qml")
        }
        onErrorMessage: { console.log("error message")
            F.loadPage("TimetablePage.qml", {errorText: message})
        }
    }

    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            id: favourite
            platformIconId: "toolbar-favorite-unmark"
            enabled: pageStack.depth > 1
            onClicked: {
                if (pageStack.depth > 1) {
                    pageStack.pop(mainPage)
                    searchModel.queryByList(favouriteStops.data)
                    search.enabled = true
                    location.enabled = true
                }

            }
            Rectangle {
                anchors.fill: parent
                radius: 20
                opacity: 0.1
                color: UI.COLOR_BUTTON_BACKGROUND
                visible: !parent.enabled
            }

        }
        ToolIcon {
            id: search
            platformIconId: "toolbar-search"
            onClicked: {
                F.loadPage("SearchPage.qml")
                enabled = false
                location.enabled = true
            }
            Rectangle {
                anchors.fill: parent
                radius: 20
                opacity: 0.1
                color: UI.COLOR_BUTTON_BACKGROUND
                visible: !parent.enabled
            }

        }
        ToolIcon {
            id: location
            platformIconId: "common-location"
            onClicked: {
                F.loadPage("MapPage.qml")
                enabled = false
                search.enabled = true
            }
            Rectangle {
                anchors.fill: parent
                radius: 20
                opacity: 0.1
                color: UI.COLOR_BUTTON_BACKGROUND
                visible: !parent.enabled
            }

        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked:
                (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Settings"
                onClicked: F.loadPage("SettingsPage.qml")
            }
            MenuItem {
                text: "About"
                onClicked: F.loadObject(mainPage,
                                           "AboutDialog.qml",
                                           {}).open()
            }
/*
            MenuItem {
                text: "Test"
                onClicked: F.loadPage("TimetablePage.qml")
            }
*/
        }
    }
}
