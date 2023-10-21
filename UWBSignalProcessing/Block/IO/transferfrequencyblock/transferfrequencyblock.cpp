#include "transferfrequencyblock.h"

TransferFrequencyBlock::TransferFrequencyBlock()
    : IOBlock("Перенос частоты"), m_module(nullptr) {
  setToolTip("Выполняет цифровой перенос сигнала на 500 МГц.");
}

AbstractBlock *TransferFrequencyBlock::Clone() const {
  return new TransferFrequencyBlock();
}

AbstractModule *TransferFrequencyBlock::GetModule() const {
  return m_module.get();
}

QString TransferFrequencyBlock::GetType() const {
  return "TransferFrequencyBlock";
}

void TransferFrequencyBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetTransferFrequency(m_dialog->GetTransferFrequency(),
                                   m_dialog->GetSampFreq());
  }
}

void TransferFrequencyBlock::paint(QPainter *painter,
                                   const QStyleOptionGraphicsItem *option,
                                   QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void TransferFrequencyBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->drawText(static_cast<int>(rect.left()) + 8,
                    static_cast<int>(rect.top()) + 20, "I");
  painter->drawText(static_cast<int>(rect.left()) + 5,
                    static_cast<int>(rect.bottom()) - 10, "Q");
  painter->drawText(static_cast<int>(rect.right()) - 19,
                    static_cast<int>(rect.top()) + 20, "Re");
  painter->drawText(static_cast<int>(rect.right()) - 19,
                    static_cast<int>(rect.bottom()) - 10, "Im");
}

void TransferFrequencyBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<TransferFrequencyModule>();
  m_dialog = std::make_unique<TransferFrequencyDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void TransferFrequencyBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const {
  return new TransferFrequencyBlock;
}

void TransferFrequencyBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
