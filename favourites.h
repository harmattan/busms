#ifndef FAVOURITES_H
#define FAVOURITES_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include "settings.h"

class Favourites : public QObject
{
    Q_OBJECT
public:
    explicit Favourites(QObject *parent = 0);
    //Favourites(QObject *parent = 0);
    Q_PROPERTY(QStringList data READ get)
    void initSettings(Settings* settings);

signals:
    void changed(const QString& key, QVariant list);
    
public slots:
    void add(const QString& stop);
    void remove(const QString& stop);
    bool contains(const QString& stop);
    const QStringList& get();

private:
    QStringList m_list;

};

#endif // FAVOURITES_H
