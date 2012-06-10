#include "notificationconsumer.h"

#include <ovinotificationinterface.h>
#include <ovinotificationsession.h>
#include <ovinotificationstate.h>
#include <ovinotificationinfo.h>
#include <ovinotificationmessage.h>
#include <ovinotificationpayload.h>

NotificationConsumer::NotificationConsumer(QObject* parent)
    : QObject(parent)
{
    initialize();
}

void NotificationConsumer::initialize() {
    QPluginLoader *loader = new QPluginLoader(ONE_PLUGIN_ABSOLUTE_PATH);
    QObject *serviceObject = loader->instance();
    delete loader;

    if (serviceObject)
    {
        // Store the service interface for later usage
        mNotificationSession = static_cast<OviNotificationSession *>(serviceObject);
        mNotificationSession->setNotificationUi(true);
        mNotificationSession->registerApplication("com.marquarding");

        // Connext signals to slots.
        connect(serviceObject,SIGNAL(stateChanged(QObject*)),
                this,SLOT(stateChanged(QObject*)));
        connect(serviceObject,SIGNAL(received(QObject*)),
                this,SLOT(received(QObject*)));
        connect(serviceObject,SIGNAL(notificationInformation(QObject*)),
                this,SLOT(notificationInfo(QObject*)));
        connect(serviceObject,SIGNAL(version(QString)),
                this,SLOT(versionResponse(QString)));
        qDebug() << "REGSTERED";
    }

}

void NotificationConsumer::stateChanged(QObject* aState)
{
    // State of the application has changed
    OviNotificationState* state = static_cast<OviNotificationState*>(aState);

    // Print out the session state on the screen
    switch (state->sessionState())
    {
        case OviNotificationState::EStateOffline:
        {
             qDebug() << "Application is offline.";
            break;
        }
        case OviNotificationState::EStateOnline:
        {
            qDebug() <<"Application is online.";

            //mState = OviNotificationState::EStateOnline;
            break;
        }
        case OviNotificationState::EStateConnecting:
        {
            qDebug() << "Application is connecting.";
            break;
        }
        default:
        {
            break;
        }
    }
    // Print out the error, if there is any
    if (state->sessionError() != OviNotificationState::EErrorNone)
    {
        qDebug() << "Error "  <<state->sessionErrorString() <<  " occured!";
    }
    delete state;
}

void NotificationConsumer::received(QObject* aNotification)
{
    // Casting the QObject to OviNotificationMessage to gain access
    // to all its members.
    OviNotificationMessage* notification =
            static_cast<OviNotificationMessage*>(aNotification);
//#if defined(Q_OS_SYMBIAN)
//    mMessageIdList.append(notification->id());
//#endif

    // Show the received notification in the screen
    OviNotificationPayload*  payload =  static_cast<OviNotificationPayload*>(notification->payload());
    qDebug() << "Notification received!";
    qDebug() << "Application " + notification->senderInformation()
                            + " of the service "+notification->from()+ " service sent: \n"
                            + "'" + payload->dataString() + "'";
    // Printing out the notification details in text console. Optional.
//    ui->textBrowser->append("Notification received.");
//    ui->textBrowser->append("Notification was sent at " + notification->timestamp());
    if (!payload->type().isEmpty())
    {
        qDebug() << "Payload type was set to " + payload->type();
    }
    if (!payload->encoding().isEmpty()) {
        qDebug() << "Payload was encoded in " + payload->encoding();
     }
     delete payload;
     delete notification;

}
void NotificationConsumer::versionResponse(QString aVersion)
{
    qDebug() << "Version :" + aVersion;
}


void NotificationConsumer::notificationInfo(QObject* aData)
{
    // Cast QObject to OviNotificationInfo to access the methods
    OviNotificationInfo* info = static_cast<OviNotificationInfo*>(aData);
    qDebug() << "Notification Id: " + info->notificationId();
    // store NID for later usage
    //mNotificationId = info->notificationId();
    delete info;
}
