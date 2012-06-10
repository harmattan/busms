#ifndef BUSMODEL_H
#define BUSMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class Bus
{
public:
    Bus(const QString &name, int time,
        bool status=false);

    QString name() const;
    int time() const;
    bool status() const;

private:
    QString m_name;
    int m_time;
    bool m_status;
};

class BusModel : public QAbstractListModel
{
    Q_OBJECT

public:

    Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)
    enum BusRoles {
        NameRole = Qt::UserRole + 1,
        TimeRole,
        StatusRole
    };

    BusModel(QObject *parent = 0);

    void addBus(const Bus &Bus);

    int rowCount(const QModelIndex &parent=QModelIndex()) const;

    QVariant data(const QModelIndex &index,
                  int role=Qt::DisplayRole) const;

    void clear(const QModelIndex &parent=QModelIndex());

signals:
    void errorMessage(const QString& message);
    void modelUpdated();
    void titleChanged();

public slots:
    void updateModel(const QString &message);

private:
    QString getTitle() const { return m_title; }
    bool parseMessage(const QString &message);
    QList<Bus> m_Buses;
    QString m_title;
};
#endif // BusMODEL_H
