#include "controlantennsblock.h"

ControlAntennsBlock::ControlAntennsBlock() : IOBlock("Антенна") {}

AbstractBlock *ControlAntennsBlock::Clone() const {
  return new ControlAntennsBlock();
}

AbstractModule *ControlAntennsBlock::GetModule() const {
  return m_module.get();
}

QString ControlAntennsBlock::GetType() const { return "ControlAntennsBlock"; }

void ControlAntennsBlock::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ControlAntennsBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "А\nкадров");
  painter->setFont(QFont());
}

void ControlAntennsBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ControlAntennsModule>();
  CreateBlockPorts();
}

void ControlAntennsBlock::CreateBlockPorts() {
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
  return new ControlAntennsBlock;
}

void ControlAntennsBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
