#include "changesizesignalblock.h"

ChangeSizeSignalBlock::ChangeSizeSignalBlock():
    IOBlock("SizeSig")
{
    setToolTip("Изменяет длину сигнала");
}

AbstractBlock *ChangeSizeSignalBlock::Clone() const
{
    return new ChangeSizeSignalBlock();
}

AbstractModule* ChangeSizeSignalBlock::GetModule() const
{
    return m_module.get();
}

QString ChangeSizeSignalBlock::GetType() const
{
    return "ChangeSizeSignalBlock";
}

void ChangeSizeSignalBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetParameters(m_dialog->GetStartIndex(), m_dialog->GetSiseSig());
    }
}

void ChangeSizeSignalBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ChangeSizeSignalBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 8));
    painter->drawText(rect, Qt::AlignCenter, "Size");
    painter->setFont(QFont());
}

void ChangeSizeSignalBlock::InitBlock(QWidget*)
{
    m_module = std::make_unique<ChangeSizeSignalModule>();
    m_dialog = std::make_unique<ChangeSizeSignalDialogBox>();
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void ChangeSizeSignalBlock::CreateBlockPorts()
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
    return new ChangeSizeSignalBlock;
}

void ChangeSizeSignalBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
