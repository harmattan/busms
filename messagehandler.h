#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QVariant>
#include <QMessageService>
#include <QMessageManager>
#include "settings.h"


class QDBusInterface;

QTM_USE_NAMESPACE

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandler(QObject *parent = 0);

    Q_PROPERTY(QString message READ getMessage NOTIFY messageChanged)
    void initSettings(Settings* settings);

signals:
    void messageChanged(const QString& message);
    void sendStatus(const QString& status);

public slots:
    void sendSMS(const QString& stop);
    void settingsChanged(const QString& key, const QVariant& value);

private slots:
    void stateChanged(QMessageService::State s);
    void messageReceived(const QMessageId&,
                         const QMessageManager::NotificationFilterIdSet&);

private:
    void deleteSent();
    QString getMessage() const { return m_message; }
/*
    void setDelete(bool state) { m_deleteMessage = state; }
    bool getDelete() { return m_deleteMessage; }
*/
    QMessageId m_sendId;
    QMessageManager::NotificationFilterIdSet m_notifFilterSet;
    bool m_deleteMessage;
    QMessageService *m_service;
    QMessageManager m_manager;
    QString m_stop;
    QString m_message;
    bool m_compose;
    QDBusInterface* m_notification;
};

#endif // MESSAGEHANDLER_H
