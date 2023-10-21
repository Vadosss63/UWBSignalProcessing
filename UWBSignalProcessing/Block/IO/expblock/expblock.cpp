#include "expblock.h"

ExpBlock::ExpBlock() : IOBlock("exp") {
  setToolTip("производит вычесление exp(x)");
}

AbstractBlock *ExpBlock::Clone() const { return new ExpBlock(); }

AbstractModule *ExpBlock::GetModule() const { return m_module.get(); }

QString ExpBlock::GetType() const { return "ExpBlock"; }

void ExpBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetParameters(m_dialog->GetThreshold());
  }
}

void ExpBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ExpBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 8));
  painter->drawText(rect, Qt::AlignCenter, "EXP(x)");
  painter->setFont(QFont());
}

void ExpBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ExpModule>();
  m_dialog = std::make_unique<ExpDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ExpBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new ExpBlock; }

void ExpBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
