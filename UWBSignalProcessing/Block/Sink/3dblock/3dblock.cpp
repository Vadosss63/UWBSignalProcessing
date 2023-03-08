#include "3dblock.h"

AbstractScanBlock::AbstractScanBlock(const QString& title):
    SinkBlock(title)
{
    m_gradientImage.setColorAt(0, Qt::white);
    m_gradientImage.setColorAt(1, Qt::gray);
    m_gradientImage.setAngle(52);
    m_gradientImage.setCenter(GetborderRect().center());
}

void AbstractScanBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void AbstractScanBlock::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->drawEllipse(static_cast<int>(rect.left()) + 8, static_cast<int>(rect.bottom()) - 12, 8, 8);
    painter->drawEllipse(static_cast<int>(rect.right()) - 16, static_cast<int>(rect.bottom()) - 12, 8, 8);
    painter->setBrush(m_gradientImage);
    QPointF center(GetborderRect().center());
    painter->drawEllipse(center, 28, 28);
    painter->drawEllipse(center, 18, 18);
    painter->drawEllipse(center, 8, 8);
    painter->drawLine(center, QPointF(52, 13));
}

QConicalGradient& AbstractScanBlock::GradientImage()
{
    return m_gradientImage;
}

//////////////////
D3Block::D3Block():
    AbstractScanBlock("3D")
{
    setToolTip("3D");
}

AbstractBlock* D3Block::Clone() const
{
    return new D3Block();
}

AbstractModule* D3Block::GetModule() const
{
    return m_module.get();
}

QString D3Block::GetType() const
{
    return "3dBlock";
}

void D3Block::Change()
{
    if (m_module && m_dialog){}
}

void D3Block::InitBlock(QWidget*)
{
    m_dialog = std::make_unique<D3DialogBox>();

    m_module = std::make_unique<D3Module>(m_dialog.get());
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void D3Block::CreateBlockPorts()
{
    uint8_t size = m_module->CountInputPort();
    QVector<IPort*> ports;

    for (uint8_t i = 0; i < size; ++i)
        ports.push_back(m_module->GetInput(i));

    CreatePorts(ports);
}

void D3Block::DrawImage(QPainter *painter)
{
    const QRectF& rect = GetborderRect();
    painter->drawEllipse(static_cast<int>(rect.left()) + 8, static_cast<int>(rect.bottom()) - 12, 8, 8);
    painter->drawEllipse(static_cast<int>(rect.right()) - 16, static_cast<int>(rect.bottom()) - 12, 8, 8);
    painter->setBrush(GradientImage());
    QRectF rectc = GetborderRect();
    rectc.moveCenter(QPointF(GetborderRect().center().x(),GetborderRect().center().y()+ 20));
    QPainterPath path;
    path.moveTo(rectc.center());
    path.arcTo(rectc, 45, 90);
    painter->drawPath(path);
    painter->drawLine(rectc.center(), QPointF(32, 20));
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock*PluginBlock::LoudBlock() const
{
    return new D3Block;
}

void D3Block::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
