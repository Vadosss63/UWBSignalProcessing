#include "vrliitem.h"
#include <cmath>

QColor VRLIItem::m_colorItem = QColor(Qt::red);

VRLIItem::VRLIItem(qreal angle, qreal range, qreal amp, qreal v)
    : m_angle(angle), m_range(range), m_v(v), m_amplitude(amp),
      m_pen(QPen(Qt::red, 2)) {
  m_pen.setStyle(Qt::SolidLine);
  m_pen.setCapStyle(Qt::RoundCap);
  qreal size = 20;
  m_boundingRect = QRectF(0, 0, size, size);
  m_pathItem.addRect(m_boundingRect);
  SetPos(range, angle);
}

void VRLIItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  // painter->setFont(QFont ("System", 23));
  // painter->setPen();
  // painter->drawText(m_boundingRect, Qt::AlignCenter,
  // QString::number(m_amplitude));
  painter->setFont(QFont());
  painter->setPen(QPen(GetColorItem(), 2));
  painter->setRenderHints(QPainter::Antialiasing |
                          QPainter::SmoothPixmapTransform);
  painter->drawPath(m_pathItem);
}

void VRLIItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
}

QColor VRLIItem::GetColorItem() { return m_colorItem; }

void VRLIItem::SetColorItem(const QColor &colorItem) {
  m_colorItem = colorItem;
}

qreal VRLIItem::GetV() const { return m_v; }

void VRLIItem::SetV(qreal v) { m_v = v; }

qreal VRLIItem::GetAngle() const { return m_angle; }

void VRLIItem::SetAngle(const qreal &startAnglePos) { m_angle = startAnglePos; }

void VRLIItem::CreatItem(qreal angle, qreal range, qreal v) {
  m_angle = angle;
  m_range = range;
  m_v = v;
  update();
}

QPen VRLIItem::GetPen() const { return m_pen; }

void VRLIItem::SetPen(const QPen &pen) { m_pen = pen; }

qreal VRLIItem::GetRange() const { return m_range; }

void VRLIItem::SetRange(qreal range) { m_range = range; }

QRectF VRLIItem::boundingRect() const { return m_boundingRect; }

void VRLIItem::SetPos(qreal range, qreal angle) {
  double x = (range)*std::sin((angle)*M_PI / 180);
  double y = -(range)*std::cos((angle)*M_PI / 180);

  QGraphicsItem::setPos(x - m_boundingRect.width() / 2,
                        y - m_boundingRect.height() / 2);
}
