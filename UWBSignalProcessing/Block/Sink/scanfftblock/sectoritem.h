#ifndef SECTORBASEITEM_H
#define SECTORBASEITEM_H

#include <QGraphicsItem>
#include <QLinearGradient>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <math.h>
#include <complex>

// базовый класс для отображения углов сканирования
class SectorBaseItem: public QGraphicsItem
{

public:

    SectorBaseItem(qreal start, qreal count, QGraphicsItem *parent);
    ~SectorBaseItem() override;

    QLinearGradient* GetGradientImage() const;

    qreal Width() const;
    qreal Height() const;

    void SetWidth(qreal width);
    void SetHeight(qreal height);

    size_t CountPoint() const;
    void SetCountPoint(const size_t &CountPoint);

    QPen Pen() const;
    void SetPen(const QPen &Pen);

    QLinearGradient* GradientImage() const;
    void SetGradientImage(QLinearGradient* GradientImage);

    qreal Start() const;
    void SetStart(qreal Start);

    QPainterPath& GetPath();
    void SetPath(const QPainterPath& path);

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    qreal Step() const;

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    virtual void CreatItem() = 0;

private:
    // начало сектора
    qreal m_start;
    // шаг сектора
    qreal m_step;
    QPen m_pen;
    // градиент для отображения
    QLinearGradient* m_gradientImage = nullptr;
    // Путь отображения сектора
    QPainterPath m_path;
    // размер прямоугольника 2*1024 ширина и высота
    qreal m_width = 1024;
    qreal m_height = 1024;

};

class SectorRect: public SectorBaseItem
{

public:
    SectorRect(qreal start, qreal count = 1024, QGraphicsItem *parent = nullptr);

protected:
    void CreatItem() override;

};

#endif // SECTORBASEITEM_H
