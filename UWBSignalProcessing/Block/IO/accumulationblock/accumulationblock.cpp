#include "accumulationblock.h"

AccumulationBlock::AccumulationBlock() :
    IOBlock("Накопление"), m_image(4)
{
    setToolTip("Производит сумирование сигнала n раз, \n после чего происходит выталкивания \n среднего значения");
    const QRectF& rect = GetborderRect();
    m_image[0] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.top()) + 15, static_cast<int>(rect.right()) - 15, static_cast<int>(rect.top()) + 15);
    m_image[1] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.top()) + 15, static_cast<int>(rect.width()/2) + 15, static_cast<int>(rect.height()/2));
    m_image[2] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.bottom()) - 15, static_cast<int>(rect.width()/2) + 15, static_cast<int>(rect.height()/2));
    m_image[3] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.bottom()) - 15, static_cast<int>(rect.right()) - 15, static_cast<int>(rect.bottom()) - 15);
}

AbstractBlock* AccumulationBlock::Clone() const
{
    return new AccumulationBlock();
}

AbstractModule* AccumulationBlock::GetModule() const
{
    return m_module.get();
}

QString AccumulationBlock::GetType() const
{
    return "ConstAccumulationBlock";
}

void AccumulationBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetSizeBuf(m_dialog->GetSizeBuf(), m_dialog->GetTypeAccum());
    }
}

void AccumulationBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void AccumulationBlock::DrawImage(QPainter* painter)
{
    painter->drawLines(m_image);
}

void AccumulationBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<AccumulationModule>();
    m_dialog = std::make_unique<AccumulationDialogBox>();
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void AccumulationBlock::CreateBlockPorts()
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
    return new AccumulationBlock;
}

void AccumulationBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}

