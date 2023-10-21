#include "sectorgriditem.h"
#include <cmath>

SectorGrid::SectorGrid(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
    //    setCacheMode(CacheMode::DeviceCoordinateCache);
    SetRect(QRectF(0, 0, m_width, m_height));
}

void SectorGrid::SetColorGrid(const QColor& colorGrid)
{
    m_colorGrid = colorGrid;
}

const QColor& SectorGrid::GetColorGrid() const
{
    return m_colorGrid;
}

qreal SectorGrid::MaxRange() const
{
    return m_maxRange;
}

void SectorGrid::SetMaxRange(qreal maxRange)
{
    m_maxRange = maxRange;
}

qreal SectorGrid::GetMinRange() const
{
    return m_minRange;
}

void SectorGrid::SetMinRange(qreal minRange)
{
    m_minRange = minRange;
}

qreal SectorGrid::GetMaxVelocity() const
{
    return m_maxVelocity;
}

void SectorGrid::SetMaxVelocity(qreal velocity)
{
    m_maxVelocity = velocity;
}

QRectF SectorGrid::GetRect() const
{
    return m_rectGrid;
}

void SectorGrid::SetRect(const QRectF& rect)
{
    m_rectGrid = rect;
}

QPainterPath SectorGrid::shape() const
{
    QPainterPath path;
    path.addRect(GetRect());
    return path;
}

// Переопределенный метод рисования сетки
void SectorGrid::CreateAxisX(QPainter *painter, QPainterPath& path)
{
    QRectF textTop(0, GetRect().top() - 25, 80, 25);
    QRectF textBottom(0, GetRect().bottom(), 80, 25);

    double minV = - m_maxVelocity;
    double maxV = m_maxVelocity;
    int numTicks = 10;

    AdjustAxis(minV, maxV, numTicks);
    qreal step = m_width / numTicks;

    for (qreal i = 0; i <= numTicks; i ++)
    {
        qreal xPos = i * step;
        QPointF pointStart = QPointF(xPos, GetRect().top());
        path.moveTo(pointStart);
        path.lineTo(QPointF(xPos, GetRect().bottom()));
        textTop.moveLeft(xPos - 40);
        textBottom.moveLeft(xPos - 40);
        QString strText = QString::number(- maxV + ((2 * maxV) / numTicks) * i, 2, 0);
        painter->drawText(textTop, Qt::AlignCenter, strText);
        painter->drawText(textBottom, Qt::AlignCenter, strText);
    }
}

// Переопределенный метод рисования сетки
void SectorGrid::CreateAxisY(QPainter *painter, QPainterPath& path)
{
    QRectF textLeft(GetRect().left() - 80, 0, 80, 25);
    QRectF textRight(GetRect().right(), 0, 80, 25);

    double minRange = m_minRange;
    double maxRange = m_maxRange;
    maxRange = std::floor(maxRange / 10) * 10;
    int numTicks = 10;

    AdjustAxis(minRange, maxRange, numTicks);

    qreal step = m_height / numTicks;

    for (qreal i = 0; i <= numTicks; i ++)
    {
        qreal yPos = m_height - i * step;

        QPointF pointStart = QPointF(GetRect().left(), yPos);
        path.moveTo(pointStart);

        QPointF pointStop = QPointF(GetRect().right(), yPos);
        path.lineTo(pointStop);

        textLeft.moveBottom(yPos);
        textRight.moveBottom(yPos);
        double delta = (maxRange - minRange);
        QString strText = QString::number((delta / numTicks) * i, 2, 0);
        painter->drawText(textLeft, Qt::AlignCenter, strText);
        painter->drawText(textRight, Qt::AlignCenter, strText);
    }
}

void SectorGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;

    painter->save();
    painter->setPen(QPen(GetColorGrid(), 0.5, Qt::DashLine));

    CreateAxisX(painter, path);
    CreateAxisY(painter, path);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(path);
    painter->restore();
}

void SectorGrid::CreateValue()
{
    if(!m_value)
    {
        m_value = new QGraphicsTextItem();
        m_value->setParentItem(this);
        m_value->setPos(GetRect().topRight());
        m_value->setDefaultTextColor(GetColorGrid());
    }
}

void SectorGrid::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    CreateValue();
    qreal x = event->pos().x();
    qreal midle = m_width / 2;
    x = x - midle;
    qreal velocity = (m_maxVelocity / midle) * x;

    qreal y = event->pos().y();
    qreal delta = m_maxRange - m_minRange;
    qreal range = m_maxRange - (delta / m_height) * y;
    m_value->setPlainText(QString("Дальность, м\n%1\nСкорость, м/с\n %2").arg(QString::number(range, 2, 1)).arg(QString::number(velocity, 2, 1)));
}

void SectorGrid::AdjustAxis(double& min, double& max, int& numTicks)
{
    const int minTicks = 4;
    double grossStep = (max - min)/ minTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if(5 * step < grossStep)
        step *= 5;
    else if(3 * step < grossStep )
        step *= 3;
    else if(2 * step < grossStep )
        step *= 2;

    numTicks = int(std::ceil(max / step) - std::floor(min / step));

    if(numTicks < minTicks)
        numTicks = minTicks;

    min = std::floor(min / step) * step;
    max = std::ceil(max / step) * step;
}
