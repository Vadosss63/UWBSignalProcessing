#include "phaserblock.h"

PhaserBlock::PhaserBlock() :
    IOBlock("ФВ")
{
    setToolTip("Выполняет изменение фазы сигнала");
}

AbstractBlock *PhaserBlock::Clone() const
{
    return new PhaserBlock();
}

AbstractModule* PhaserBlock::GetModule() const
{
    return m_module.get();
}

QString PhaserBlock::GetType() const
{
    return "PhaserBlock";
}

void PhaserBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetPhase(m_dialog->GetPhase());
    }
}

void PhaserBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void PhaserBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->drawText(static_cast<int>(rect.left()) + 8, static_cast<int>(rect.top()) + 20, "I");
    painter->drawText(static_cast<int>(rect.left()) + 5, static_cast<int>(rect.bottom()) - 10, "Q");
    painter->drawText(static_cast<int>(rect.right()) - 15, static_cast<int>(rect.top()) + 20, "I");
    painter->drawText(static_cast<int>(rect.right()) - 18, static_cast<int>(rect.bottom()) - 10, "Q");
    painter->setFont(QFont("Times", 15));
    painter->drawText(rect, Qt::AlignCenter, "ФВ");
    painter->setFont(QFont());
}

void PhaserBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<PhaserModule>();
    m_dialog = std::make_unique<PhaserDialogBox>();
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void PhaserBlock::CreateBlockPorts()
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
    return new PhaserBlock;
}

void PhaserBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
