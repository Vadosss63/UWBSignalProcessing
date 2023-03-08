#include "transmitterudpblock.h"

TransmitterUDPBlock::TransmitterUDPBlock():
    SinkBlock("Передатчик"), m_module(nullptr)
{
    setToolTip("Выполняет передачу данных по протоколу UDP");
}

TransmitterUDPBlock::~TransmitterUDPBlock(){}

AbstractBlock* TransmitterUDPBlock::Clone() const
{
    return new TransmitterUDPBlock();
}

dspm::AbstractModule &TransmitterUDPBlock::GetModule()
{
    return *m_module;
}

QString TransmitterUDPBlock::GetType()
{
    return "TransmitterUDPBlock";
}

void TransmitterUDPBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void TransmitterUDPBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 10);
    painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, "UDP");
    painter->setFont(QFont());
}

void TransmitterUDPBlock::InitBlock(QWidget *)
{
    dspm::SenderUDPModule* module = new dspm::SenderUDPModule(this);
    if(m_module)
        delete m_module;
    m_module = module;
    CreatePorts(module);

    setAbstractDialogCommand(nullptr);
}


PluginBlock::PluginBlock(QObject* parent):QObject(parent){}

AbstractBlock*PluginBlock::LoudBlock() const
{
    return new TransmitterUDPBlock;
}
