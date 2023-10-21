#ifndef SECTORGRIDITEM_H
#define SECTORGRIDITEM_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#define M_PI 3.14159265358979323846

// Класс осуществяет отрисовку размерной сетки и является вспомогательным
// для класса ScanSceneDialogBox
class SectorGridBaseItem : public QObject, public QGraphicsPathItem {
  Q_OBJECT
public:
  SectorGridBaseItem(QGraphicsItem *parent = nullptr);
  ~SectorGridBaseItem() override = default;

  qreal GetSizeRect() const;
  void SetSizeRect(const qreal &GetSizeRect);

  QPainterPath &PathMousePressEvent();
  void SetPathMousePressEvent(const QPainterPath &PathMousePressEvent);

  QRectF GetRectPositionSectors() const;
  void SetRect(const QRectF &GetRectPositionSectors);

  void SetStartBearing(const float &start);
  void SetStopBearing(const float &stop);

  float StartBearing() const;
  float StopBearing() const;
  float Resolution() const;
  void SetResolution(float resolution);
  float MaxRange() const;
  void SetMaxRange(float MaxRange);

  // Функция создания визуального отображения сетки
  virtual void CreateGrid() = 0;

  virtual void SetVisible(bool isVisible) = 0;

  const QColor &GetColorGrid() const;

  void SetColorGrid(const QColor &colorGrid);

signals:
  void SelectPoint(qreal bering, qreal range);

protected:
  // отрисовка лини визирования
  void DrawPathMousePressEvent(QPainter *painter);
  // разрешающая способность (м) 1024 (2048 отсчетов)
  float m_resolution = 0.6f;
  // максимальная дальность (м)
  float m_maxRange = 4000;
  // минимальная дальность (м)
  float m_minRange = 0;

private:
  // размер прямоугольника 2*1024 ширина и высота
  qreal m_sizeRectPositionSectors = 1024;
  QPainterPath m_pathMousePressEvent;
  float m_startBearing = -45;
  float m_stopBearing = 45;
  // Цвет сетки
  QColor m_colorGrid = QColor("#00A86B");
  // Квадрат позиционирования секторов
  QRectF m_rectPositionSectors;
};

////////////
class SectorRectGridItem : public SectorGridBaseItem {
public:
  SectorRectGridItem(QGraphicsItem *parent = nullptr);
  ~SectorRectGridItem() override = default;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  QPainterPath shape() const override;
  void CreateGrid() override;

  QVector<std::pair<QRect, QString>> textRange;
  QVector<std::pair<QRect, QString>> textAngel;
  QPainterPath path;
};
////////////
class SectorRoundGridItem : public SectorGridBaseItem {
public:
  SectorRoundGridItem(QGraphicsItem *parent = nullptr);
  ~SectorRoundGridItem() override = default;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  QPainterPath shape() const override;
  // SectorGridBaseItem interface
  void CreateGrid() override;
};
////////////
class SectorImageItem : public QGraphicsPathItem {
public:
  SectorImageItem(qreal startAng, qreal stepAng, QColor color,
                  QGraphicsItem *parent = nullptr);
  ~SectorImageItem() override = default;
  //включение заливки сектора
  void SetBrush(bool onBrash);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;
  QPainterPath shape() const override;

private:
  // цвет сектора
  QColor m_color;
  // квадрат отрисовки
  QRectF m_rect;
  // стартовый угол отрисовки
  qreal m_startAng;
  // количество углов отрисовки
  qreal m_stepAng;
  // заливка сектора
  bool m_brash = false;
};
////////////
/// \brief The SectorArcGridItem class
///
class SectorArcGridItem : public SectorGridBaseItem {
  Q_OBJECT
public:
  SectorArcGridItem(QGraphicsItem *parent = nullptr);
  ~SectorArcGridItem() override = default;
  void CreateGrid() override;

  void SetVisible(bool isVisible) override;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

  QPainterPath shape() const override;

  QRectF boundingRect() const override;

private:
  void CreatePathGrid();
  // размер прямоугольника для отображения текста
  QSize m_sizeTextRect;

  QRectF m_rectText;

  QRectF m_boundingRect;

  // Путь отрисовки сетки
  QPainterPath m_path;

  bool m_isVisible = true;
};
#endif // SECTORGRIDITEM_H
