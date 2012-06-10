#ifndef STOPSMAP_H
#define STOPSMAP_H

#include <QGraphicsGeoMap>
#include <QGeoCoordinate>
#include <QGeoMapGroupObject>
#include <QGeoMapCircleObject>
#include <QTimer>

QTM_USE_NAMESPACE

class StopsMap : public QGraphicsGeoMap
{
    Q_OBJECT

  public:
      StopsMap();
      ~StopsMap();

  public slots:
      void addStop(const QVariantMap& busstop);
      void clearStops();
      void setCurrentPosition(double latitude, double longitude);
      void updateBoundingBox();

  signals:
      void stopClicked(int index);
      void boundingBoxChanged(double lat1, double lon1,double lat2, double lon2);

  protected:

      void mousePressEvent(QGraphicsSceneMouseEvent* event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

  private:
      static QGeoMappingManager* createManager();
      void unselectMarkers();

      QPointF m_lastPos;
      QGeoMapGroupObject* m_group;
      QGeoMapCircleObject* m_currentPosition;
      QTimer* m_timer;
};

#endif // STOPSMAP_H
