#include "splitterblock.h"

SplitterBlock::SplitterBlock(): IOBlock("Делитель")
{
    setToolTip("Принимая сигнал на один входной порт,\nвыталкивает его в два выходных порта");
}

AbstractBlock* SplitterBlock::Clone() const
{
    return new SplitterBlock();
}

AbstractModule* SplitterBlock::GetModule() const
{
    return m_module.get();
}

QString SplitterBlock::GetType() const
{
    return "SplitterBlock";
}

void SplitterBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetOutputPortsNumber(m_dialog->GetOutputPortsNumber());
        ChangeCountPorts();
    }
}

void SplitterBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void SplitterBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont("Times", 12));
    painter->drawText(rect, Qt::AlignCenter, "DMX");
    painter->setFont(QFont());
}

void SplitterBlock::ChangeCountPorts()
{
    CreateBlockPorts();
}

void SplitterBlock::InitBlock(QWidget * )
{
    m_module = std::make_unique<SplitterModule>();
    m_dialog = std::make_unique<SplitterDialogBox>();
    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void SplitterBlock::CreateBlockPorts()
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
    return new SplitterBlock;
}

void SplitterBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
