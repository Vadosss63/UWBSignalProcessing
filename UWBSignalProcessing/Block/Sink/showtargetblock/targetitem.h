#ifndef TARGETITEM_H
#define TARGETITEM_H

#include "target.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QTimer>
#include <functional>

//класс обвода образа цели
class TargetItem : public QGraphicsItem {

public:
  TargetItem(const Target &target);
  ~TargetItem() override = default;

  QPen GetPen() const;
  void SetPen(const QPen &GetPen);

  QRectF boundingRect() const override;

  QColor GetColorItem();
  void SetColorItem(const QColor &colorItem);

  TargetItem *GetItemZoom();

  static void SetMaxAMP(int maxAMP);
  static int GetMaxAMP();

  static void SetMaxAMPdymamic(int max);
  static int GetMaxAMPdymamic();

  double GetCurrentSpeed();

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
  void CreateItem(const std::function<double(double)> &getX,
                  const std::function<double(double)> &getY);

  void ShowZoomItem();
  // инфорсации о цели
  Target m_target;
  // стиль отрисовки
  QPen m_pen;
  // форма отрисовки обьекта
  QPainterPath m_pathItem;
  // область отрисовки данных
  QRectF m_boundingRect = QRectF(0, 0, 3, 3);
  QColor m_colorItem;
  // Элемент для отображения на zoom
  TargetItem *m_zoomItem = nullptr;
  // максимальная заначения амплитуды статических целей
  static int m_maxAMP;
  // максимальная заначения амплитуды динамических целей
  static int m_maxAMPdymamic;
};

#endif // TARGETITEM_H
