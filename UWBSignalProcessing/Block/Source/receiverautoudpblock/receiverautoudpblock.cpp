#include "receiverautoudpblock.h"

ReceiverAutoUDPBlock::ReceiverAutoUDPBlock() :
    SourceBlock("AutoUDP")
{
    setToolTip("Выполняет прием данных по протоколу AutoUDP");
}

AbstractBlock *ReceiverAutoUDPBlock::Clone() const
{
    return new ReceiverAutoUDPBlock();
}

AbstractModule* ReceiverAutoUDPBlock::GetModule() const
{
    return m_module.get();
}

QString ReceiverAutoUDPBlock::GetType() const
{
    return "ReceiverAutoUDPBlock";
}

void ReceiverAutoUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ReceiverAutoUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 10));
    painter->drawText(rect, Qt::AlignCenter, "Auto");
    painter->setFont(QFont());
}

void ReceiverAutoUDPBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<ReceiverAutoUDPModule>();
    CreateBlockPorts();
}

void ReceiverAutoUDPBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountOutputPort();
    QVector<OPort*> ports;
    for (uint8_t i = 0; i < size; ++i)
    {
        ports.push_back(m_module->GetOutput(i));
    }
    CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject* parent):QObject(parent){}

AbstractBlock* PluginBlock::LoudBlock() const
{
    return new ReceiverAutoUDPBlock;
}

void ReceiverAutoUDPBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
