#include "maskfilterblock.h"

MaskFilterBlock::MaskFilterBlock():
    IOBlock("MaskFilter")
{
    setToolTip("Фильтрация сигнала при помощи налажения маски(size 8192) в частотной области");
}

AbstractBlock* MaskFilterBlock::Clone() const
{
    return new MaskFilterBlock();
}

AbstractModule* MaskFilterBlock::GetModule() const
{
    return m_module.get();
}

QString MaskFilterBlock::GetType() const
{
    return "MaskFilterBlock";
}

void MaskFilterBlock::Change()
{
    if (m_module && m_dialog) {
        m_module->SetMask(m_dialog->GetFilterArray());
    }
}


void MaskFilterBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}


void MaskFilterBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QPainterPath path;

    path.moveTo(rect.width()/2 - 18, (rect.height())/2 + 17);
    path.cubicTo(rect.width()/2 - 4, rect.height()/2 - 5,
                 rect.width()/2 - 4, (rect.height())/2 + 17,
                 rect.width()/2 - 1, (rect.height())/2 + 18);
    path.cubicTo(rect.width()/2 + 5, rect.bottom() ,
                 rect.width()/2 + 8, (rect.height())/2 + 16,
                 rect.width()/2 + 17, (rect.height())/2 + 15);
    painter->drawPath(path);

    path.moveTo(rect.width()/2 - 18, (rect.height())/2 );
    path.cubicTo(rect.width()/2 - 4, rect.height()/2 - 22,
                 rect.width()/2 - 4, (rect.height())/2 ,
                 rect.width()/2 - 1, (rect.height())/2 + 1);
    path.cubicTo(rect.width()/2 + 5, rect.bottom() - 17,
                 rect.width()/2 + 8, (rect.height())/2 - 1,
                 rect.width()/2 + 17, (rect.height())/2 - 2);
    painter->drawPath(path);


    path.moveTo(rect.width()/2 - 18, (rect.height())/2 - 17);
    path.cubicTo(rect.width()/2 - 4, rect.height()/2 - 39,
                 rect.width()/2 - 4, (rect.height())/2 - 17,
                 rect.width()/2 - 1, (rect.height())/2 - 16);
    path.cubicTo(rect.width()/2 + 5, rect.bottom()- 34 ,
                 rect.width()/2 + 8, (rect.height())/2 - 18,
                 rect.width()/2 + 17, (rect.height())/2 - 19);
    painter->drawPath(path);

    QVector<QLine> m_image(3);
    m_image[0] = QLine(static_cast<int>(rect.width())/2 - 6 , static_cast<int>(rect.bottom()) - 14, static_cast<int>(rect.width())/2 + 3, static_cast<int>(rect.height())/2 + 13);
    m_image[1] = QLine(static_cast<int>(rect.width())/2 - 6 , static_cast<int>(rect.bottom()) - 48, static_cast<int>(rect.width())/2 + 3, static_cast<int>(rect.height())/2 - 21);

    painter->drawLines(m_image);
}

void MaskFilterBlock::InitBlock(QWidget*)
{
    m_module = std::make_unique<MaskFilterModule>();
    m_dialog = std::make_unique<MaskFilterDialogBox>();
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void MaskFilterBlock::CreateBlockPorts()
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
    return new MaskFilterBlock;
}

void MaskFilterBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
