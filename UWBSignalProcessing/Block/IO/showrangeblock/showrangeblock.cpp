#include "showrangeblock.h"

ShowRangeBlock::ShowRangeBlock() : IOBlock("Range") {
  setToolTip("выводит все углы на одной дальности");
}

AbstractBlock *ShowRangeBlock::Clone() const { return new ShowRangeBlock(); }

AbstractModule *ShowRangeBlock::GetModule() const { return m_module.get(); }

QString ShowRangeBlock::GetType() const { return "ShowRangeBlock"; }

void ShowRangeBlock::Change() {
  if (m_module && m_dialog)
    m_module->SetRange(static_cast<size_t>(m_dialog->GetRange()));
}

void ShowRangeBlock::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ShowRangeBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "Range");
  painter->setFont(QFont());
}

void ShowRangeBlock::ChangeCountPorts() { CreateBlockPorts(); }

void ShowRangeBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ShowRangeModule>();
  m_dialog = std::make_unique<ShowRangeDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ShowRangeBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new ShowRangeBlock; }

void ShowRangeBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
