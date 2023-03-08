#include "smablock.h"

SMABlock::SMABlock(): IOBlock("SMA")
{
    setToolTip("Выполняет вычисление скользящиго среднего по заданному окну");
}

AbstractBlock* SMABlock::Clone() const
{
    return new SMABlock();
}

AbstractModule* SMABlock::GetModule() const
{
    return m_module.get();
}

QString SMABlock::GetType() const
{
    return "SMABlock";
}

void SMABlock::Change()
{
    if (m_module && m_dialog)
        m_module->SetSizeWindow(m_dialog->GetSizeWindow());

}

void SMABlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void SMABlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "SMA");
    painter->setFont(QFont());
}

void SMABlock::InitBlock(QWidget*)
{
    m_module = std::make_unique<SMAModule>();
    m_dialog = std::make_unique<SMADialogBox>();

    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void SMABlock::CreateBlockPorts()
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
    return new SMABlock;
}

void SMABlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
