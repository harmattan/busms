/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);
    explicit Settings(const Settings &settings);

public slots:
    void setSetting(const QString &setting, const QVariant &value);
    QVariant getSetting(const QString &setting);
    bool removeSetting(const QString &setting);
    void clear();
    void defaultSettings(const QVariantMap &map);

signals:
    void settingChanged(const QString &setting, const QVariant &value);
};

#endif // SETTINGS_H
