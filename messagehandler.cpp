#include "messagehandler.h"
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QDebug>

const QString SMSTXT = "+61488898287";//"+61421113577";

MessageHandler::MessageHandler(QObject *parent) :
    QObject(parent),
    m_service(0)
{
    m_service =  new QMessageService(this);
    m_notification = \
            new QDBusInterface("com.meego.core.MNotificationManager",
                               "/notificationmanager",
                               "com.meego.core.MNotificationManager",
                               QDBusConnection::sessionBus(),
                               this);

    connect(&m_manager,
            SIGNAL(messageAdded(const QMessageId&,
                                const QMessageManager::NotificationFilterIdSet&)),
            this,
            SLOT(messageReceived(const QMessageId&,
                                 const QMessageManager::NotificationFilterIdSet&)));

    connect(m_service, SIGNAL(stateChanged(QMessageService::State)),
            this, SLOT(stateChanged(QMessageService::State)));

    m_notifFilterSet.insert(m_manager.registerNotificationFilter(
        QMessageFilter::byType(QMessage::Sms) &
        QMessageFilter::bySender(SMSTXT)));
    //qDebug() << QMessageAccount::defaultAccount(QMessage::Sms);

}



void MessageHandler::sendSMS(const QString &stop) {
    m_stop = stop;
    QMessage message;
    message.setType(QMessage::Sms);
    message.setParentAccountId(QMessageAccount::defaultAccount(QMessage::Sms));
    message.setTo(QMessageAddress(QMessageAddress::Phone, SMSTXT));
    message.setBody(stop);
    bool success;
    qDebug() << "compose" << m_compose;
    if (m_compose) {
        success = m_service->compose(message);
    } else {
        success = m_service->send(message);
    }
    if (success) {
      qDebug() << "service.send = true";
    } else {
        qDebug() << "message sending failed";
    }
}

void MessageHandler::deleteSent() {
    // Ugly hack as message.id() doesn't seem to work in harmattan
    QMessageFilter filter(QMessageFilter::byType(QMessage::Sms));
    QMessageFilter filter2(QMessageFilter::byTo(SMSTXT,
                                                QMessageDataComparator::Like));
    QMessageIdList results = m_manager.queryMessages(filter & filter2);
    if (results.length() > 0) {
        m_manager.removeMessage(results.first());
    }
}

void MessageHandler::messageReceived(const QMessageId& id,
          const QMessageManager::NotificationFilterIdSet& matchingFilterIds) {
    if (matchingFilterIds.contains(m_notifFilterSet)) {
        QMessage message = m_manager.message(id);
        QString messageString = message.textContent();
        qDebug() << "message received" << messageString;
        m_message = messageString;
        emit messageChanged(messageString);
        qDebug() << "emit messageChanged called";
        if (m_deleteMessage) {
            deleteSent();
            m_manager.removeMessage(id);

            QDBusReply<bool> reply = m_notification->call("removeNotification",
                                                          0u, 1u);
            if (reply.isValid()) {
                qDebug() << reply.value();
            } else {
                qDebug() << "uh oh" << reply.error().message();
            }

        }
    }
}


void MessageHandler::stateChanged(QMessageService::State newState)
{
    qDebug() << "state changed" << newState;
    QString mstatus("Message sent");

    if (newState == QMessageService::FinishedState) {
        if (m_service->error() == QMessageManager::NoError) {
            qDebug() << "finished state no error";
        } else {
            mstatus = "Message delivery failed";
            if (!m_manager.removeMessage(m_sendId)) {
            }
        }
        emit sendStatus(mstatus);
    }
}

void MessageHandler::settingsChanged(const QString& key,
                                     const QVariant& value) {
    if (key == "deleteMessage") {
        m_deleteMessage = value.toBool();
    } else if (key == "vodaHack") {
        m_compose = value.toBool();
    }
}

void MessageHandler::initSettings(Settings* settings) {
    m_deleteMessage = settings->getSetting("deleteMessage").toBool();
    m_compose = settings->getSetting("vodaHack").toBool();
    connect(settings, SIGNAL(settingChanged(QString,QVariant)),
                     this, SLOT(settingsChanged(QString,QVariant)) );


}

