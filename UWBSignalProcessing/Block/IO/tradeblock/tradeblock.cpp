#include "tradeblock.h"

TradeBlock::TradeBlock(): IOBlock("ShiftTrade")
{
    setToolTip("Производит смещение отсчетов на константу");
}

TradeBlock::~TradeBlock(){}

AbstractBlock *TradeBlock::Clone() const
{
    return new TradeBlock();
}

AbstractModule* TradeBlock::GetModule() const
{
    return m_module.get();
}

QString TradeBlock::GetType() const
{
    return "TradeBlock";
}

void TradeBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetShiftBoard(m_dialog->GetShiftBoard());
    }
}

void TradeBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void TradeBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 8));
    painter->drawText(rect, Qt::AlignCenter, "shiftLine");
    painter->setFont(QFont());
}

void TradeBlock::InitBlock(QWidget* )
{
    m_module = std::make_unique<TradeModule>();
    m_dialog = std::make_unique<TradeDialogBox>();
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void TradeBlock::CreateBlockPorts()
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
    return new TradeBlock;
}

void TradeBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
