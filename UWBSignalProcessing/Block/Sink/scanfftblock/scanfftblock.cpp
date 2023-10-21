#include "scanfftblock.h"

AbstractScanBlock::AbstractScanBlock(const QString &title) : SinkBlock(title) {
  m_gradientImage.setColorAt(0, Qt::white);
  m_gradientImage.setColorAt(1, Qt::gray);
  m_gradientImage.setAngle(52);
  m_gradientImage.setCenter(GetborderRect().center());
}

void AbstractScanBlock::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void AbstractScanBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->drawEllipse(static_cast<int>(rect.left()) + 8,
                       static_cast<int>(rect.bottom()) - 12, 8, 8);
  painter->drawEllipse(static_cast<int>(rect.right()) - 16,
                       static_cast<int>(rect.bottom()) - 12, 8, 8);
  painter->setBrush(m_gradientImage);
  QPointF center(GetborderRect().center());
  painter->drawEllipse(center, 28, 28);
  painter->drawEllipse(center, 18, 18);
  painter->drawEllipse(center, 8, 8);
  painter->drawLine(center, QPointF(52, 13));
}

QConicalGradient &AbstractScanBlock::GradientImage() { return m_gradientImage; }

//////////////////
ScanfftBlock::ScanfftBlock() : AbstractScanBlock("Scanfft") {
  setToolTip("Scanfft");
}

AbstractBlock *ScanfftBlock::Clone() const { return new ScanfftBlock(); }

AbstractModule *ScanfftBlock::GetModule() const { return m_module.get(); }

QString ScanfftBlock::GetType() const { return "ScanfftBlock"; }

void ScanfftBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetIsLog(m_dialog->GetIsLog());
    m_module->SetIsVerticalLine(m_dialog->GetIsVerticalPloter());
    m_module->SetPloterLine(m_dialog->GetLinePloter());
  }
}

void ScanfftBlock::InitBlock(QWidget *wparent) {
  m_module = std::make_unique<ScanfftModule>();
  m_dialog = std::make_unique<ScanfftDialogBox>(wparent);
  m_module->SetUpdateScane(m_dialog.get());
  CreateBlockPorts();
  // класс для отрисовки поля параметров
  SetAbstractDialogCommand(m_dialog.get());
}

void ScanfftBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountInputPort();
  QVector<IPort *> ports;

  for (uint8_t i = 0; i < size; ++i)
    ports.push_back(m_module->GetInput(i));

  CreatePorts(ports);
}

void ScanfftBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->drawEllipse(static_cast<int>(rect.left()) + 8,
                       static_cast<int>(rect.bottom()) - 12, 8, 8);
  painter->drawEllipse(static_cast<int>(rect.right()) - 16,
                       static_cast<int>(rect.bottom()) - 12, 8, 8);
  painter->setBrush(GradientImage());
  QRectF rectc = GetborderRect();
  rectc.moveCenter(
      QPointF(GetborderRect().center().x(), GetborderRect().center().y() + 20));
  QPainterPath path;
  path.moveTo(rectc.center());
  path.arcTo(rectc, 45, 90);
  painter->drawPath(path);
  painter->drawLine(rectc.center(), QPointF(32, 20));
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new ScanfftBlock; }

void ScanfftBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
