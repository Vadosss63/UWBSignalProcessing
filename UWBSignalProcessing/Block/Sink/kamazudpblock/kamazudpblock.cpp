#include "kamazudpblock.h"

KamazUDPBlock::KamazUDPBlock():
    SinkBlock("Камаз UDP"), m_module(nullptr)
{
    setToolTip("Выполняет передачу данных по протоколу UDP");
}

AbstractBlock* KamazUDPBlock::Clone() const
{
    return new KamazUDPBlock();
}

AbstractModule* KamazUDPBlock::GetModule() const
{
    return m_module.get();
}

QString KamazUDPBlock::GetType() const
{
    return "KamazUDPBlock";
}

void KamazUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void KamazUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "UDP\nKamaz");
    painter->setFont(QFont());
}

void KamazUDPBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<KamazUDPModule>();
    CreateBlockPorts();
}

void KamazUDPBlock::CreateBlockPorts()
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

AbstractBlock*PluginBlock::LoudBlock() const
{
    return new KamazUDPBlock;
}

void KamazUDPBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
