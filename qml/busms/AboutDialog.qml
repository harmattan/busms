import QtQuick 1.1
import com.nokia.meego 1.1


QueryDialog {
    titleText: "BuSMS 0.1"
    message: "<b>(c) Malte Marquarding 2012</b><br>"
             + "The data is provided as is. No guarantees with regards to "
             + "its accuracy. This is not an official application, "
             + "it just uses the 0488898187 service provided by Sydney Buses."
             + "<br>"
             + "<small>This application is free sofware licenced under the GNU Public License version 3</small>"
             + "<br>"
    acceptButtonText: "Close"
    //onLinkActivated: Qt.openUrlExternally(link)
}
