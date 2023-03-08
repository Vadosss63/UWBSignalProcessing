#include "readcomplexblock.h"

ReadComplexBlock::ReadComplexBlock() :
    SourceBlock("ReadComplex")
{
    setToolTip("Производит чтение комплекного сигнала из файла");
}

AbstractBlock* ReadComplexBlock::Clone() const
{
    return new ReadComplexBlock();
}

AbstractModule* ReadComplexBlock::GetModule() const
{
    return m_module.get();
}

QString ReadComplexBlock::GetType() const
{
    return "ReadComplexBlock";
}

void ReadComplexBlock::Change()
{
    if (m_dialog && m_module) {
        m_module->SetPathFile(m_dialog->GetPathFileSigI(), m_dialog->GetPathFileSigQ());
    }
}

void ReadComplexBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ReadComplexBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont ("Times", 10));
    painter->drawText(rect, Qt::AlignCenter, "Чтение\nиз файла");
    painter->setFont(QFont());
}

void ReadComplexBlock::InitBlock(QWidget *)
{
    m_module = std::make_unique<ReadComplexModule>();
    m_dialog = std::make_unique<ReadComplexDialogBox>();
    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void ReadComplexBlock::CreateBlockPorts()
{
    QVector<OPort*> ports;

    uint8_t size = m_module->CountOutputPort();
    for (uint8_t i = 0; i < size; ++i)
        ports.push_back(m_module->GetOutput(i));

    CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock* PluginBlock::LoudBlock() const
{
    return new ReadComplexBlock;
}

void ReadComplexBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
