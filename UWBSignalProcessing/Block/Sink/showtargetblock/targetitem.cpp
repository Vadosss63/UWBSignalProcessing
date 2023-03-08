#include "targetitem.h"
#include "sectorgriditem.h"

int TargetItem::m_maxAMP = 1000;
int TargetItem::m_maxAMPdymamic = 1000;

TargetItem::TargetItem(const Target& target):
    m_target(target), m_pen(QPen(Qt::black, 1))
{
    CreateItem([](double x){
        return SectorGrid::Val_XToPixelLeftPanel(x);
    }, [](double y){
        return SectorGrid::Val_YToPixelLeftPanel(y);
    });
}

void TargetItem::ShowZoomItem()
{
    if(m_zoomItem)
    {
        if(!SectorGrid::ContenesZoom(m_zoomItem->m_target.GetY(m_target.GetFont())))
            m_zoomItem->setVisible(false);
        else
            m_zoomItem->setVisible(true);
    }
}

void TargetItem::SetMaxAMP(int maxAMP)
{
    m_maxAMP = maxAMP;
}

void TargetItem::SetMaxAMPdymamic(int max)
{
    m_maxAMPdymamic = max;
}

int TargetItem::GetMaxAMP()
{
    return m_maxAMP;
}

int TargetItem::GetMaxAMPdymamic()
{
    return m_maxAMPdymamic;
}

double TargetItem::GetCurrentSpeed()
{
    return m_target.GetToSpeedKMH(m_target.m_currentSpeed);
}

void TargetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setFont(QFont());
    painter->setPen(QPen(GetColorItem(), 3));
    painter->setBrush(QBrush(GetColorItem()));
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform|QPainter::NonCosmeticDefaultPen);
    painter->drawPath(m_pathItem);
//    painter->drawRect(m_boundingRect);
    painter->restore();
    ShowZoomItem();
}

void TargetItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

QColor TargetItem::GetColorItem()
{
    int amp = static_cast<int>(m_target.GetAMP());
    int max = !m_target.GetPoint(0).speed ? m_maxAMP : m_maxAMPdymamic;
    int alfa  = (amp < max) ? (amp * 255) / max : 255;
    m_colorItem.setAlpha(alfa);
    return m_colorItem;
}

void TargetItem::SetColorItem(const QColor& colorItem)
{
    m_colorItem = colorItem;
}

TargetItem* TargetItem::GetItemZoom()
{
    if(!m_zoomItem)
    {
        m_zoomItem = new TargetItem(m_target);
        m_zoomItem->m_boundingRect = QRectF(0, 0, 3, 3);
        m_zoomItem->CreateItem([](double x){
            return SectorGrid::Val_XToPixel(x);
        }, [](double y){
            return SectorGrid::Val_YToPixel(y);
        });
    }
    return m_zoomItem;
}


QPen TargetItem::GetPen() const
{
    return m_pen;
}

void TargetItem::SetPen(const QPen &pen)
{
    m_pen = pen;
}

QRectF TargetItem::boundingRect() const
{
    return m_boundingRect;
}

void TargetItem::CreateItem(const std::function<double (double)>& getX, const std::function<double(double)>& getY)
{
    double lastX = m_target.GetX(m_target.GetFont());
    double newX = 0;

    m_pathItem.moveTo(QPointF(getX(m_target.GetX(m_target.GetFont())),
                              getY(m_target.GetY(m_target.GetFont()))));

    for(Target::Point& point : m_target.GetPoints())
    {
        QPointF p = QPointF(getX(m_target.GetX(point)),
                            getY(m_target.GetY(point)));

        newX = m_target.GetX(point);
        if((std::abs(lastX - newX)) < 2.0)
        {
            m_pathItem.lineTo(p);
            lastX = newX;
        }

        m_boundingRect.moveCenter(p);
        m_pathItem.addEllipse(m_boundingRect);
    }
    //    setToolTip(QString("AMP = %1\nV = %2 км/ч\nX = %3 м\nY = %4 м\nSize = %5")
    //               .arg(m_target.GetAMP())
    //               .arg(m_target.GetSpeedKMH())
    //               .arg(m_target.GetX())
    //               .arg(m_target.GetY())
    //               .arg(m_target.m_pointTarget.size()));

     m_boundingRect = m_pathItem.boundingRect();

    if(m_target.GetFont().speed)
    {
        if(m_target.GetSpeedKMH() > 0)
            SetColorItem(QColor(Qt::red));
        else
            SetColorItem(QColor(Qt::blue));
    }
}
