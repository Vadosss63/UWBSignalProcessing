#include "vrliudpblock.h"

VRLIUDPBlock::VRLIUDPBlock() : SinkBlock("ВРЛИ UDP"), m_module(nullptr) {
  setToolTip("Выполняет передачу данных по протоколу UDP");
}

AbstractBlock *VRLIUDPBlock::Clone() const { return new VRLIUDPBlock(); }

AbstractModule *VRLIUDPBlock::GetModule() const { return m_module.get(); }

QString VRLIUDPBlock::GetType() const { return "VRLIUDPBlock"; }

void VRLIUDPBlock::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void VRLIUDPBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  QFont f("Times", 10);
  painter->setFont(f);
  painter->drawText(rect, Qt::AlignCenter, "UDP");
  painter->setFont(QFont());
}

void VRLIUDPBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<VRLIUDPModule>();
  CreateBlockPorts();
}

void VRLIUDPBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountInputPort();
  QVector<IPort *> ports;
  for (uint8_t i = 0; i < size; ++i) {
    ports.push_back(m_module->GetInput(i));
  }
  CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new VRLIUDPBlock; }

void VRLIUDPBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
