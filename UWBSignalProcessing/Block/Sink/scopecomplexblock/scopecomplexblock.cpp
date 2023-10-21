#include "scopecomplexblock.h"

ScopeComplexBlock::ScopeComplexBlock() : SinkBlock("ScopeComplex") {
  setToolTip("Выводит сигнал на осциллограф");
}

AbstractBlock *ScopeComplexBlock::Clone() const {
  return new ScopeComplexBlock();
}

AbstractModule *ScopeComplexBlock::GetModule() const { return m_module.get(); }

QString ScopeComplexBlock::GetType() const { return "ScopeComplexBlock"; }

void ScopeComplexBlock::Change() {
  if (m_module && m_dialog) {
  }
}

void ScopeComplexBlock::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ScopeComplexBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  QRectF rectangle(rect.left() + 5, rect.top() + 10, rect.width() - 10,
                   rect.height() - 25);
  painter->drawRoundedRect(rectangle, 20.0, 15.0);

  painter->drawEllipse(rect.left() + 8, rect.bottom() - 12, 8, 8);

  painter->drawEllipse(rect.right() - 16, rect.bottom() - 12, 8, 8);
  QPainterPath path;
  path.moveTo(rect.left() + 5, (rect.height()) / 2);
  path.lineTo(rect.width() / 2 - 12, (rect.height()) / 2);

  path.cubicTo(rect.width() / 2 - 5, rect.top(), rect.width() / 2 - 1,
               (rect.height()) / 2, rect.width() / 2 + 2,
               (rect.height()) / 2 + 10);

  path.cubicTo(rect.width() / 2 + 8, rect.bottom() - 10, rect.width() / 2 + 9,
               (rect.height()) / 2, rect.width() / 2 + 14, (rect.height()) / 2);

  path.lineTo(rect.right() - 5, (rect.height()) / 2);

  painter->drawPath(path);
}

void ScopeComplexBlock::InitBlock(QWidget *wparent) {
  m_module = std::make_unique<ScopeComplexModule>();
  m_dialog = std::make_unique<ScopeComplexDialogBox>(m_module.get(), wparent);
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ScopeComplexBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountInputPort();
  QVector<IPort *> ports;
  for (uint8_t i = 0; i < size; ++i) {
    ports.push_back(m_module->GetInput(i));
  }
  CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new ScopeComplexBlock; }

void ScopeComplexBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
