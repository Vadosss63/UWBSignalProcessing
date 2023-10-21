#include "binblock.h"

BinBlock::BinBlock() : IOBlock("BinSig"), m_module(nullptr) {
  setToolTip("Блок формирования бинарного сигнала");
}

AbstractBlock *BinBlock::Clone() const { return new BinBlock(); }

AbstractModule *BinBlock::GetModule() const { return m_module.get(); }

QString BinBlock::GetType() const { return "BinBlock"; }

void BinBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetSettings(m_dialog->GetShiftI(), m_dialog->GetShiftQ(),
                          m_dialog->GetRangeUp(), m_dialog->GetRangeBottom());
  }
}

void BinBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void BinBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "Bin\nsignal");
  painter->setFont(QFont());
}

void BinBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<BinModule>();
  m_dialog = std::make_unique<BinDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void BinBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new BinBlock; }

void BinBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
