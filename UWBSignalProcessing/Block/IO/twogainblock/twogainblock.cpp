#include "twogainblock.h"

TwoGainBlock::TwoGainBlock(): IOBlock("TwoGain"),
    m_image(6)
{
    setToolTip("Формирует сигнал из двух, путем склеивания первой\n части от первого и второй от второго,\n раздеителем является индекс нового усиления");
}

AbstractBlock *TwoGainBlock::Clone() const
{
    return new TwoGainBlock();
}

AbstractModule* TwoGainBlock::GetModule() const
{
    return m_module.get();
}

QString TwoGainBlock::GetType() const
{
    return "TwoGainBlock";
}

void TwoGainBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetIndexOtherGainSignal(m_dialog->GetIndexOtherGainSignal());
    }
}

void TwoGainBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void TwoGainBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 8));
    painter->drawText(rect, Qt::AlignCenter, "1 + 1");
    painter->setFont(QFont());
}

void TwoGainBlock::InitBlock(QWidget*)
{
    m_module = std::make_unique<TwoGainModule>();
    m_dialog = std::make_unique<TwoGainDialogBox>();

    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void TwoGainBlock::CreateBlockPorts()
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
    return new TwoGainBlock;
}

void TwoGainBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}

