#include "thresholdblock.h"

ThresholdBlock::ThresholdBlock() : IOBlock("Порог") {
  setToolTip("Ограничиваеит сигнал входной функциеей\nпервый порт "
             "ограничивающая функция\nвторой порт сигнал");
}

AbstractBlock *ThresholdBlock::Clone() const { return new ThresholdBlock(); }

AbstractModule *ThresholdBlock::GetModule() const { return m_module.get(); }

QString ThresholdBlock::GetType() const { return "ThresholdBlock"; }

void ThresholdBlock::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ThresholdBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 25));
  painter->drawText(rect, Qt::AlignCenter, "F(X)");
  painter->setFont(QFont());
}

void ThresholdBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ThresholdModule>();
  CreateBlockPorts();
}

void ThresholdBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new ThresholdBlock; }

void ThresholdBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
