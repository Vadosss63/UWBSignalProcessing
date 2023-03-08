#include "receiverudpblock.h"


ReceiverUDPBlock::ReceiverUDPBlock() :
    SourceBlock("UDP")
{
    setToolTip("Выполняет прием csignal данных UDP");
}

AbstractBlock *ReceiverUDPBlock::Clone() const
{
    return new ReceiverUDPBlock();
}

AbstractModule* ReceiverUDPBlock::GetModule() const
{
    return m_module.get();
}

QString ReceiverUDPBlock::GetType() const
{
    return "ReceiverUDPBlock";
}

void ReceiverUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ReceiverUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 10));
    painter->drawText(rect, Qt::AlignCenter, "UDP");
    painter->setFont(QFont());
}

void ReceiverUDPBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<ReceiverUDPModule>();
    CreateBlockPorts();
}

void ReceiverUDPBlock::CreateBlockPorts()
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
    return new ReceiverUDPBlock;
}

void ReceiverUDPBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
