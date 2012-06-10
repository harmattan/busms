#include "searchmodel.h"

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QStringList>
#include <QDebug>
#include <qmath.h>


const QString CATEGORY("BusStops");
const QString SELECT("SELECT name, descr, long, lat FROM stops WHERE");


SearchModel::SearchModel(const QSqlDatabase &db,
                         int limit, QObject *parent) :
    QSqlQueryModel(parent),
//    m_landmarkManager(0),
    m_limit(limit)
{
    m_db = db;
    if (!m_db.open()) {
    }
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescRole] = "description";
    roles[LonRole] = "longitude";
    roles[LatRole] = "latitude";
    setRoleNames(roles);
}


QList<double> SearchModel::getBoundingBox(double lon, double lat, double radius) {
    const double rad2deg = 57.2957795;
    double latr = lat/rad2deg;
    double lonr = lon/rad2deg;
    double halfSide = radius;
    const double WGS84_a = 6378137.0;  // Major semiaxis [m]
    const double WGS84_b = 6356752.3; // Minor semiaxis [m]

    //http://en.wikipedia.org/wiki/Earth_radius
    double An = WGS84_a * WGS84_a * cos(latr);
    double Bn = WGS84_b * WGS84_b * sin(latr);
    double Ad = WGS84_a * cos(latr);
    double Bd = WGS84_b * sin(latr);
    double eradius = sqrt( (An*An + Bn*Bn)/(Ad*Ad + Bd*Bd) );
    double pradius = eradius*cos(latr);

    QList<double> fl;
    fl.append(rad2deg*(lonr - halfSide/pradius));
    fl.append(rad2deg*(latr - halfSide/eradius));
    fl.append(rad2deg*(lonr + halfSide/pradius));
    fl.append(rad2deg*(latr + halfSide/eradius));
    return fl;

}

void SearchModel::queryByPosition(double lon, double lat, double radius) {
    QList<double> bbox = getBoundingBox(lon, lat, radius);
    QString query;
    QTextStream stream(&query);
    stream << SELECT
           << " long > " << bbox.at(0)
           << " AND long < " << bbox.at(2)
           << " AND lat > " << bbox.at(1)
           << " AND lat < " << bbox.at(3)
           << "  LIMIT " << m_limit;
    qDebug() << query;
    setQuery(query, m_db);
}

void SearchModel::queryByBox(double lat1, double lon1,
                             double lat2, double lon2) {
    QString query;
    QTextStream stream(&query);
    stream << SELECT
           << " long > " << lon1
           << " AND long < " << lon2
           << " AND lat > " << lat2
           << " AND lat < " << lat1
           << " LIMIT " << m_limit;
    qDebug() << query;
    setQuery(query, m_db);

    for (int i=0; i<rowCount(); ++i) {
        emit stopUpdated(getData(i));
    }
}

void SearchModel::queryByList(const QStringList& stopIds) {
    if (stopIds.isEmpty()) {
        setQuery(SELECT+" name = 1", m_db);
        return;
    }

    QRegExp rx("^2\\d{4,6}$");
    QString tmp;
    foreach (tmp, stopIds) {
        if (rx.indexIn(tmp) == 1) return;
    }
    QString query;
    QTextStream stream(&query);
    stream << SELECT << " name IN (";
    for (int i=0; i < stopIds.length(); ++i) {
        stream << "'" << stopIds.at(i) << "'";
        if (i != stopIds.length()-1 ) {
            stream << ",";
        } else {
            stream << ")";
        }
    }
    stream << "  LIMIT " << m_limit;
    qDebug() << query;
    setQuery(query, m_db);
}


void SearchModel::queryById(const QString& stopId) {
    QString query;
    QTextStream stream(&query);
    stream << SELECT << " name LIKE '"
           << stopId
           << "%' ORDER BY name ASC LIMIT " << m_limit;
    qDebug() << query;
    setQuery(query, m_db);
}

void SearchModel::queryByPattern(const QString& pattern) {
    QRegExp rx("^2\\d{0,6}$");
    if (rx.indexIn(pattern) > -1) {
        queryById(pattern);
        return;
    }
    QStringList illegal;
    illegal.push_back("INSERT");illegal.push_back("DELETE");
    illegal.push_back("TABLE");
    foreach(QString tmp, illegal) {
        if (pattern.contains(tmp,Qt::CaseInsensitive)) {
            return;
        }
    }
    QString query;
    QTextStream stream(&query);
    stream << SELECT << " descr LIKE '%"
           << pattern << "%'"
           <<" OR name LIKE '" << pattern<< "%'"
           << "ORDER BY name ASC LIMIT " << m_limit;
    qDebug() << query;
    setQuery(query, m_db);
}


QVariant SearchModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

void SearchModel::setLimit(const QString &setting, const QVariant &value) {
    if (setting == "limit") {
        m_limit = value.toInt();
    }
}

QVariantMap SearchModel::getData(int index) {
    QVariantMap qvm;
    qvm["index"] = index;
    qvm["name"] = record(index).value("name");
    qvm["longitude"] = record(index).value("long");
    qvm["latitude"] = record(index).value("lat");
    qvm["description"] = record(index).value("descr");
    return qvm;
}
