#include "sectoritem.h"
#include <QStyleOptionGraphicsItem>
#include <cmath>
#include <complex>

SectorBaseItem::SectorBaseItem(qreal startAnglePos, qreal bering,
                               qreal stepAngle, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_startAnglePos(startAnglePos),
      m_stepAngle(stepAngle), m_activeWhiteSector(false), m_bering(bering),
      m_pen(QColor(Qt::transparent)) {
  m_pen.setStyle(Qt::DotLine);
  m_pen.setCapStyle(Qt::RoundCap);
}

SectorRect::SectorRect(qreal startAnglePos, float bering, qreal stepAngle,
                       QGraphicsItem *parent)
    : SectorBaseItem(startAnglePos, bering, stepAngle, parent) {
  CreatItem();
}

void SectorRect::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // генирируем сигнал с учетом дальности дискрета
  //    emit PressEvent(Bering(), -event->pos().y() * 2);
  QGraphicsItem::mousePressEvent(event);
}

void SectorRect::CreatItem() {
  QPainterPath path;
  QRectF sRect(StartAnglePos(), -SizeRect(), StepAngle() + 1, SizeRect());
  path.addRect(sRect);
  SetPath(path);

  QLinearGradient *gradientImage =
      new QLinearGradient(sRect.bottomLeft(), sRect.topLeft());
  gradientImage->setColorAt(0, Qt::black);
  gradientImage->setColorAt(1, Qt::black);
  SetGradientImage(gradientImage);

  QLinearGradient gradientWhiteSector(sRect.topRight(), sRect.topLeft());
  gradientWhiteSector.setColorAt(0.05, Qt::black);
  gradientWhiteSector.setColorAt(0.8, Qt::white);
  gradientWhiteSector.setColorAt(1, Qt::black);
  SetGradientWhiteSector(gradientWhiteSector);
}

SectorBaseItem::~SectorBaseItem() { delete m_gradientImage; }

void SectorBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                           QWidget *) {
  painter->setPen(m_pen);
  // painter->setRenderHints(QPainter::Antialiasing |
  // QPainter::SmoothPixmapTransform|QPainter::NonCosmeticDefaultPen);
  if (m_activeWhiteSector) {
    painter->fillPath(GetPath(), m_gradientWhiteSector);
    m_activeWhiteSector = false;
  } else
    painter->fillPath(GetPath(), *m_gradientImage);
}

QPainterPath &SectorBaseItem::GetPath() { return m_path; }

void SectorBaseItem::SetPath(const QPainterPath &path) { m_path = path; }

qreal SectorBaseItem::StepAngle() const { return m_stepAngle; }

void SectorBaseItem::SetStepAngle(const qreal &stepAngle) {
  m_stepAngle = stepAngle;
}

qreal SectorBaseItem::StartAnglePos() const { return m_startAnglePos; }

void SectorBaseItem::SetStartAnglePos(const qreal &startAnglePos) {
  m_startAnglePos = startAnglePos;
}

QLinearGradient SectorBaseItem::GradientWhiteSector() const {
  return m_gradientWhiteSector;
}

void SectorBaseItem::SetGradientWhiteSector(
    const QLinearGradient &gradientWhiteSector) {
  m_gradientWhiteSector = gradientWhiteSector;
}

QLinearGradient *SectorBaseItem::GradientImage() const {
  return m_gradientImage;
}

void SectorBaseItem::SetGradientImage(QLinearGradient *gradientImage) {
  m_gradientImage = gradientImage;
}

QPen SectorBaseItem::Pen() const { return m_pen; }

void SectorBaseItem::SetPen(const QPen &pen) { m_pen = pen; }

size_t SectorBaseItem::CountPoint() const { return m_countPoint; }

void SectorBaseItem::SetCountPoint(const size_t &countPoint) {
  m_countPoint = countPoint;
}

qreal SectorBaseItem::Bering() const { return m_bering; }

void SectorBaseItem::SetBering(qreal bering) { m_bering = bering; }

qreal SectorBaseItem::SizeRect() const { return m_sizeRect; }

void SectorBaseItem::SetSizeRect(const qreal &sizeRect) {
  m_sizeRect = sizeRect;
}

QLinearGradient *SectorBaseItem::GetGradientImage() const {
  return m_gradientImage;
}

void SectorBaseItem::SetActiveWhiteSector(bool activeWhiteSector) {
  m_activeWhiteSector = activeWhiteSector;
}

SectorArc::SectorArc(qreal startAnglePos, qreal bering, qreal stepAngle,
                     QGraphicsItem *parent)
    : SectorBaseItem(startAnglePos, bering, stepAngle, parent) {
  CreatItem();
}

void SectorArc::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  //    // вычисление x
  //    qreal x = event->pos().x() * (CountPoint() / SizeRect());
  //    // вычисление y
  //    qreal y = event->pos().y() * ((CountPoint()) / SizeRect());
  //    qreal range = sqrt(std::pow(x, 2) +
  //                       std::pow(y, 2));
  //    // генирируем сигнал с учетом дальности дискрета
  //    emit PressEvent(Bering(), range);
  //    qreal bering = (std::arg(std::complex<qreal>(static_cast<qreal>(-y),
  //    static_cast<qreal>(x)))) * 180 / (M_PI); if(x < 0)
  //        bering += 360;

  //    emit PressEventReal(bering, range);
  QGraphicsItem::mousePressEvent(event);
}

void SectorArc::CreatItem() {
  const qreal shiftArc = 0.05;
  QPainterPath path;
  QRectF sRect(-SizeRect(), -SizeRect(), 2 * SizeRect(), 2 * SizeRect());
  path.moveTo(sRect.center());
  path.arcTo(sRect, 90 - StartAnglePos(),
             -StepAngle() - shiftArc); // построение по часовой стрелки
  path.lineTo(sRect.center());
  SetPath(path);

  qreal x =
      SizeRect() * std::sin((2.0 * 3.14 * (180.0 - StartAnglePos())) / 360);
  qreal y =
      SizeRect() * std::cos((2.0 * 3.14 * (180.0 - StartAnglePos())) / 360);

  QPointF pointStop = QPointF(x, y);
  QLinearGradient *gradientImage =
      new QLinearGradient(sRect.center(), sRect.center() + pointStop);
  gradientImage->setColorAt(0, Qt::black);
  gradientImage->setColorAt(1, Qt::black);
  SetGradientImage(gradientImage);

  qreal xS =
      SizeRect() *
      std::sin((2 * 3.14 * (180.0 - StartAnglePos() - StepAngle())) / 360);
  qreal yS =
      SizeRect() *
      std::cos((2 * 3.14 * (180.0 - StartAnglePos() - StepAngle())) / 360);
  QPointF pointStart = QPointF(xS, yS);

  QLinearGradient gradientWhiteSector(sRect.center() + pointStart,
                                      sRect.center() + pointStop);
  gradientWhiteSector.setColorAt(0.2, Qt::black);
  gradientWhiteSector.setColorAt(0.50, Qt::white);
  gradientWhiteSector.setColorAt(0.8, Qt::black);
  gradientWhiteSector.setColorAt(1.0, Qt::black);
  SetGradientWhiteSector(gradientWhiteSector);
}

QPainterPath SectorBaseItem::shape() const { return m_path; }

QRectF SectorBaseItem::boundingRect() const { return m_path.boundingRect(); }
