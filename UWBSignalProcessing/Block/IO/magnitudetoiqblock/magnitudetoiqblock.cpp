#include "magnitudetoiqblock.h"

MagnitudetoIQBlock::MagnitudetoIQBlock() : IOBlock("MAtoIQ") {
  setToolTip("Делает преобразования фазы и\nспектра сигнала к I и Q каналу");
}

AbstractBlock *MagnitudetoIQBlock::Clone() const {
  return new MagnitudetoIQBlock();
}

AbstractModule *MagnitudetoIQBlock::GetModule() const { return m_module.get(); }

QString MagnitudetoIQBlock::GetType() const { return "MagnitudetoIQBlock"; }

void MagnitudetoIQBlock::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void MagnitudetoIQBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->drawText(static_cast<int>(rect.left()) + 10,
                    static_cast<int>(rect.top()) + 20, "A");
  painter->drawText(static_cast<int>(rect.left()) + 3,
                    static_cast<int>(rect.bottom()) - 10, "|M|");
  painter->drawText(static_cast<int>(rect.right()) - 12,
                    static_cast<int>(rect.top()) + 20, "I");
  painter->drawText(static_cast<int>(rect.right()) - 15,
                    static_cast<int>(rect.bottom()) - 10, "Q");
  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "icfft");
  painter->setFont(QFont());
}

void MagnitudetoIQBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<Magnitude_AngleModuleToIQModule>();
  CreateBlockPorts();
  SetAbstractDialogCommand(nullptr);
}

void MagnitudetoIQBlock::CreateBlockPorts() {
  uint8_t sizeIn = m_module->CountInputPort();
  QVector<IPort *> inPorts;
  for (uint8_t i = 0; i < sizeIn; ++i)
    inPorts.push_back(m_module->GetInput(i));

  uint8_t sizeOut = m_module->CountOutputPort();
  QVector<OPort *> outPorts;
  for (uint8_t i = 0; i < sizeOut; ++i)
    outPorts.push_back(m_module->GetOutput(i));

  CreatePorts(inPorts, outPorts);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new MagnitudetoIQBlock; }

void MagnitudetoIQBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
