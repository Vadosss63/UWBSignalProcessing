#include "transmitterudpblock.h"

TransmitterUDPBlock::TransmitterUDPBlock():
    SinkBlock("Передатчик"), m_module(nullptr)
{
    setToolTip("Выполняет передачу данных по протоколу UDP");
}

AbstractBlock* TransmitterUDPBlock::Clone() const
{
    return new TransmitterUDPBlock();
}

AbstractModule* TransmitterUDPBlock::GetModule() const
{
    return m_module.get();
}

QString TransmitterUDPBlock::GetType() const
{
    return "TransmitterUDPBlock";
}

void TransmitterUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void TransmitterUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "UDP");
    painter->setFont(QFont());
}

void TransmitterUDPBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<TransmitterUDPModule>();
    CreateBlockPorts();
}

void TransmitterUDPBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountInputPort();
    QVector<IPort*> ports;
    for (uint8_t i = 0; i < size; ++i)
    {
        ports.push_back(m_module->GetInput(i));
    }
    CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock* PluginBlock::LoudBlock() const
{
    return new TransmitterUDPBlock;
}

void TransmitterUDPBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
