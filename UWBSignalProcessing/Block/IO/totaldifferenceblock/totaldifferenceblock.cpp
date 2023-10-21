#include "totaldifferenceblock.h"

TotalTifferenceBlock::TotalTifferenceBlock()
    : IOBlock("Sum-Diff"), m_module(nullptr) {
  setToolTip("Модуль суммарно разностного преобразования");
}

AbstractBlock *TotalTifferenceBlock::Clone() const {
  return new TotalTifferenceBlock();
}

AbstractModule *TotalTifferenceBlock::GetModule() const {
  return m_module.get();
}

QString TotalTifferenceBlock::GetType() const { return "TotalTifferenceBlock"; }

void TotalTifferenceBlock::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void TotalTifferenceBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "dX sX");
  painter->setFont(QFont());
}

void TotalTifferenceBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<TotalTifferenceModule>();
  CreateBlockPorts();
}

void TotalTifferenceBlock::CreateBlockPorts() {
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
  return new TotalTifferenceBlock;
}

void TotalTifferenceBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
