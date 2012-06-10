import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI
import "Functions.js" as F

CustomPage {
    id: msgPage
    tools: commonTools
    anchors.margins: UI.DEFAULT_MARGIN
    pageText: "Favourites"

    ListView {
        width: parent.width
        height: parent.height - headerHeight
        anchors.top: headerBottom
        anchors.left: parent.left
        model: searchModel
        currentIndex: -1

        delegate: StopDelegate { id: stopDel
            onFavouriteChanged: {
                searchModel.queryByList(favouriteStops.data)
            }
        }

    }


    Component.onCompleted: {
        searchModel.queryByList(favouriteStops.data)
    }


}
