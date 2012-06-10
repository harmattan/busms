#include "stopsmap.h"

#include <QDebug>
#include <QGeoServiceProvider>
#include <QGeoMappingManager>
#include <QGeoCoordinate>
#include <QGeoMapPixmapObject>
#include <QGeoMapCircleObject>
#include <QGraphicsSceneMouseEvent>
#include <QGeoBoundingBox>

#ifdef Q_WS_SIMULATOR
    const QString MARKER("/media/disk/QtSDK/Simulator/Qt/gcc/harmattanthemes/blanco/meegotouch/icons/icon-s-location-picker.png");
    const QString MARKER_SELECTED("/media/disk/QtSDK/Simulator/Qt/gcc/harmattanthemes/blanco/meegotouch/icons/icon-s-location-picker-selected.png");
#else
    const QString MARKER("/usr/share/themes/blanco/meegotouch/icons/icon-s-location-picker.png");
    const QString MARKER_SELECTED("/usr/share/themes/blanco/meegotouch/icons/icon-s-location-picker-selected.png");
#endif

StopsMap::StopsMap() :
    QGraphicsGeoMap(createManager())
{
    QGeoCoordinate c(-33.8909216, 151.1872859);
    setCenter(c);
    setZoomLevel(17);
    setConnectivityMode(QGraphicsGeoMap::HybridMode);
    setAcceptTouchEvents(true);
    m_group = new QGeoMapGroupObject();
    m_group->setProperty("name", "markergroup");
    addMapObject(m_group);
    m_currentPosition = new QGeoMapCircleObject(c, 20);
    m_currentPosition->setPen(QPen(QColor(255,0,0,220)));
    m_currentPosition->setBrush(QBrush(QColor(255,0,0,128)));
    m_currentPosition->setProperty("name", "currentPosition");
    QGeoMapGroupObject* group2 = new QGeoMapGroupObject();
    addMapObject(group2);
    group2->addChildObject(m_currentPosition);
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(100);
    connect(this, SIGNAL(centerChanged(QGeoCoordinate)), m_timer, SLOT(start()));
    connect(this, SIGNAL(zoomLevelChanged(qreal)), m_timer, SLOT(start()));
    connect(m_timer, SIGNAL(timeout()),this, SLOT(updateBoundingBox()));
    emit
}

StopsMap::~StopsMap()
{
}

QGeoMappingManager* StopsMap::createManager()
{
    qDebug() << "INFO: Creating mapping manager";

    QGeoServiceProvider *serviceProvider = new QGeoServiceProvider("osm");
    QGeoMappingManager *mappingManager = serviceProvider->mappingManager();
    if (mappingManager == 0) {
        qDebug() << "WARN: Could not load 'osm' plugin. Falling back to 'nokia' plugin.";
        serviceProvider = new QGeoServiceProvider("nokia");
        mappingManager = serviceProvider->mappingManager();
    }
    return mappingManager;
}

void StopsMap::updateBoundingBox() {
    const QGeoBoundingBox& bb = viewport();
    clearStops();
    emit boundingBoxChanged(bb.topLeft().latitude(), bb.topLeft().longitude(),
                            bb.bottomRight().latitude(), bb.bottomRight().longitude());
}

void StopsMap::setCurrentPosition(double latitude, double longitude) {
    QGeoCoordinate coord(latitude, longitude);
    setCenter(coord);
    m_currentPosition->setCenter(coord);
    m_timer->start();
}

void StopsMap::unselectMarkers() {
    QList<QGeoMapObject*> objects = m_group->childObjects();
    qDebug()<< objects.size();
    for (int i = 0; i < objects.length(); i++) {
        QGeoMapObject *obj = objects[i];
        if (obj->objectName() == "stopMarker") {
            if (obj->type() == QGeoMapObject::PixmapType) {
                QGeoMapPixmapObject* tmp = dynamic_cast<QGeoMapPixmapObject*>(obj);
                tmp->setPixmap(QPixmap(MARKER));
            }
        }
    }
}

void StopsMap::addStop(const QVariantMap& busstop)
{
    QGeoCoordinate coord(busstop.value("latitude").toDouble(),
                         busstop.value("longitude").toDouble());

    QPixmap pixmap;
    pixmap = QPixmap(MARKER);
    QGeoMapPixmapObject *pixMapObject =
            new QGeoMapPixmapObject(coord, QPoint(-16,-32), pixmap);

    pixMapObject->setProperty("index", busstop.value("index").toInt());
    pixMapObject->setObjectName("stopMarker");
    m_group->addChildObject(pixMapObject);
}

void StopsMap::clearStops()
{
    m_group->clearChildObjects();
}

void StopsMap::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

    m_lastPos = event->pos();
}

void StopsMap::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF newPos = event->pos();
    QPointF diff = m_lastPos - newPos;

    if (qAbs(diff.x()) > 30 || qAbs(diff.y()) > 30) {
        return;
    }
    unselectMarkers();
    QList<QGeoMapObject*> objects = mapObjectsAtScreenPosition(m_lastPos);
    for (int i = 0; i < objects.length(); i++) {
        QGeoMapObject *obj = objects[i];
        if (obj->objectName() == "stopMarker") {
            if (obj->type() == QGeoMapObject::PixmapType) {
                QGeoMapPixmapObject* tmp = dynamic_cast<QGeoMapPixmapObject*>(obj);
                tmp->setPixmap(QPixmap(MARKER_SELECTED));
            }
            int index = obj->property("index").toInt();
            emit stopClicked(index);
            break;
        }
    }}

void StopsMap::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

    // Round to int
    QPoint lastPos = event->lastPos().toPoint();
    QPoint pos = event->pos().toPoint();

    int dx = lastPos.x() - pos.x();
    int dy = lastPos.y() - pos.y();

    if (qAbs(dx) > 0 || qAbs(dy) > 0) {
        qDebug() << "panning";
        pan(dx, dy);
    }

}
