#include "logicblock.h"

LogicBlock::LogicBlock() : IOBlock("Logic") {
  setToolTip("Принимет на вход два сигнала, выдает то первый пропуская второй, "
             "то второй пропуская первый");
}

AbstractBlock *LogicBlock::Clone() const { return new LogicBlock(); }

AbstractModule *LogicBlock::GetModule() const { return m_module.get(); }

QString LogicBlock::GetType() const { return "LogicBlock"; }

void LogicBlock::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void LogicBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "||");
  painter->setFont(QFont());
}

void LogicBlock::ChangeCountPorts() { CreateBlockPorts(); }

void LogicBlock::InitBlock(QWidget *) {

  m_module = std::make_unique<LogicModule>();
  CreateBlockPorts();
  SetAbstractDialogCommand(nullptr);
}

void LogicBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new LogicBlock; }

void LogicBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
