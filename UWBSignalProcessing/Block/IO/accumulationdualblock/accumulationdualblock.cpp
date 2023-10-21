#include "accumulationdualblock.h"

AccumulationDualBlock::AccumulationDualBlock()
    : IOBlock("ЧП Накопл. "), m_image(4), m_module(nullptr) {
  setToolTip(
      "Производит сумирование сигнала первые n раз, \n и вычитание последних n "
      "раз \n после чего происходит выталкивание \n среднего значения");
}

AbstractBlock *AccumulationDualBlock::Clone() const {
  return new AccumulationDualBlock();
}

AbstractModule *AccumulationDualBlock::GetModule() const {
  return m_module.get();
}

QString AccumulationDualBlock::GetType() const {
  return "AccumulationDualBlock";
}

void AccumulationDualBlock::Change() {
  if (m_module && m_dialog) {
  }
}

void AccumulationDualBlock::paint(QPainter *painter,
                                  const QStyleOptionGraphicsItem *option,
                                  QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void AccumulationDualBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "ЧПН");
  painter->setFont(QFont());
}

void AccumulationDualBlock::InitBlock(QWidget *wparent) {

  m_module = std::make_unique<AccumulationDualModule>();
  m_dialog =
      std::make_unique<AccumulationDialogBoxDual>(m_module.get(), wparent);
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void AccumulationDualBlock::CreateBlockPorts() {
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
  return new AccumulationDualBlock;
}

void AccumulationDualBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
