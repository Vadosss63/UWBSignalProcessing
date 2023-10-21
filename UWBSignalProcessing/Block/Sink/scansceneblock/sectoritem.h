#ifndef SECTORITEM_H
#define SECTORITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QPainter>

// Класс осуществяет отрисовку сектора и является вспомогательным
//для класса ScanSceneDialogBox

// базовый класс для отображения углов сканирования
class SectorBaseItem : public QGraphicsItem {

public:
  SectorBaseItem(qreal StartAnglePos, qreal Bering, qreal StepAngle,
                 QGraphicsItem *parent);
  ~SectorBaseItem() override;

  QLinearGradient *GetGradientImage() const;
  // установка флаго активности
  void SetActiveWhiteSector(bool activeWhiteSector);

  qreal SizeRect() const;
  void SetSizeRect(const qreal &SizeRect);

  qreal Bering() const;
  void SetBering(qreal Bering);

  size_t CountPoint() const;
  void SetCountPoint(const size_t &CountPoint);

  QPen Pen() const;
  void SetPen(const QPen &Pen);

  QLinearGradient *GradientImage() const;
  void SetGradientImage(QLinearGradient *GradientImage);

  QLinearGradient GradientWhiteSector() const;
  void SetGradientWhiteSector(const QLinearGradient &GradientWhiteSector);

  qreal StartAnglePos() const;
  void SetStartAnglePos(const qreal &StartAnglePos);

  qreal StepAngle() const;
  void SetStepAngle(const qreal &StepAngle);

  QPainterPath &GetPath();
  void SetPath(const QPainterPath &path);

  QRectF boundingRect() const override;

  QPainterPath shape() const override;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;
  virtual void CreatItem() = 0;

private:
  // начальная позиция сектора
  qreal m_startAnglePos;
  // шаг сектора (размер)
  qreal m_stepAngle;
  // флаг активности белого сектора
  bool m_activeWhiteSector;
  // градиент отрисовки белого сектора
  QLinearGradient m_gradientWhiteSector;
  // градиент для отображения
  QLinearGradient *m_gradientImage = nullptr;
  // Путь отображения сектора
  QPainterPath m_path;
  // размер прямоугольника 2*1024 ширина и высота
  qreal m_sizeRect = 1024;
  qreal m_bering;
  // при частоте дискретизации 500 МГц
  size_t m_countPoint = 2048;
  QPen m_pen;
};

class SectorArc : public SectorBaseItem {

public:
  SectorArc(qreal StartAnglePos, qreal Bering, qreal StepAngle = 1,
            QGraphicsItem *parent = nullptr);
  ~SectorArc() override = default;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:
  void CreatItem() override;
};

class SectorRect : public SectorBaseItem {

public:
  SectorRect(qreal StartAnglePos, float Bering, qreal StepAngle = 1,
             QGraphicsItem *parent = nullptr);
  ~SectorRect() override = default;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:
  void CreatItem() override;
};

#endif // SECTORITEM_H
