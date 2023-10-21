#ifndef GRID_H
#define GRID_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>

class SectorGrid : public QObject, public QGraphicsPathItem {
  Q_OBJECT
public:
  SectorGrid(QGraphicsItem *parent = nullptr);
  ~SectorGrid() override = default;

  QRectF GetRect() const;
  void SetRect(const QRectF &GetRect);

  qreal MaxRange() const;
  void SetMaxRange(qreal MaxRange);

  qreal GetMinRange() const;
  void SetMinRange(qreal minRange);

  qreal GetMaxVelocity() const;
  void SetMaxVelocity(qreal velocity);

  const QColor &GetColorGrid() const;
  void SetColorGrid(const QColor &colorGrid);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

  QPainterPath shape() const override;

private:
  void AdjustAxis(double &min, double &max, int &numTicks);
  void CreateAxisX(QPainter *painter, QPainterPath &path);
  void CreateAxisY(QPainter *painter, QPainterPath &path);
  void CreateValue();
  QGraphicsTextItem *m_value = nullptr;
  // максимальная дальность (м)
  qreal m_maxRange = 153.6;
  // минимальная дальность (м)
  qreal m_minRange = 0;
  // максимальная скорость (м/с)
  qreal m_maxVelocity = 15;

  qreal m_width = 1024;
  qreal m_height = 1024;
  // Цвет сетки
  QColor m_colorGrid = QColor("#D0D0D0");
  // Квадрат позиционирования секторов
  QRectF m_rectGrid;
};

#endif // GRID_H
