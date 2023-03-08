#include "3dtargetblock.h"

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
D3TargetBlock::D3TargetBlock():
    AbstractScanBlock("3D Target")
{
    setToolTip("3D Target");
}

AbstractBlock* D3TargetBlock::Clone() const
{
    return new D3TargetBlock();
}

AbstractModule* D3TargetBlock::GetModule() const
{
    return m_module.get();
}

QString D3TargetBlock::GetType() const
{
    return "D3TargetBlock";
}

void D3TargetBlock::Change()
{
    if (m_module && m_dialog){}
}

void D3TargetBlock::InitBlock(QWidget*)
{
    m_dialog = std::make_unique<D3TargetDialogBox>();

    m_module = std::make_unique<D3TargetModule>(m_dialog.get());
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void D3TargetBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountInputPort();
    QVector<IPort*> ports;

    for (uint8_t i = 0; i < size; ++i)
        ports.push_back(m_module->GetInput(i));

    CreatePorts(ports);
}

void D3TargetBlock::DrawImage(QPainter *painter)
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
    return new D3TargetBlock;
}

void D3TargetBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
