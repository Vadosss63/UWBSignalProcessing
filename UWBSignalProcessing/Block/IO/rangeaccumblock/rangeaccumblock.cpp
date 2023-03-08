#include "rangeaccumblock.h"

RangeAccumBlock::RangeAccumBlock(): IOBlock("Get Speed")
{
    setToolTip("Получает значение отсчетов через некоторый период");
}

AbstractBlock *RangeAccumBlock::Clone() const
{
    return new RangeAccumBlock();
}

AbstractModule* RangeAccumBlock::GetModule() const
{
    return m_module.get();
}

QString RangeAccumBlock::GetType() const
{
    return "RangeAccumBlock";
}

void RangeAccumBlock::Change()
{
    if (m_module && m_dialog)
    {
        m_module->SetTypeWindow(m_dialog->GetWindow());
        m_module->SetTypeThreshol(m_dialog->GetThresholType());
        m_module->SetIsVARU(m_dialog->IsVARU());
        m_module->SetGain(m_dialog->GetGain());

        m_module->SetSetting(m_dialog->GetCountAcumm(), m_dialog->GetNumberRange(), m_dialog->GetRangeAcumm(), m_dialog->GetThresholAcumm());
    }
}

void RangeAccumBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void RangeAccumBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 8));
    painter->drawText(rect, Qt::AlignCenter, "Get Range");
    painter->setFont(QFont());
}

void RangeAccumBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<RangeAccumModule>();
    m_dialog = std::make_unique<RangeAccumDialogBox>();
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void RangeAccumBlock::CreateBlockPorts()
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
    return new RangeAccumBlock;
}

void RangeAccumBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
