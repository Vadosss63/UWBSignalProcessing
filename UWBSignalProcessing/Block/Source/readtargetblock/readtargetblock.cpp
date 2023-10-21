#include "readtargetblock.h"
#include <QTimer>

ReadTargetBlock::ReadTargetBlock() : SourceBlock("Read\nTarget") {
  setToolTip("Производит чтение целей из файла");
}

AbstractBlock *ReadTargetBlock::Clone() const { return new ReadTargetBlock(); }

AbstractModule *ReadTargetBlock::GetModule() const { return m_module.get(); }

QString ReadTargetBlock::GetType() const { return "ReadTargetBlock"; }

void ReadTargetBlock::Change() {
  if (m_dialog && m_module) {
    m_module->SetPathFile(m_dialog->GetPathFile());
  }
}

void ReadTargetBlock::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ReadTargetBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "Чтение\nTarget");
  painter->setFont(QFont());
}

void ReadTargetBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ReadTargetModule>();
  m_dialog = std::make_unique<ReadTargetDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ReadTargetBlock::CreateBlockPorts() {
  QVector<OPort *> ports;
  uint8_t size = m_module->CountOutputPort();
  for (uint8_t i = 0; i < size; ++i)
    ports.push_back(m_module->GetOutput(i));
  CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new ReadTargetBlock; }

void ReadTargetBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
