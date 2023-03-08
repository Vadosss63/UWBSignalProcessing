#include "scansceneblock.h"
#include <QLinearGradient>

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
    painter->drawEllipse(static_cast<int>(rect.right()) - 16,  static_cast<int>(rect.bottom()) - 12, 8, 8);
    painter->setBrush(m_gradientImage);
    QPointF center(GetborderRect().center());
    painter->drawEllipse(center, 28, 28);
    painter->drawEllipse(center, 18, 18);
    painter->drawEllipse(center, 8, 8);
    painter->drawLine(center, QPointF(52, 13));
}

QConicalGradient AbstractScanBlock::GradientImage() const
{
    return m_gradientImage;
}

//////////////////
ScanSceneBlock::ScanSceneBlock():
    AbstractScanBlock("ScanScene")
{
    setToolTip("Выводит радиолокационное изображение");
}

AbstractBlock *ScanSceneBlock::Clone() const
{
    return new ScanSceneBlock();
}

AbstractModule* ScanSceneBlock::GetModule() const
{
    return m_module.get();
}

QString ScanSceneBlock::GetType() const
{
    return "ScanSceneBlock";
}

void ScanSceneBlock::Change()
{
    if (m_moduleVrli && m_dialog) {
        m_moduleVrli->SetCoeffRange(static_cast<float>(m_dialog->GetCoefStreatch()) / 2);
        m_dialog->ClearVRLI();
    }
}

void ScanSceneBlock::InitBlock(QWidget* wparent)
{
    m_module = std::make_unique<ScanBufferSceneModule>();
    m_moduleVrli = std::make_unique<VRLIViewModule>();
    m_dialog = std::make_unique<ScanSceneDialogBox>(m_module.get(), wparent);
    m_dialog->SetVRLIModule(m_moduleVrli.get());
    CreateBlockPorts();
    // класс для отрисовки поля параметров
    SetAbstractDialogCommand(m_dialog.get());
}

void ScanSceneBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountInputPort();
    QVector<IPort*> ports;
    for (uint8_t i = 0; i < size; ++i)
        ports.push_back(m_module->GetInput(i));

    uint8_t sizeVRLI = m_module->CountInputPort();
    for (uint8_t i = 0; i < sizeVRLI; ++i)
        ports.push_back(m_moduleVrli->GetInput(i));

    CreatePorts(ports);
}

void ScanSceneBlock::DrawImage(QPainter *painter)
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
    return new ScanSceneBlock;
}

void ScanSceneBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);

    if(m_moduleVrli)
        m_moduleVrli->RegistrOperationManager(operationManager);
}
