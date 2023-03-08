#include "receiverkamazudpblock.h"

ReceiverKamazUDPBlock::ReceiverKamazUDPBlock() :
    SourceBlock("Приемник")
{
    setToolTip("Выполняет прием данных по протоколу UDP");
}

AbstractBlock *ReceiverKamazUDPBlock::Clone() const
{
    return new ReceiverKamazUDPBlock();
}

AbstractModule* ReceiverKamazUDPBlock::GetModule() const
{
    return m_module.get();
}

QString ReceiverKamazUDPBlock::GetType() const
{
    return "ReceiverKamazUDPBlock";
}

void ReceiverKamazUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ReceiverKamazUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 10));
    painter->drawText(rect, Qt::AlignCenter, "UDP камаз");
    painter->setFont(QFont());
}

void ReceiverKamazUDPBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<ReceiverKamazUDPModule>();
    CreateBlockPorts();
    SetAbstractDialogCommand(nullptr);
}

void ReceiverKamazUDPBlock::CreateBlockPorts()
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
    return new ReceiverKamazUDPBlock;
}

void ReceiverKamazUDPBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
