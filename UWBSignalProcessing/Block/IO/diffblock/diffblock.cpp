#include "diffblock.h"

DiffBlock::DiffBlock() : IOBlock("Diff") {
  setToolTip("Выполняет дифферинцирование функции");
}

AbstractBlock *DiffBlock::Clone() const { return new DiffBlock(); }

AbstractModule *DiffBlock::GetModule() const { return m_module.get(); }

QString DiffBlock::GetType() const { return "DiffBlock"; }

void DiffBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetDelta_x(m_dialog->GetDeltaX());
  }
}

void DiffBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void DiffBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  QFont f("Times", 10);
  painter->setFont(f);
  painter->drawText(rect, Qt::AlignCenter, "Diff");
  painter->setFont(QFont());
}

void DiffBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<DiffModule>();
  m_dialog = std::make_unique<DiffDialogBox>();

  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void DiffBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new DiffBlock; }

void DiffBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
