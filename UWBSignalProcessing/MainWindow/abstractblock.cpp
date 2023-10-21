#include "abstractblock.h"

AbstractBlock::AbstractBlock(const QString &lable, QGraphicsItem *parent): QGraphicsItem (parent),
    m_lable(lable), m_textColor(Qt::darkRed), m_backgroundColor(Qt::white), m_outLineColor(Qt::darkBlue)
{
    setAcceptDrops(true);
    setZValue(0);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    m_borderRect = QRect(0, 0, 70, 70);
    setCursor(Qt::OpenHandCursor);
}

AbstractBlock::~AbstractBlock(){}

QRectF AbstractBlock::OutlineRect() const
{
    int Padding = 24;
    QRectF rect(m_borderRect);
    rect.adjust(-Padding, -5, +Padding, +Padding);
    return rect;
}

QRectF AbstractBlock::boundingRect() const
{
    const int Margin = 0;
    return OutlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath AbstractBlock::shape() const
{
    QPainterPath path;
    path.addRoundedRect(m_borderRect, Roundness(m_borderRect.width()), Roundness(m_borderRect.height()), Qt::RelativeSize);
    return path;
}

void AbstractBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget */*widget*/)
{
    DrawRect(painter, option);
}

void AbstractBlock::ShowHelp(){}

void AbstractBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event )
{
    ShowDialog();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void AbstractBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(acceptDrops())
    {
        if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
                .length() < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag(event->widget());
        BlockMimeData* mime = new BlockMimeData(this);
        drag->setMimeData(mime);
        QPixmap pixmap(static_cast<int>(boundingRect().height()), static_cast<int>(boundingRect().height()) + 24);
        ///TODO сделать равным цвету сцены
        pixmap.fill(Qt::white);
        QPainter painter(&pixmap);
        painter.translate(15, 15);
        painter.setRenderHint(QPainter::Antialiasing);
        QStyleOptionGraphicsItem option;
        DrawRect(&painter, &option);
        DrawImage(&painter);
        painter.end();
        pixmap.setMask(pixmap.createHeuristicMask());
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(15, 20));
        drag->exec();
        setCursor(Qt::OpenHandCursor);
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void AbstractBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    setZValue(0);
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

int AbstractBlock::Roundness(double size) const
{
    const int Diametr = 12;
    return 100 * Diametr / static_cast<int>(size);
}

void AbstractBlock::SetAbstractDialogCommand(DialogWindow *abstractDialogCommand)
{
    m_dialogWindow = abstractDialogCommand;
    if(m_dialogWindow)
        m_dialogWindow->Attach(this);
}

QList<std::pair<QString, QVariant>> AbstractBlock::GetSetting() const
{
    return m_dialogWindow ? m_dialogWindow->GetSetting() : QList<std::pair<QString, QVariant> >();
}

bool AbstractBlock::SetSetting(QList<std::pair<QString, QVariant> > list)
{
    if(m_dialogWindow)
    {
        if(m_dialogWindow->SetSetting(list))
        {
            m_dialogWindow->SendCommand();
            return true;
        }
    }
    return false;
}

void AbstractBlock::Change(){}

BlockStruct AbstractBlock::GetBlockStruct()
{
    BlockStruct blockStruct;
    blockStruct.id = GetId();
    blockStruct.type = GetType();
    blockStruct.coordX = static_cast<int>(pos().x());
    blockStruct.coordY = static_cast<int>(pos().y());
    blockStruct.listSetting = GetSetting();
    return blockStruct;
}

QList<LinkStruct> AbstractBlock::GetLinkStruct()
{
    return QList<LinkStruct>();
}

size_t AbstractBlock::GetId() const
{
    return m_id;
}

void AbstractBlock::SetId(size_t id)
{
    m_id = id;
}

void AbstractBlock::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    setCursor(Qt::ClosedHandCursor);
    setZValue(10);
    QGraphicsItem::mousePressEvent(event);
}

void AbstractBlock::ShowDialog()
{
    if(m_dialogWindow)
        m_dialogWindow->ShowDialog();
}

void AbstractBlock::SetRect(QRect& r)
{
    m_borderRect = r;
}

void AbstractBlock::SetRectWidth(int w)
{
    m_borderRect.setWidth(w);
}

void AbstractBlock::SetRectHeight(int h)
{
    m_borderRect.setHeight(h);
}

QRectF& AbstractBlock::GetborderRect()
{
    return m_borderRect;
}

int AbstractBlock::GetRectWidth() const
{
    return static_cast<int>(m_borderRect.width());
}

int AbstractBlock::GetRectHeight() const
{
    return static_cast<int>(m_borderRect.height());
}

void AbstractBlock::DrawRect(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
    QPen pen(m_outLineColor);
    if(option->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    //    painter->setBrush(m_backgroundColor);
    painter->drawRoundedRect(m_borderRect, Roundness(m_borderRect.width()), Roundness(m_borderRect.height()),Qt::RelativeSize);

    painter->setPen(m_textColor);
    QRectF rectLable(OutlineRect().left(), m_borderRect.bottom(),
                     OutlineRect().width(), OutlineRect().bottom() - m_borderRect.bottom() );
    painter->drawText(rectLable, Qt::AlignCenter, m_lable);
}

///====================================================///

BlockMimeData::BlockMimeData(AbstractBlock *block) :
    QMimeData(), m_block(block){}

BlockMimeData::~BlockMimeData(){}

bool BlockMimeData::HasAbstractBlock() const
{
    return m_block;
}

AbstractBlock* BlockMimeData::Block() const
{
    return m_block;
}

Block::~Block(){}

