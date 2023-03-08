#include "autoudpblock.h"

AutoUDPBlock::AutoUDPBlock():
    SinkBlock("АВТО UDP"), m_module(nullptr)
{
    setToolTip("Протокол передачи ВРЛИ");
}

AbstractBlock* AutoUDPBlock::Clone() const
{
    return new AutoUDPBlock();
}

AbstractModule* AutoUDPBlock::GetModule() const
{
    return m_module.get();
}

QString AutoUDPBlock::GetType() const
{
    return "AutoUDPBlock";
}

void AutoUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void AutoUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "UDP\nTarget");
    painter->setFont(QFont());
}

void AutoUDPBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<AutoUDPModule>();
    CreateBlockPorts();
}

void AutoUDPBlock::CreateBlockPorts()
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
    return new AutoUDPBlock;
}

void AutoUDPBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
