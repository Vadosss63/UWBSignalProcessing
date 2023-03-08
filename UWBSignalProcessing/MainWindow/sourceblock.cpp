#include "sourceblock.h"

SourceBlock::SourceBlock(const QString& lable, QGraphicsItem* parent)
    : AbstractBlock(lable, parent){}

SourceBlock::~SourceBlock() {}

void SourceBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
}

QVariant SourceBlock::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if(change == ItemPositionHasChanged)
    {
        for (auto* port : m_port)
            port->TrackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}

QVector<OPortBlock*> SourceBlock::GetOutPort() const
{
    return m_port;
}

QVector<IPortBlock*> SourceBlock::GetInPort() const
{
    return QVector<IPortBlock*>();
}

void SourceBlock::CreatePorts(QVector<OPort*> ports)
{
    ClearPorts();
    uint8_t outputs = ports.size();
    if(outputs > 3)
        SetRectHeight(20 * outputs);

    QRectF rect(GetborderRect());
    short stepOut = (rect.height())/(outputs + 1);

    for (uint8_t i = 1; i < outputs + 1; i++)
    {
        OutPortBlock* outPortBlock = new OutPortBlock(
                                         QPoint(rect.right(), rect.top()  - 6 + i * stepOut),
                                         ports[i - 1], this);
        outPortBlock->SetId(i);
        m_port.append(outPortBlock);
    }
    update();
    // Переустановливаем родителей для портов
    SetId(GetId());
}

void SourceBlock::ClearPorts()
{
    if(m_port.size())
    {
        qDeleteAll(m_port);
        m_port.clear();
    }
}

QList<LinkStruct> SourceBlock::GetLinkStruct()
{
    QList<LinkStruct> linkStructs;
    QVector<OPortBlock*> ports = GetOutPort();
    foreach (OPortBlock* outPort, ports)
    {
        LineConnect* line = outPort->GetLine();
        if(line)
        {
            LinkStruct linkStruct;
            linkStruct.idInBlock = line->GetInPort()->GetParentId();
            linkStruct.idInPort = line->GetInPort()->GetId();

            linkStruct.idOutBlock = line->GetOutPort()->GetParentId();
            linkStruct.idOutPort = line->GetOutPort()->GetId();

            linkStructs.append(linkStruct);
        }
    }
    return linkStructs;
}

void SourceBlock::SetId(size_t id)
{
    AbstractBlock::SetId(id);

    for (auto* out : m_port)
        out->SetParentId(GetId());
}

void SourceBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    for (auto* port : m_port)
        port->TryConnect();

    AbstractBlock::mouseReleaseEvent(event);
}
