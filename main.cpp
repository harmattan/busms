#include <QtGui/QApplication>
#include <QDebug>
#include <QtMessaging/QMessageManager>
#include <QtMessaging/QMessageFilter>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include <QtSql/QSqlQuery>
#include <QCoreApplication>
#include <QDir>

#include "searchmodel.h"
#include "messagehandler.h"
#include "busmodel.h"
#include "favourites.h"
#include "settings.h"
#include "stopsmap.h"
#include <QSystemNetworkInfo>

#ifdef HARMATTAN_BOOSTER
#include <MDeclarativeCache>
#endif


QSqlDatabase initDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "mydb");
    QDir appdir(QCoreApplication::applicationDirPath());
#ifndef Q_WS_SIMULATOR
    appdir.cdUp();
#endif
    QString dbpath;
    QTextStream stream(&dbpath);
    stream << appdir.absolutePath()
           << QDir::separator() << "db"
           << QDir::separator() << "sydneybusstops.db";
    qDebug() << dbpath;
    db.setDatabaseName(dbpath);
    if (!db.open()) {
        qDebug() << "couldn't open database";
    }
    return db;
}

void defaultSettings(Settings& s) {
    QString carrierName = QSystemNetworkInfo::networkName(QSystemNetworkInfo::GsmMode);
    if (carrierName.size() > 0 && carrierName.startsWith("vodafone")) {
        isVodafone = true;
    } else {
        carrierName = QSystemNetworkInfo::networkName(QSystemNetworkInfo::WcdmaMode);
        if (carrierName.size() > 0 && carrierName.startsWith("vodafone")) {
            isVodafone = true;
        }
    }
    QVariantMap qvm;
    qvm["deleteMessage"] = false;
    qvm["favourites"] = QStringList();
    qvm["limit"] = 100;
    qvm["vodaHack"] = isVodafone;
    s.defaultSettings(qvm);
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef HARMATTAN_BOOSTER
    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
#else
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
#endif
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    bool isVodafone = false;
    QSqlDatabase db = initDB();
    Settings settings;
    defaultSettings(settings);
    MessageHandler handler;
    handler.initSettings(&settings);
    BusModel busModel;
    // Debug model
    //busModel.updateModel("204211 Newtown, King St nr Newtown Stat\nM30 DUE\n426 8min*\n428 18min\n423 25min\nTransport Info 131500");
    SearchModel searchModel(db,settings.getSetting("limit").toInt());
    Favourites favs;
    favs.initSettings(&settings);

    QObject::connect(&handler, SIGNAL(messageChanged(QString)),
                     &busModel, SLOT(updateModel(QString)) );
    //QObject::connect(&favs, SIGNAL(changed(QString,QVariant)),
    //                 &settings, SLOT(setSetting(QString,QVariant)) );
    QObject::connect(&settings, SIGNAL(settingChanged(QString,QVariant)),
                     &searchModel, SLOT(setLimit(QString,QVariant)) );
    //QObject::connect(&settings, SIGNAL(settingChanged(QString,QVariant)),
    //                 &handler, SLOT(settingsChanged(QString,QVariant)) );

    QDeclarativeContext *context = viewer.rootContext();
    context->setContextProperty("messaging", &handler);
    context->setContextProperty("busModel", &busModel);
    context->setContextProperty("searchModel", &searchModel);
    context->setContextProperty("favouriteStops", &favs);
    context->setContextProperty("settings", &settings);
#ifdef Q_WS_SIMULATOR
    context->setContextProperty("isSimulator", true);
#else
    context->setContextProperty("isSimulator", false);
#endif

    qmlRegisterType<StopsMap>("busmswidget", 1, 0, "StopMap");
    viewer.setMainQmlFile(QLatin1String("qml/busms/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
