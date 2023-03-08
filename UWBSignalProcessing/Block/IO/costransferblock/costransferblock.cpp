#include "costransferblock.h"

CosTransferBlock::CosTransferBlock():
    IOBlock("Cos"), m_module(nullptr)
{
    setToolTip("Выполняет перенос сигнала\nна заданую частоту");
}

AbstractBlock *CosTransferBlock::Clone() const
{
    return new CosTransferBlock();
}

AbstractModule* CosTransferBlock::GetModule() const
{
    return m_module.get();
}

QString CosTransferBlock::GetType() const
{
    return "CosTransferBlock";
}

void CosTransferBlock::Change()
{
    if (m_module && m_dialog)
    {
        m_module->ChangeParametersGenerator(m_dialog->GetFrequency(),
                                            m_dialog->GetPhase(),
                                            m_dialog->GetSimpleFrequency());
    }
}

void CosTransferBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void CosTransferBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    QFont f("Times", 20);
    QRect rectText(static_cast<int>(rect.left()) , static_cast<int>(rect.top())+ 10, static_cast<int>(rect.width()), static_cast<int>(rect.height()) / 2);
    painter->setFont(f);
    painter->drawText(rectText, Qt::AlignHCenter, "G");
    painter->setFont(QFont());
    QPainterPath path;

    path.moveTo(rect.width()/2 - 12, (rect.height())/2 + 17);

    path.cubicTo(rect.width()/2 - 4, rect.height()/2 - 5,
                 rect.width()/2 - 4, (rect.height())/2 + 17,
                 rect.width()/2 - 1, (rect.height())/2 + 18);

    path.cubicTo(rect.width()/2 + 5, rect.bottom() ,
                 rect.width()/2 + 8, (rect.height())/2 + 16,
                 rect.width()/2 + 11, (rect.height())/2 + 15);

    painter->drawPath(path);

    QVector<QLine> m_image(3);
    m_image[0] = QLine(static_cast<int>(rect.width())/2 - 12 , static_cast<int>(rect.bottom()) - 10, static_cast<int>(rect.width())/2 + 7, static_cast<int>(rect.height())/2 + 9);
    m_image[1] = QLine(static_cast<int>(rect.width())/2 + 1, static_cast<int>(rect.bottom()) - 25, static_cast<int>(rect.width())/2 + 7, static_cast<int>(rect.height())/2 + 9);
    m_image[2] = QLine(static_cast<int>(rect.width())/2 + 5, static_cast<int>(rect.bottom()) - 20, static_cast<int>(rect.width())/2 + 7, static_cast<int>(rect.height())/2 + 9);
    painter->drawLines(m_image);
}

void CosTransferBlock::InitBlock(QWidget*)
{
    m_module = std::make_unique<CosTransferModule>();
    m_dialog = std::make_unique<CosTransferDialogBox>();
    CreateBlockPorts();
    SetAbstractDialogCommand(m_dialog.get());
}

void CosTransferBlock::CreateBlockPorts()
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
    return new CosTransferBlock;
}

void CosTransferBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
