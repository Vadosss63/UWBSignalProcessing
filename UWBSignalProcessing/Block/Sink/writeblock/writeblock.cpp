#include "writeblock.h"

WriteBlock::WriteBlock() : SinkBlock("Write") {
  setToolTip("Выполняет запись сигнала\n в указанный файл");
}

AbstractBlock *WriteBlock::Clone() const { return new WriteBlock(); }

AbstractModule *WriteBlock::GetModule() const { return m_module.get(); }

QString WriteBlock::GetType() const { return "WriteBlock"; }

void WriteBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetPathFile(m_dialog->GetPathFile());
    m_module->SetTypeWrite(m_dialog->GetTypeWrite());
  }
}

void WriteBlock::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void WriteBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  QFont f("Times", 10);
  painter->setFont(f);
  painter->drawText(rect, Qt::AlignCenter, "Запись\nв файл");
  painter->setFont(QFont());
}

void WriteBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountInputPort();
  QVector<IPort *> ports;
  for (uint8_t i = 0; i < size; ++i) {
    ports.push_back(m_module->GetInput(i));
  }
  CreatePorts(ports);
}

void WriteBlock::InitBlock(QWidget *) {

  m_module = std::make_unique<WriteModule>();
  m_dialog = std::make_unique<WriteDialogBox>();

  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new WriteBlock; }

void WriteBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
