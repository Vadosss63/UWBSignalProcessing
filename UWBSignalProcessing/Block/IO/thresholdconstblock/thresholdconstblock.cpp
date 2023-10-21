#include "thresholdconstblock.h"

ThresholdConstBlock::ThresholdConstBlock() : IOBlock("Пророг const") {
  setToolTip(
      "Опускает сигнал на знаяения парога/nи все что ниже нуля обнуляет");
}

AbstractBlock *ThresholdConstBlock::Clone() const {
  return new ThresholdConstBlock();
}

AbstractModule *ThresholdConstBlock::GetModule() const {
  return m_module.get();
}

QString ThresholdConstBlock::GetType() const { return "ThresholdConstBlock"; }

void ThresholdConstBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetParameters(m_dialog->GetThreshold());
  }
}

void ThresholdConstBlock::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ThresholdConstBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 8));
  painter->drawText(rect, Qt::AlignCenter, "F(x)\nconst");
  painter->setFont(QFont());
}

void ThresholdConstBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ThresholdModule>();
  m_dialog = std::make_unique<ThresholdDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ThresholdConstBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const {
  return new ThresholdConstBlock;
}

void ThresholdConstBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
