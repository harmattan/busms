import QtQuick 1.1
import com.nokia.meego 1.1


QueryDialog {
    id: qdial
    property string stopName
    titleText: "Send?"
    //message: "The message"
    icon: "image://theme/icon-m-conversation-send-sms"
    acceptButtonText: "Yes"
    rejectButtonText: "No"


    onAccepted: {
        console.log("Sending SMS:", stopName)
        messaging.sendSMS(stopName)
    }
}
