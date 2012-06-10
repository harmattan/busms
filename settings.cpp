/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "settings.h"
#include <QSettings>
#include <QDebug>

// Constants
const char *Organisation = "Marquarding";
const char *Application = "BuSMS";


Settings::Settings(QObject *parent)
    : QObject(parent)
{
}



Settings::Settings(const Settings &settings)
    : QObject(0)
{
    Q_UNUSED(settings)
}



void Settings::setSetting(const QString &setting, const QVariant &value)
{
    QSettings settings(QSettings::UserScope, Organisation, Application);
    settings.setValue(setting, value);
    qDebug() << setting << value;
    emit settingChanged(setting, value);
}



QVariant Settings::getSetting(const QString &setting) //,
                              //const QVariant &defaultvalue)
{
    QSettings settings(QSettings::UserScope, Organisation, Application);
    /*
    if (!settings.contains(setting)) {
        setSetting(setting, defaultvalue);
        return defaultvalue;
    }
    */
    return settings.value(setting);
}


bool Settings::removeSetting(const QString &setting)
{
    QSettings settings(QSettings::UserScope, Organisation, Application);
    if (!settings.contains(setting)) {
        return false;
    }

    settings.remove(setting);
    return true;
}


void Settings::defaultSettings(const QVariantMap &map) {
    QSettings settings(QSettings::UserScope, Organisation, Application);

    QVariantMap::const_iterator iter = map.constBegin();
    while (iter != map.constEnd()) {
        if (!settings.contains(iter.key())) {
            setSetting(iter.key(), iter.value());
        }
        ++iter;
    }
}

void Settings::clear()
{
    QSettings settings(QSettings::UserScope, Organisation, Application);
    settings.clear();
}
