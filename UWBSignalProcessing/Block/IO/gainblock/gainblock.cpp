#include "gainblock.h"

GainBlock::GainBlock(): IOBlock("Gain"),
    m_image(6)
{
    const QRectF& rect = GetborderRect();
    m_image[0] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.top()) + 15, static_cast<int>(rect.right()) - 15, static_cast<int>(rect.height())/2);
    m_image[1] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.top()) + 15, static_cast<int>(rect.left()) + 15, static_cast<int>(rect.bottom()) - 15);
    m_image[2] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.bottom()) - 15, static_cast<int>(rect.right()) - 15, static_cast<int>(rect.height())/2);

    m_image[3] = QLine(static_cast<int>(rect.left()) + 5, static_cast<int>(rect.height())/2, static_cast<int>(rect.width())/2, static_cast<int>(rect.top()) + 12);
    m_image[4] = QLine(static_cast<int>(rect.width())/2 - 8, static_cast<int>(rect.top()) + 14, static_cast<int>(rect.width())/2, static_cast<int>(rect.top()) + 12);
    m_image[5] = QLine(static_cast<int>(rect.width())/2 - 4, static_cast<int>(rect.top()) + 19, static_cast<int>(rect.width())/2, static_cast<int>(rect.top()) + 12);
    setToolTip("Умнажает сигнал на\nкоэффициент усиления");
}

AbstractBlock *GainBlock::Clone() const
{
    return new GainBlock();
}

AbstractModule* GainBlock::GetModule() const
{
    return m_module.get();
}

QString GainBlock::GetType() const
{
    return "GainBlock";
}

void GainBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetGainSignal(m_dialog->GetGain());
    }
}

void GainBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void GainBlock::DrawImage(QPainter *painter)
{
    painter->drawLines(m_image);
}

void GainBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<GainModule>();
    m_dialog = std::make_unique<GainDialogBox>();
    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void GainBlock::CreateBlockPorts()
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
    return new GainBlock;
}

void GainBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}

