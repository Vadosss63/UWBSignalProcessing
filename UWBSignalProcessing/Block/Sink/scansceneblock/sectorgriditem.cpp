#include "sectorgriditem.h"
#include <cmath>
#include <complex>
#include <utility>

SectorGridBaseItem::SectorGridBaseItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent) {
  //    setCacheMode(CacheMode::DeviceCoordinateCache);
}

void SectorGridBaseItem::DrawPathMousePressEvent(QPainter *painter) {
  //    painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(QPen(Qt::red, 1));
  painter->drawPath(m_pathMousePressEvent);
}

void SectorGridBaseItem::SetColorGrid(const QColor &colorGrid) {
  m_colorGrid = colorGrid;
}

const QColor &SectorGridBaseItem::GetColorGrid() const { return m_colorGrid; }

float SectorGridBaseItem::MaxRange() const { return m_maxRange; }

void SectorGridBaseItem::SetMaxRange(float maxRange) {
  m_maxRange = maxRange;
  CreateGrid();
}

float SectorGridBaseItem::StopBearing() const { return m_stopBearing; }

float SectorGridBaseItem::StartBearing() const { return m_startBearing; }

void SectorGridBaseItem::SetStartBearing(const float &start) {
  m_startBearing = start;
  CreateGrid();
}

void SectorGridBaseItem::SetStopBearing(const float &stop) {
  m_stopBearing = stop;
  CreateGrid();
}

QRectF SectorGridBaseItem::GetRectPositionSectors() const {
  return m_rectPositionSectors;
}

void SectorGridBaseItem::SetRect(const QRectF &rect) {
  m_rectPositionSectors = rect;
}

QPainterPath &SectorGridBaseItem::PathMousePressEvent() {
  return m_pathMousePressEvent;
}

void SectorGridBaseItem::SetPathMousePressEvent(
    const QPainterPath &pathMousePressEvent) {
  m_pathMousePressEvent = pathMousePressEvent;
}

qreal SectorGridBaseItem::GetSizeRect() const {
  return m_sizeRectPositionSectors;
}

void SectorGridBaseItem::SetSizeRect(const qreal &sizeRect) {
  m_sizeRectPositionSectors = sizeRect;
}

float SectorGridBaseItem::Resolution() const { return m_resolution; }

void SectorGridBaseItem::SetResolution(float resolution) {
  m_resolution = resolution;
  CreateGrid();
}

//////////////////////////////////////////////////////////////////////////////////////////

SectorRectGridItem::SectorRectGridItem(QGraphicsItem *parent)
    : SectorGridBaseItem(parent) {
  SetRect(
      QRectF(-GetSizeRect(), -GetSizeRect(), 2 * GetSizeRect(), GetSizeRect()));
}

// Переопределенный метод рисования сетки
void SectorRectGridItem::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *, QWidget *) {
  painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  float bering = StartBearing();
  float step = (StopBearing() - StartBearing()) / 6;
  //Рисование радиальных линий сетки
  // шаг угла пелинга в сетки углов
  painter->save();
  //    painter->setPen(QPen(QColor("#00A6B"), 1));
  painter->setPen(QPen(Qt::green, 1));

  painter->setFont(QFont("Monospace", 20));
  qreal startAng = 0;
  qreal stopAng = 0;
  qreal stepAngel = 0;
  if (90 <= StopBearing() - StartBearing()) {
    startAng = -GetSizeRect();
    stopAng = GetSizeRect();
    stepAngel = 2 * GetSizeRect() / 6;
  } else {
    //Рисование радиальных линий сетки
    // шаг угла пелинга в сетки углов
    qreal stepBering = 360.0 / (64 * 4);
    // расчет шага углов
    stepAngel = (GetSizeRect() * 2) / ((90) / stepBering);
    startAng = -((StopBearing() - StartBearing()) / stepBering) * stepAngel / 2;
    stopAng =
        startAng + ((StopBearing() - StartBearing()) / stepBering) * stepAngel;
    qreal end;
    for (end = startAng; end <= stopAng; end += stepAngel) {
    }
    stopAng = end;
    float delta = StopBearing() - StartBearing();
    float del;

    if (delta < 5)
      del = 1;
    else if (delta < 10)
      del = 2;
    else if (delta < 20)
      del = 4;
    else if (delta < 30)
      del = 5;
    else
      del = 6;

    step = (StopBearing() - StartBearing()) / del;
    stepAngel = static_cast<int>((stopAng - startAng) / del);
    QRectF rec = GetRectPositionSectors();
    rec.moveLeft(startAng);
    rec.setWidth(stopAng - startAng);
    SetRect(rec);
  }

  QRect text(0, static_cast<int>(GetRectPositionSectors().top()) - 25, 80, 25);
  for (qreal i = startAng; i <= stopAng; i += stepAngel) {
    QPointF pointStart = QPointF(i, GetRectPositionSectors().top());
    path.moveTo(pointStart);
    path.lineTo(QPointF(i, GetRectPositionSectors().bottom()));
    text.moveLeft(static_cast<int>(i - 40));
    painter->drawText(text, Qt::AlignCenter, QString::number(bering, 2, 0));
    bering += step;
  }

  QRect rectText(0, 0, 120, 25);
  //Рисование секторов дальности
  for (qreal i = -GetSizeRect(); i <= 0; i += GetSizeRect() / 5) {
    QPointF pointStart = QPointF(startAng, i);
    path.moveTo(pointStart);
    path.lineTo(QPointF(stopAng, i));
    QString text(QString::number(
        m_maxRange - 2048 * m_resolution - (2 * (i)*m_resolution), 2, 0));
    rectText.moveRight(startAng + 20);
    rectText.moveTop(i);
    painter->drawText(rectText, Qt::AlignCenter, text);
    rectText.moveLeft(stopAng - 20);
    painter->drawText(rectText, Qt::AlignCenter, text);
  }

  painter->restore();
  painter->setPen(QPen(Qt::green, 1));
  painter->setRenderHint(QPainter::Antialiasing);
  painter->drawPath(path);
  DrawPathMousePressEvent(painter);
}

void SectorRectGridItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QPainterPath path;
  // Рисование радиальных линий сетки
  path.moveTo(QPointF(event->pos().x(), GetRectPositionSectors().top()));
  path.lineTo(QPointF(event->pos().x(), GetRectPositionSectors().bottom()));
  //Рисование дуг сетки
  path.moveTo(QPointF(GetRectPositionSectors().left(), event->pos().y()));
  path.lineTo(QPointF(GetRectPositionSectors().right(), event->pos().y()));
  SetPathMousePressEvent(path);
  update();
  QGraphicsPathItem::mousePressEvent(event);
}

QPainterPath SectorRectGridItem::shape() const {
  QPainterPath path;
  path.addRect(GetRectPositionSectors());
  return path;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

SectorRoundGridItem::SectorRoundGridItem(QGraphicsItem *parent)
    : SectorGridBaseItem(parent) {
  SetRect(QRectF(-GetSizeRect(), -GetSizeRect(), 2 * GetSizeRect(),
                 2 * GetSizeRect()));
}

// Переопределенный метод рисования сетки
void SectorRoundGridItem::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *, QWidget *) {
  painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;

  painter->setPen(QPen(Qt::green, 1));
  //Рисование радиальных линий сетки
  for (qreal i = 0; i < 360; i += 15) {
    qreal xS = GetSizeRect() * std::sin((2 * M_PI * (90 + i)) / 360);
    qreal yS = GetSizeRect() * std::cos((2 * M_PI * (90 + i)) / 360);
    QPointF pointStart = QPointF(GetRectPositionSectors().center().rx() + xS,
                                 GetRectPositionSectors().center().ry() + yS);
    path.moveTo(pointStart);
    path.lineTo(GetRectPositionSectors().center());
    painter->save();
    painter->translate(GetRectPositionSectors().center());
    painter->setPen(QPen(Qt::black, 4));
    painter->rotate(i);
    QRect rectText(-50, -(GetSizeRect() + 50), 100, 50);
    painter->setPen(QPen(Qt::green, 1));
    painter->setFont(QFont("Monospace", 24));
    painter->drawText(rectText, Qt::AlignCenter, QString::number(i));
    painter->setFont(QFont());
    painter->restore();
  }
  //Рисование дуг сетки
  for (int i = 0; i < 6; i++) {
    QRectF Rec(GetRectPositionSectors().x() + 150 * i,
               GetRectPositionSectors().y() + 150 * i,
               GetRectPositionSectors().width() - 300 * i,
               GetRectPositionSectors().height() - 300 * i);
    path.arcMoveTo(Rec, 0);
    path.arcTo(Rec, 0, 360);

    painter->setPen(QPen(Qt::green, 1));
    painter->setFont(QFont("Monospace", 24));

    int countNumb;
    if (m_resolution <= 0.3f)
      countNumb = 2;
    else
      countNumb = 1;

    QString textRan(QString::number(
        (static_cast<double>(m_resolution) * (Rec.height())) / 1000, 3,
        countNumb));

    QRect rectTextTop(Rec.center().x() - 50, Rec.top(), 100, 50);
    painter->drawText(rectTextTop, Qt::AlignCenter, textRan);

    QRect rectTextBottom(Rec.center().x() - 50, Rec.bottom() - 50, 100, 50);
    painter->drawText(rectTextBottom, Qt::AlignCenter, textRan);

    QRect rectTextLeft(Rec.left(), Rec.center().y() - 25, 100, 50);
    painter->drawText(rectTextLeft, Qt::AlignCenter, textRan);

    QRect rectTextRight(Rec.right() - 100, Rec.center().y() - 25, 100, 50);
    painter->drawText(rectTextRight, Qt::AlignCenter, textRan);

    painter->setFont(QFont());
    //        painter->restore();
  }
  painter->setRenderHint(QPainter::Antialiasing);
  painter->drawPath(path);
  DrawPathMousePressEvent(painter);
}

void SectorRoundGridItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QPainterPath pathMousePressEvent;
  // Рисование радиальных линий сетки

  if (event->pos().y() < 0) {
    qreal angle = atan(event->pos().x() / event->pos().y());
    qreal xS = GetSizeRect() * std::sin(angle + M_PI);
    qreal yS = GetSizeRect() * std::cos(angle + M_PI);

    QPointF pointStart = QPointF(GetRectPositionSectors().center().rx() + xS,
                                 GetRectPositionSectors().center().ry() + yS);
    pathMousePressEvent.moveTo(pointStart);
    pathMousePressEvent.lineTo(GetRectPositionSectors().center());
  } else {
    qreal angle = atan(event->pos().x() / event->pos().y());
    qreal xS = GetSizeRect() * std::sin(angle);
    qreal yS = GetSizeRect() * std::cos(angle);

    QPointF pointStart = QPointF(GetRectPositionSectors().center().rx() + xS,
                                 GetRectPositionSectors().center().ry() + yS);
    pathMousePressEvent.moveTo(pointStart);
    pathMousePressEvent.lineTo(GetRectPositionSectors().center());
  }

  qreal range =
      std::sqrt(std::pow(event->pos().x(), 2) + std::pow(event->pos().y(), 2));
  //Рисование дуг сетки
  QRectF Rec(GetRectPositionSectors().topLeft().x() + (GetSizeRect() - range),
             GetRectPositionSectors().topLeft().x() + (GetSizeRect() - range),
             2 * range, 2 * range);
  pathMousePressEvent.arcMoveTo(Rec, 0);
  pathMousePressEvent.arcTo(Rec, 0, 360);
  SetPathMousePressEvent(pathMousePressEvent);
  update();
  QGraphicsPathItem::mousePressEvent(event);
}

QPainterPath SectorRoundGridItem::shape() const {
  QPainterPath path;
  path.moveTo(GetRectPositionSectors().center());
  path.arcTo(GetRectPositionSectors(), 0, 360);
  path.lineTo(GetRectPositionSectors().center());
  return path;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
SectorImageItem::SectorImageItem(qreal startAng, qreal stepAng, QColor color,
                                 QGraphicsItem *parent)
    : QGraphicsPathItem(parent), m_color(std::move(color)),
      m_rect(-1024, -1024, 2048, 2048), m_startAng(startAng),
      m_stepAng(stepAng) {}

void SectorImageItem::SetBrush(bool onBrash) { m_brash = onBrash; }

void SectorImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                            QWidget *) {
  if (m_brash)
    painter->setBrush(QBrush(m_color, Qt::Dense4Pattern));
  painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  painter->setPen(QPen(m_color, 5, Qt::DotLine));
  if (m_stepAng == 360) {
    painter->drawEllipse(m_rect);
  } else {
    path.arcTo(m_rect, 90 - m_startAng, -m_stepAng);
    path.lineTo(m_rect.center());
    painter->drawPath(path);
  }
}

QPainterPath SectorImageItem::shape() const {
  QPainterPath path;
  path.addRect(m_rect);
  return path;
}

//////////////////////////////////////////////////////////////////////////////////
SectorArcGridItem::SectorArcGridItem(QGraphicsItem *parent)
    : SectorGridBaseItem(parent) {}

// Переопределенный метод рисования сетки
void SectorArcGridItem::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *, QWidget *) {

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(QPen(GetColorGrid(), 0.5, Qt::DashLine));
  // Рисование радиальных линий сетки
  for (qreal i = 45; i <= 135; i += 15) {
    painter->save();
    painter->translate(GetRectPositionSectors().center());
    painter->rotate((i - 90));
    QFontMetrics fm(painter->font());
    QString number = QString::number(i - 90);
    //        m_rectText.setWidth(fm.width(number));
    //        m_rectText.setHeight(fm.height());
    //        m_rectText.moveCenter(QPoint(0, -(GetSizeRect() +
    //        m_sizeTextRect.height() / 2)));
    painter->drawText(m_rectText, Qt::AlignCenter, number);
    painter->restore();
  }

  //Рисование дуг сетки
  for (int i = 0; i < 6; i++) {
    float posY = GetSizeRect() - 150 * i;
    double x = posY * std::sin((45) * M_PI / 180);
    double y = -posY * std::cos((45) * M_PI / 180);
    QString text = QString::number(Resolution() * posY, 3, 0);
    QRectF textRect(x, y, 50, 50);
    painter->drawText(textRect, Qt::AlignCenter, text);
  }

  if (m_isVisible)
    painter->drawPath(m_path);

  painter->drawRect(boundingRect());
}

void SectorArcGridItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  //    SetPathMousePressEvent(QPainterPath());
  //    // Рисование радиальных линий сетки

  //    qreal angle = atan(event->pos().x() / event->pos().y() ) * 180 / M_PI +
  //    180; qreal xS = GetSizeRect() * std::sin((2 * M_PI * (angle)) / 360);
  //    qreal yS = GetSizeRect() * std::cos((2 * M_PI * ( angle)) / 360);
  //    QPointF pointStart = QPointF(GetRectPositionSectors().center().rx() +
  //    xS, GetRectPositionSectors().center().ry() + yS);
  //    PathMousePressEvent().moveTo(pointStart);
  //    PathMousePressEvent().lineTo(GetRectPositionSectors().center());
  //    qreal range = sqrt(event->pos().x() * event->pos().x() +
  //    event->pos().y() * event->pos().y());
  //    //Рисование дуг сетки
  //    QRectF Rec(GetRectPositionSectors().topLeft().x() +(GetSizeRect() -
  //    range) , GetRectPositionSectors().topLeft().x() + (GetSizeRect() -
  //    range), 2*range, 2*range); PathMousePressEvent().arcMoveTo(Rec, 45);
  //    PathMousePressEvent().arcTo(Rec, 45, 90);
  //    update();

  // вычисление x
  qreal x = event->pos().x() * Resolution();
  // вычисление y
  qreal y = event->pos().y() * Resolution();

  std::complex<qreal> complex(static_cast<qreal>(-y), static_cast<qreal>(x));
  qreal range = std::abs(complex);
  // генирируем сигнал с учетом дальности дискрета
  qreal bering = (std::arg(complex)) * 180 / (M_PI);

  emit SelectPoint(bering, range);
  QGraphicsPathItem::mousePressEvent(event);
}

QPainterPath SectorArcGridItem::shape() const {
  QPainterPath path;
  QRectF rectPath = GetRectPositionSectors();
  path.moveTo(rectPath.center());
  path.arcTo(rectPath, 45, 90);
  path.lineTo(rectPath.center());
  return path;
}

void SectorArcGridItem::CreateGrid() {
  SetRect(QRectF(-GetSizeRect(), -GetSizeRect(), 2 * GetSizeRect(),
                 2 * GetSizeRect()));

  m_sizeTextRect = QSize(50, 25);
  const qreal sizeRect = GetRectPositionSectors().height() / 2;
  // облать отрисовки
  m_boundingRect = QRectF(-sizeRect / std::sqrt(2), -sizeRect,
                          2 * (sizeRect / std::sqrt(2)), sizeRect);

  int magin = m_sizeTextRect.width();
  // добавляем отступ
  m_boundingRect.adjust(-magin, -magin, magin, magin);
  // квадрат отрисовки значений
  m_rectText = QRectF(-m_sizeTextRect.width() / 2,
                      -(GetSizeRect() + m_sizeTextRect.width() / 2),
                      m_sizeTextRect.width(), m_sizeTextRect.height());
  // создаем сетку
  CreatePathGrid();
}

void SectorArcGridItem::SetVisible(bool isVisible) { m_isVisible = isVisible; }

QRectF SectorArcGridItem::boundingRect() const { return m_boundingRect; }

void SectorArcGridItem::CreatePathGrid() {
  m_path = QPainterPath();
  // Рисование радиальных линий сетки
  for (qreal i = 45; i <= 135; i += 15) {
    qreal xS = GetSizeRect() * std::sin((2 * 3.14 * (90 + i)) / 360);
    qreal yS = GetSizeRect() * std::cos((2 * 3.14 * (90 + i)) / 360);
    QPointF pointStart = QPointF(GetRectPositionSectors().center().rx() + xS,
                                 GetRectPositionSectors().center().ry() + yS);
    m_path.moveTo(pointStart);
    m_path.lineTo(GetRectPositionSectors().center());
  }

  //Рисование дуг сетки
  for (int i = 0; i < 6; i++) {
    QRectF Rec(GetRectPositionSectors().x() + 150 * i,
               GetRectPositionSectors().y() + 150 * i,
               GetRectPositionSectors().width() - 300 * i,
               GetRectPositionSectors().height() - 300 * i);
    m_path.arcMoveTo(Rec, 45);
    m_path.arcTo(Rec, 45, 90);
  }
}

void SectorRoundGridItem::CreateGrid() {}
void SectorRectGridItem::CreateGrid() {}
