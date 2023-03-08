#include "statisticalestimateblock.h"

StatisticaleStimateBlock::StatisticaleStimateBlock():
    SinkBlock("Stimate")
{
    setToolTip("Статистическая оценка сигнала");
}

AbstractBlock* StatisticaleStimateBlock::Clone() const
{
    return new StatisticaleStimateBlock();
}

AbstractModule* StatisticaleStimateBlock::GetModule() const
{
    return m_module.get();
}

QString StatisticaleStimateBlock::GetType() const
{
    return "StatisticaleStimateBlock";
}

void StatisticaleStimateBlock::Change()
{
    if (m_module && m_dialog)
    {
        m_module->SetIndex(m_dialog->GetStartIndex(), m_dialog->GetStopIndex());
    }
}

void StatisticaleStimateBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void StatisticaleStimateBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "Мат.\nстатистика");
    painter->setFont(QFont());
}

void StatisticaleStimateBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountInputPort();
    QVector<IPort*> ports;
    for (uint8_t i = 0; i < size; ++i)
    {
        ports.push_back(m_module->GetInput(i));
    }
    CreatePorts(ports);
}


void StatisticaleStimateBlock::InitBlock(QWidget *)
{        
    m_module = std::make_unique<StatisticaleStimateModule>();
    m_dialog =std::make_unique<StatisticaleStimateDialogBox>();
    m_module->SetShowStatisticaleStimate(m_dialog.get());
    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock*PluginBlock::LoudBlock() const
{
    return new StatisticaleStimateBlock;
}

void StatisticaleStimateBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
