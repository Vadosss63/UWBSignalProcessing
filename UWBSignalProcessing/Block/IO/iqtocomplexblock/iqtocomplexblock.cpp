#include "iqtocomplexblock.h"

IQToComplexBlock::IQToComplexBlock() :
    IOBlock("IQ2Complex")
{
    setToolTip("Принимает на вход квадратуры, на выходе выдает комплексный сигнал");
}

AbstractBlock *IQToComplexBlock::Clone() const
{
    return new IQToComplexBlock();
}

AbstractModule* IQToComplexBlock::GetModule() const
{
    return m_module.get();
}

QString IQToComplexBlock::GetType() const
{
    return "IQToComplexBlock";
}

void IQToComplexBlock::Change(){}

void IQToComplexBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void IQToComplexBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();

    painter->drawText(static_cast<int>(rect.right()) - 18, static_cast<int>(rect.center().y()) + 5, "C");
    painter->drawText(static_cast<int>(rect.left()) + 8, static_cast<int>(rect.top()) + 20, "I");
    painter->drawText(static_cast<int>(rect.left()) + 8, static_cast<int>(rect.bottom()) - 10, "Q");
    painter->setFont(QFont());
}

void IQToComplexBlock::ChangeCountPorts()
{
    CreateBlockPorts();
}

void IQToComplexBlock::InitBlock(QWidget * )
{
    m_module = std::make_unique<IQToComplexModule>();
    CreateBlockPorts();
}

void IQToComplexBlock::CreateBlockPorts()
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
    return new IQToComplexBlock;
}

void IQToComplexBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
