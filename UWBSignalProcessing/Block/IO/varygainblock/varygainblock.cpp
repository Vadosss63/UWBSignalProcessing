#include "varygainblock.h"

VaryGainBlock::VaryGainBlock(): IOBlock("Вару усил."),
    m_image(6)
{
    const QRectF& rect = GetborderRect();
    m_image[0] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.top()) + 15, static_cast<int>(rect.right()) - 15, static_cast<int>(rect.height())/2);
    m_image[1] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.top()) + 15, static_cast<int>(rect.left()) + 15, static_cast<int>(rect.bottom()) - 15);
    m_image[2] = QLine(static_cast<int>(rect.left()) + 15, static_cast<int>(rect.bottom()) - 15, static_cast<int>(rect.right()) - 15, static_cast<int>(rect.height())/2);

    m_image[3] = QLine(static_cast<int>(rect.left()) + 5, static_cast<int>(rect.height())/2, static_cast<int>(rect.width())/2, static_cast<int>(rect.top()) + 12);
    m_image[4] = QLine(static_cast<int>(rect.width())/2 - 8, static_cast<int>(rect.top()) + 14, static_cast<int>(rect.width())/2, static_cast<int>(rect.top()) + 12);
    m_image[5] = QLine(static_cast<int>(rect.width())/2 - 4, static_cast<int>(rect.top()) + 19, static_cast<int>(rect.width())/2, static_cast<int>(rect.top()) + 12);
    setToolTip("ВАРУ");
}

AbstractBlock *VaryGainBlock::Clone() const
{
    return new VaryGainBlock();
}

AbstractModule* VaryGainBlock::GetModule() const
{
    return m_module.get();
}

QString VaryGainBlock::GetType() const
{
    return "VaryGainBlock";
}

void VaryGainBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetIndexConst(m_dialog->GetIndex());
    }
}

void VaryGainBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void VaryGainBlock::DrawImage(QPainter *painter)
{
    painter->drawLines(m_image);
}

void VaryGainBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<VaryGainModule>();
    m_dialog = std::make_unique<VaryGainDialogBox>();
    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void VaryGainBlock::CreateBlockPorts()
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
    return new VaryGainBlock;
}

void VaryGainBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}

