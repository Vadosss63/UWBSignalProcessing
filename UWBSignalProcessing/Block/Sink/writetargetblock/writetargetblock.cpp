#include "writetargetblock.h"

WriteTargetBlock::WriteTargetBlock():
    SinkBlock("Write\nTarget")
{
    setToolTip("Выполняет запись целей\n в указанный файл");
}

AbstractBlock* WriteTargetBlock::Clone() const
{
    return new WriteTargetBlock();
}

AbstractModule* WriteTargetBlock::GetModule() const
{
    return m_module.get();
}

QString WriteTargetBlock::GetType() const
{
    return "WriteTargetBlock";
}

void WriteTargetBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetPathFile(m_dialog->GetPathFile());
    }
}

void WriteTargetBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void WriteTargetBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "Запись\nв файл");
    painter->setFont(QFont());
}

void WriteTargetBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountInputPort();
    QVector<IPort*> ports;
    for (uint8_t i = 0; i < size; ++i)
    {
        ports.push_back(m_module->GetInput(i));
    }
    CreatePorts(ports);
}

void WriteTargetBlock::InitBlock(QWidget *)
{    
    m_module = std::make_unique<WriteTargetModule>();
    m_dialog = std::make_unique<WriteTargetDialogBox>();

    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock* PluginBlock::LoudBlock() const
{
    return new WriteTargetBlock;
}

void WriteTargetBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
