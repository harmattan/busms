#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QTimer>


class SearchModel : public QSqlQueryModel
{
    Q_OBJECT

private:
    QList<double> getBoundingBox(double lon, double lat, double radius=0.2);
    QSqlDatabase m_db;
    int m_limit;

public:
    enum BusRoles {
        NameRole = Qt::UserRole + 1,
        DescRole,
        LonRole,
        LatRole
    };
    explicit SearchModel(const QSqlDatabase &db = QSqlDatabase(),
                         int limit=100,QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Q_PROPERTY(int nrows READ rowCount)

signals:
    void stopUpdated(const QVariantMap& stop);

public slots:
    void clear() { QSqlQueryModel::clear(); }
    void queryByList(const QStringList& stopIds);
    void queryById(const QString& stopId);
    void queryByPattern(const QString& pattern);
    void queryByPosition(double lon, double lat,
                         double radius=0.2);
    void queryByBox(double lat1, double lon1, double lat2, double lon2);
    void setLimit(const QString &setting,
                  const QVariant &value);

    QVariantMap getData(int index);

};

#endif // SEARCHMODEL_H
