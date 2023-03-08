#include "sectoritem.h"

SectorBaseItem::SectorBaseItem(qreal start, qreal count, QGraphicsItem* parent):
    QGraphicsItem(parent),
    m_start(start), m_pen(QColor(Qt::transparent))
{
    m_step = m_height / count;
    m_pen.setStyle(Qt::DotLine);
    m_pen.setCapStyle(Qt::RoundCap);
}

SectorRect::SectorRect(qreal start, qreal count, QGraphicsItem *parent): SectorBaseItem(start, count, parent)
{
    CreatItem();
}

void SectorRect::CreatItem()
{
    QPainterPath path;
    QRectF rect(0, Start(), Width(), Step());
    path.addRect(rect);
    SetPath(path);
    QLinearGradient* gradientImage = new QLinearGradient(rect.bottomLeft(), rect.bottomRight());
    gradientImage->setColorAt(0, Qt::black);
    gradientImage->setColorAt(0.5, "#F0F0F0");
    gradientImage->setColorAt(1, Qt::black);

    SetGradientImage(gradientImage);
}

SectorBaseItem::~SectorBaseItem()
{
    delete m_gradientImage;
}

void SectorBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
{
    painter->setPen(m_pen);
    painter->fillPath(GetPath(), *m_gradientImage);
}

QPainterPath& SectorBaseItem::GetPath()
{
    return m_path;
}

void SectorBaseItem::SetPath(const QPainterPath& path)
{
    m_path = path;
}

qreal SectorBaseItem::Step() const
{
    return m_step;
}

qreal SectorBaseItem::Start() const
{
    return m_start;
}

void SectorBaseItem::SetStart(qreal startAnglePos)
{
    m_start = startAnglePos;
}

QLinearGradient* SectorBaseItem::GradientImage() const
{
    return m_gradientImage;
}

void SectorBaseItem::SetGradientImage(QLinearGradient *gradientImage)
{
    m_gradientImage = gradientImage;
}

QPen SectorBaseItem::Pen() const
{
    return m_pen;
}

void SectorBaseItem::SetPen(const QPen &pen)
{
    m_pen = pen;
}

qreal SectorBaseItem::Width() const
{
    return m_width;
}

qreal SectorBaseItem::Height() const
{
    return m_height;
}

void SectorBaseItem::SetHeight(qreal height)
{
    m_height = height;
}

void SectorBaseItem::SetWidth(qreal width)
{
    m_width = width;
}

QLinearGradient* SectorBaseItem::GetGradientImage() const
{
    return m_gradientImage;
}

QPainterPath SectorBaseItem::shape() const
{
    return m_path;
}

QRectF SectorBaseItem::boundingRect() const
{
    return m_path.boundingRect();
}
