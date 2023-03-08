#include "correlationblock.h"

CorrelationBlock::CorrelationBlock() :
    IOBlock("Корреляция"), m_module(nullptr)
{
    setToolTip("Выполняет корреляцию входных сигналов");
}

AbstractBlock *CorrelationBlock::Clone() const
{
    return new CorrelationBlock();
}

AbstractModule* CorrelationBlock::GetModule() const
{
    return m_module.get();
}

QString CorrelationBlock::GetType() const
{
    return "CorrelationBlock";
}

void CorrelationBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void CorrelationBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();

    painter->setFont(QFont ("Times", 12));
    painter->drawText(rect, Qt::AlignCenter, "Correlation");
    painter->setFont(QFont());

}

void CorrelationBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<Correlationmodule>();
    CreateBlockPorts();
}

void CorrelationBlock::CreateBlockPorts()
{
    uint8_t sizeIn = m_module->CountInputPort();
    QVector<IPort*> inPorts;
    for (uint8_t i = 0; i < sizeIn; ++i)
        inPorts.push_back(m_module->GetInput(i));

    uint8_t sizeOut = m_module->CountOutputPort();
    QVector<OPort*> outPorts;
    for (uint8_t i = 0; i < sizeOut; ++i)
        outPorts.push_back(m_module->GetOutput(i));

    CreatePorts(inPorts, outPorts);
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock* PluginBlock::LoudBlock() const
{
    return new CorrelationBlock;
}

void CorrelationBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
