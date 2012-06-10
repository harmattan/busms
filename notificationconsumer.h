#ifndef NOTIFICATIONCONSUMER_H
#define NOTIFICATIONCONSUMER_H

#include <QObject>

class OviNotificationSession;

class NotificationConsumer: public QObject
{
    Q_OBJECT

public:
    explicit NotificationConsumer(QObject *parent=0);
    ~NotificationConsumer() {}

private slots:
    void stateChanged(QObject* aState);
    void received(QObject* aNotification);
    void versionResponse(QString aVersion);
    void notificationInfo(QObject* aData);

private:
    void initialize();
    OviNotificationSession* mNotificationSession;


};

#endif // NOTIFICATIONCONSUMER_H
