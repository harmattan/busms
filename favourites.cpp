#include "favourites.h"

#include <QDebug>

const QString KEY = "favourites";

Favourites::Favourites(QObject *parent) :
    QObject(parent)
{
}

const QStringList& Favourites::get() {
    m_list.sort();
    return m_list;
}

void Favourites::add(const QString& stop) {
    if (m_list.contains(stop)) return;
    m_list.push_back(stop);
    changed(KEY, QVariant(m_list));
}

void Favourites::remove(const QString& stop) {
    if (m_list.removeOne(stop)) changed(KEY, QVariant(m_list));
}

bool Favourites::contains(const QString& stop) {
    return m_list.contains(stop);
}
void Favourites::initSettings(Settings* settings) {
    m_list = settings->getSetting(KEY).toStringList();
    connect(this, SIGNAL(changed(QString,QVariant)),
            settings, SLOT(setSetting(QString, QVariant)) );


}
