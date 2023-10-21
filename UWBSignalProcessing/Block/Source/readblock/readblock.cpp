#include "readblock.h"
#include <QTimer>

ReadBlock::ReadBlock() : SourceBlock("Read") {
  setToolTip("Производит чтение сигнала из файла");
}

AbstractModule *ReadBlock::GetModule() const { return m_module.get(); }

AbstractBlock *ReadBlock::Clone() const { return new ReadBlock; }

QString ReadBlock::GetType() const { return "ReadBlock"; }

void ReadBlock::Change() {
  if (m_dialog && m_module) {
    m_module->SetPathFile(m_dialog->GetPathFile());
  }
}

void ReadBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ReadBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "Чтение\nиз файла");
  painter->setFont(QFont());
}

void ReadBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ReadModule>();
  m_dialog = std::make_unique<ReadDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ReadBlock::CreateBlockPorts() {
  QVector<OPort *> ports;
  uint8_t size = m_module->CountOutputPort();
  for (uint8_t i = 0; i < size; ++i)
    ports.push_back(m_module->GetOutput(i));
  CreatePorts(ports);
}

void ReadBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new ReadBlock; }
