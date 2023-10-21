#ifndef VRLIITEM_H
#define VRLIITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QTimer>
#define M_PI 3.14159265358979323846

//класс обвода образа цели
class VRLIItem : public QGraphicsItem {

public:
  VRLIItem(qreal angle, qreal range, qreal amp, qreal v = 0);
  ~VRLIItem() override = default;

  QPen GetPen() const;
  void SetPen(const QPen &GetPen);

  qreal GetAngle() const;
  void SetAngle(const qreal &startAnglePos);

  qreal GetRange() const;
  void SetRange(qreal range);

  void CreatItem(qreal angle, qreal GetRange, qreal GetV);

  qreal GetV() const;
  void SetV(qreal GetV);

  void SetPos(qreal x, qreal angle);

  QRectF boundingRect() const override;

  static QColor GetColorItem();
  static void SetColorItem(const QColor &colorItem);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
  // пелинг
  qreal m_angle = 0;
  // дистанция
  qreal m_range = 0;
  // скорость
  qreal m_v = 0;
  // значение ЭПР(амплетуда точки в центре масс)
  qreal m_amplitude = 0;
  // стиль отрисовки
  QPen m_pen;
  // форма отрисовки обьекта
  QPainterPath m_pathItem;
  // область отрисовки данных
  QRectF m_boundingRect;

  static QColor m_colorItem;
};

#endif // VRLIITEM_H
