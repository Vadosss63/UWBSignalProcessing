#include "syntheticapertureblock.h"

SyntheticApertureBlock::SyntheticApertureBlock() : IOBlock("Aperture") {
  setToolTip("Выполняет цифровой синтез апертуры");
}

AbstractBlock *SyntheticApertureBlock::Clone() const {
  return new SyntheticApertureBlock();
}

AbstractModule *SyntheticApertureBlock::GetModule() const {
  return m_module.get();
}

QString SyntheticApertureBlock::GetType() const {
  return "SyntheticApertureBlock";
}

void SyntheticApertureBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetIndexStop(m_dialog->GetIndexStop());
  }
}

void SyntheticApertureBlock::paint(QPainter *painter,
                                   const QStyleOptionGraphicsItem *option,
                                   QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void SyntheticApertureBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 8));
  painter->drawText(rect, Qt::AlignCenter, "DSA");
  painter->setFont(QFont());
}

void SyntheticApertureBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<SyntheticApertureModule>();
  m_dialog = std::make_unique<SyntheticApertureDialogBox>();
  CreateBlockPorts();
  // класс для отрисовки поля параметров
  SetAbstractDialogCommand(m_dialog.get());
}

void SyntheticApertureBlock::CreateBlockPorts() {
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
  return new SyntheticApertureBlock;
}

void SyntheticApertureBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
