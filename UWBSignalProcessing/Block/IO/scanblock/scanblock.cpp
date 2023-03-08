#include "scanblock.h"

ScanBlock::ScanBlock():
    IOBlock("scan")
{
    setToolTip("Создает скан из набора сигналов");
}

AbstractBlock* ScanBlock::Clone() const
{
    return new ScanBlock();
}

AbstractModule* ScanBlock::GetModule() const
{
    return m_module.get();
}

QString ScanBlock::GetType() const
{
    return "ScanBlock";
}

void ScanBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ScanBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->setFont(QFont("Times", 10));
    painter->drawText(rect, Qt::AlignCenter, "Scan");
    painter->setFont(QFont());
}

void ScanBlock::ChangeCountPorts()
{
    CreateBlockPorts();
}

void ScanBlock::InitBlock(QWidget*)
{
    m_module = std::make_unique<ScanModule>();
    CreateBlockPorts();
    SetAbstractDialogCommand(nullptr);
}

void ScanBlock::CreateBlockPorts()
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


PluginBlock::PluginBlock(QObject* parent):QObject(parent){}

AbstractBlock*PluginBlock::LoudBlock() const
{
    return new ScanBlock;
}

void ScanBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
