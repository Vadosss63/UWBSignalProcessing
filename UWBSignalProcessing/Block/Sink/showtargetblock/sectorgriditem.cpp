#include "sectorgriditem.h"
#include <cmath>

qreal SectorGrid::m_maxVal_YZoom = 30;
qreal SectorGrid::m_minVal_YZoom = 0;
QRectF SectorGrid::m_leftPanel =
        QRectF(-1.5 * SectorGrid::m_widthLeftPanel, 0,
               SectorGrid::m_widthLeftPanel, SectorGrid::m_height);

SectorGrid::SectorGrid(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
    SetRect(QRectF(0, 0, m_width, m_height));
}

void SectorGrid::SetColorGrid(const QColor& colorGrid)
{
    m_colorGrid = colorGrid;
}

void SectorGrid::ShowCurrentSpeed(double speed)
{
    if(!m_valueSpeed){
        m_valueSpeed = new QGraphicsTextItem();
        m_valueSpeed->setParentItem(this);
        m_valueSpeed->setPos(GetRect().topRight().x(), GetRect().topRight().y() + 100);
        m_valueSpeed->setDefaultTextColor(GetColorGrid());
    }
    m_valueSpeed->setPlainText(QString("v = %1 км/ч").arg(QString::number(speed, 2, 1)));

}

bool SectorGrid::ContenesZoom(qreal y)
{
    return (y > m_minVal_YZoom) && (y < m_maxVal_YZoom);
}

const QColor& SectorGrid::GetColorGrid() const
{
    return m_colorGrid;
}

qreal SectorGrid::MaxVal_Y()
{
    return m_maxVal_YZoom;
}

qreal SectorGrid::MinVal_Y()
{
    return m_minVal_YZoom;
}

qreal SectorGrid::MaxVal_X()
{
    return m_maxVal_XZoom;
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
    QRectF shapRect;
    shapRect.setX(-1.5 * m_widthLeftPanel);
    shapRect.setY(0);
    shapRect.setRight(GetRect().right());
    shapRect.setBottom(GetRect().bottom());
    path.addRect(shapRect);
    return path;
}

qreal SectorGrid::Val_XToPixel(double val)
{
    constexpr double middel = (m_width / 2);
    return  middel + (middel / m_maxVal_XZoom) * val;
}

qreal SectorGrid::Val_YToPixel(double val)
{
    val = val - m_minVal_YZoom;
    return m_height - ((m_height) / (m_maxVal_YZoom - m_minVal_YZoom)) * val;
}

qreal SectorGrid::Val_XToPixelLeftPanel(double val)
{
    double middel = (m_widthLeftPanel / 2);
    return m_leftPanel.left() + middel + (middel / m_maxVal_XPanel) * val;
}

qreal SectorGrid::Val_YToPixelLeftPanel(double val)
{
    return m_height - ((m_height) / (m_maxVal_YPanel)) * val;
}

// Переопределенный метод рисования сетки
void SectorGrid::CreateAxisX(QPainter *painter, QPainterPath& path)
{
    QRectF textTop(0, GetRect().top() - 25, 80, 25);
    QRectF textBottom(0, GetRect().bottom(), 80, 25);
    double maxVal_X = m_maxVal_XZoom;
    int numTicks = 6;
    qreal step = m_width / numTicks;

    for (qreal i = 0; i <= numTicks; i ++)
    {
        qreal xPos = i * step;
        QPointF pointStart = QPointF(xPos, GetRect().top());
        path.moveTo(pointStart);
        path.lineTo(QPointF(xPos, GetRect().bottom()));
        textTop.moveLeft(xPos - 40);
        textBottom.moveLeft(xPos - 40);
        QString strText = QString::number(- maxVal_X + ((2 * maxVal_X) / numTicks) * i, 2, 0);
        painter->drawText(textTop, Qt::AlignCenter, strText);
        painter->drawText(textBottom, Qt::AlignCenter, strText);
    }
}

// Переопределенный метод рисования сетки
void SectorGrid::CreateAxisY(QPainter *painter, QPainterPath& path)
{
    QRectF textLeft(GetRect().left() - 80, 0, 80, 25);
    QRectF textRight(GetRect().right(), 0, 80, 25);

    double minRange = m_minVal_YZoom;
    double maxRange = m_maxVal_YZoom;

    qreal step = m_height / m_numTicks;

    for (qreal i = 0; i <= m_numTicks; i ++)
    {
        qreal yPos = m_height - i * step;

        QPointF pointStart = QPointF(GetRect().left(), yPos);
        path.moveTo(pointStart);

        QPointF pointStop = QPointF(GetRect().right(), yPos);
        path.lineTo(pointStop);

        textLeft.moveBottom(yPos);
        textRight.moveBottom(yPos);
        double delta = (maxRange - minRange);
        QString strText = QString::number(minRange + (delta / m_numTicks) * i, 2, 0);
        painter->drawText(textLeft, Qt::AlignCenter, strText);
        painter->drawText(textRight, Qt::AlignCenter, strText);
    }
}

void SectorGrid::CreateDangerTrack(double width, QPainter *painter)
{
    qreal xPos = Val_XToPixel(width / 2);
    QPointF pointStart = QPointF(xPos, GetRect().top());
    QPointF pointStop =  QPointF(xPos, GetRect().bottom());
    painter->drawLine(pointStart, pointStop);
    pointStart.setX(Val_XToPixel(- width / 2));
    pointStop.setX(Val_XToPixel(- width / 2));
    painter->drawLine(pointStart, pointStop);
}

void SectorGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;

    painter->save();
    painter->setPen(QPen(GetColorGrid(), 0.5, Qt::DashLine));

    CreateAxisX(painter, path);
    CreateAxisY(painter, path);
    CreateLeftPanel();
    DrawLeftPanel(painter);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawPath(path);
    painter->restore();
    painter->save();
    painter->setPen(QPen(Qt::red, 0.5, Qt::DashLine));
    CreateDangerTrack(3, painter);
    painter->restore();
    painter->setPen(QPen(Qt::darkGreen, 0.5, Qt::DashLine));
    painter->drawPath(CreateZoneLook());
    painter->setPen(QPen(Qt::blue, 0.5, Qt::DashLine));
    painter->drawPath(CreateNearZoneLook());

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

void SectorGrid::DrawLeftPanel(QPainter* painter)
{
    painter->drawRect(m_leftPanel);

    QRectF textLeft(m_leftPanel.left() - 80, 0, 80, 25);

    double minRange = 0;
    double maxRange = 150;

    qreal step = m_height / m_numTicks;
    QPainterPath path;
    for (qreal i = 0; i <= m_numTicks; i ++)
    {
        qreal yPos = m_height - i * step;
        QPointF pointStart = QPointF(m_leftPanel.left(), yPos);
        path.moveTo(pointStart);
        QPointF pointStop = QPointF(m_leftPanel.right(), yPos);
        path.lineTo(pointStop);
        textLeft.moveBottom(yPos);
        double delta = (maxRange - minRange);
        QString strText = QString::number(minRange + (delta / m_numTicks) * i, 2, 0);
        painter->drawText(textLeft, Qt::AlignCenter, strText);
    }
    painter->drawPath(path);
    QPointF pointStart = QPointF(m_leftPanel.center().x(), m_leftPanel.top());
    QPointF pointStop = QPointF(m_leftPanel.center().x(), m_leftPanel.bottom());
    painter->drawLine(pointStart, pointStop);
    textLeft.moveTo(m_leftPanel.left() - 40, m_leftPanel.bottom() + 10);
    painter->drawText(textLeft, Qt::AlignCenter, "-15");
    textLeft.moveTo(m_leftPanel.center().x() - 40, m_leftPanel.bottom() + 10);
    painter->drawText(textLeft, Qt::AlignCenter, "0");
    textLeft.moveTo(m_leftPanel.right() - 40, m_leftPanel.bottom() + 10);
    painter->drawText(textLeft, Qt::AlignCenter, "15");

    QRectF showZoomRect;
    showZoomRect.setX(Val_XToPixelLeftPanel(-m_maxVal_XPanel));
    showZoomRect.setY(Val_YToPixelLeftPanel(m_maxVal_YZoom));
    showZoomRect.setRight(Val_XToPixelLeftPanel(m_maxVal_XPanel));
    showZoomRect.setBottom(Val_YToPixelLeftPanel(m_minVal_YZoom));
    painter->save();
    painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));

    painter->drawRect(showZoomRect);
    painter->restore();
}

void SectorGrid::CreateLeftPanel(){}

QPainterPath SectorGrid::CreateZoneLook()
{
    return GetZoneLook(std::sin(m_maxAngel * M_PI / 180));
}

QPainterPath SectorGrid::CreateNearZoneLook()
{
    return GetZoneLook(std::sin(4* m_maxAngel * M_PI / 180));
}

QPainterPath SectorGrid::GetZoneLook(const double constVal) const
{
    QPointF leftTopPoint;
    if(- m_maxVal_YZoom * constVal > -m_maxVal_XZoom)
        leftTopPoint = QPointF(- m_maxVal_YZoom * constVal, m_maxVal_YZoom);
    else
        leftTopPoint = QPointF (-m_maxVal_XZoom, m_maxVal_XZoom/constVal);

    QPointF leftBottomPoint(- m_minVal_YZoom * constVal, m_minVal_YZoom);

    QPointF rightBottomPoint(m_minVal_YZoom * constVal, m_minVal_YZoom);

    QPointF rightTopPoint( m_minVal_YZoom * constVal, m_minVal_YZoom);
    if(m_maxVal_YZoom * constVal < m_maxVal_XZoom)
        rightTopPoint = QPointF(m_maxVal_YZoom * constVal, m_maxVal_YZoom);
    else
        rightTopPoint = QPointF(m_maxVal_XZoom, m_maxVal_XZoom / constVal);

    if(std::abs(leftTopPoint.x()) > m_maxVal_XZoom)
        leftTopPoint.setX(-m_maxVal_XZoom);

    QPainterPath path;
    path.moveTo(Val_XToPixel(leftTopPoint.x()), Val_YToPixel(leftTopPoint.y()));
    path.lineTo(Val_XToPixel(leftBottomPoint.x()), Val_YToPixel(leftBottomPoint.y()));
    path.moveTo(Val_XToPixel(rightBottomPoint.x()), Val_YToPixel(rightBottomPoint.y()));
    path.lineTo(Val_XToPixel(rightTopPoint.x()), Val_YToPixel(rightTopPoint.y()));
    return path;
}

void SectorGrid::IncShowRange()
{
    if(m_maxVal_YZoom + 5 > m_maxVal_YPanel)
        return;
    m_maxVal_YZoom += 5;
    m_minVal_YZoom += 5;
    update();
}

void SectorGrid::DecShowRange()
{
    if(m_minVal_YZoom - 5 < 0)
        return;
    m_maxVal_YZoom -= 5;
    m_minVal_YZoom -= 5;
    update();
}

void SectorGrid::mousePressEvent(QGraphicsSceneMouseEvent* event)
{    
    if(m_rectGrid.contains(event->pos()))
    {
        CreateValue();
        qreal x = event->pos().x();
        qreal midle = m_width / 2;
        x = x - midle;
        qreal velocity = (m_maxVal_XZoom / midle) * x;

        qreal y = event->pos().y();
        qreal delta = m_maxVal_YZoom - m_minVal_YZoom;
        qreal range = m_maxVal_YZoom - (delta / m_height) * y;
        m_value->setPlainText(QString("x = %2 м\ny = %1 м").arg(QString::number(range, 2, 1)).arg(QString::number(velocity, 2, 1)));
        return;
    }

    if(m_leftPanel.contains(event->pos()))
    {
        // вычисляем значение выбранное пользователем
        qreal y = event->pos().y();
        qreal delta = m_maxVal_YPanel;
        qreal range = m_maxVal_YPanel - (delta / m_height) * y;

        int shiftCenter = static_cast<int>(range) / 5;
        m_maxVal_YZoom = shiftCenter * 5 + 15;
        m_minVal_YZoom = shiftCenter * 5 - 15;
        if(m_minVal_YZoom < 0)
        {
            m_minVal_YZoom = 0;
            m_maxVal_YZoom = 30;
        }
        if(m_maxVal_YZoom > m_maxVal_YPanel)
        {
            m_minVal_YZoom = m_maxVal_YPanel - 30;
            m_maxVal_YZoom = m_maxVal_YPanel;
        }
        update();
        return;
    }
}

void SectorGrid::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if(event->delta() > 0)
        IncShowRange();
    else
        DecShowRange();
}
