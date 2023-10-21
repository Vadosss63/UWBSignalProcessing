#include "controlframesblock.h"

ControlFramesBlock::ControlFramesBlock() : IOBlock("Контроль\n кадров") {}

AbstractBlock *ControlFramesBlock::Clone() const {
  return new ControlFramesBlock();
}

AbstractModule *ControlFramesBlock::GetModule() const { return m_module.get(); }

QString ControlFramesBlock::GetType() const { return "ControlFramesBlock"; }

void ControlFramesBlock::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ControlFramesBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "контроль\nкадров");
  painter->setFont(QFont());
}

void ControlFramesBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ControlFramesModule>();
  CreateBlockPorts();
}

void ControlFramesBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new ControlFramesBlock; }

void ControlFramesBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
