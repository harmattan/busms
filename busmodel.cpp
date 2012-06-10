#include "busmodel.h"
#include <QDebug>

Bus::Bus(const QString &name, int time,
         bool status) : m_name(name), m_time(time),
    m_status(status)
{
}

QString Bus::name() const
{
    return m_name;
}

int Bus::time() const
{
    return m_time;
}

bool Bus::status() const
{
    return m_status;
}


BusModel::BusModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TimeRole] = "time";
    roles[StatusRole] = "timetable";
    setRoleNames(roles);
}

void BusModel::addBus(const Bus &Bus)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_Buses << Bus;
    endInsertRows();
}

int BusModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return m_Buses.count();
}

void BusModel::clear(const QModelIndex &parent)
{
    if (m_Buses.size() == 0) return;
    beginRemoveRows(parent, 0, m_Buses.size()-1);
    m_Buses.clear();
    endRemoveRows();
}

QVariant BusModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_Buses.count())
        return QVariant();

    const Bus &Bus = m_Buses[index.row()];
    if (role == NameRole)
        return Bus.name();
    else if (role == TimeRole)
        return Bus.time();
    else if (role == StatusRole)
        return Bus.status();
    return QVariant();
}

void BusModel::updateModel(const QString &message) {
    clear();
    if (!parseMessage(message)) {
        emit errorMessage(message);
        return;
    }
    qDebug() << "emit modelUpdated";
    emit modelUpdated();
}

bool BusModel::parseMessage(const QString &message)
{
    QRegExp rx("^([LM]?\\d{2,3})\\s+.+$");
    QRegExp rxmin("(\\d+)");
    QRegExp rxhead("^\\d{5,7}");
    QStringList items = message.split("\n");
    if (items.size() > 0) {
        qDebug() << items[0];
        if (rxhead.indexIn(items[0]) > -1) {
            m_title = items[0];
            emit titleChanged();
            qDebug() << "headerFound";
        }
    }
    QStringList tmplist;
    QString tmp;
    int minutes = 0;
    foreach (QString line, items) {
        bool status = false;
        if (rx.indexIn(line) > -1) {
            tmplist = line.split(QRegExp("\\s"));
            if (tmplist.size() == 2) {
                tmp = tmplist.at(1);
                if (tmp.endsWith('*')) {
                    status = true;
                }
                if ( tmp.startsWith("DUE")) {
                    minutes = 0;
                } else {
                    if (rxmin.indexIn(tmp) > -1) {
                        minutes = rxmin.cap(1).toInt();
                    }
                }
                qDebug() << tmplist.at(0) << minutes << status;
                Bus bus(tmplist.at(0), minutes, status);
                addBus(bus);
            }
        }
    }
    if (m_Buses.size() > 0 ) {
        return true;
    }
    return false;
}

