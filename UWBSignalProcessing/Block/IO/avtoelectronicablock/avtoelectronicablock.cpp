#include "avtoelectronicablock.h"

AvtoelectronicaBlock::AvtoelectronicaBlock() : IOBlock("AVTO") {
  setToolTip("Модуль для обработки автоэлектроники");
}

AbstractBlock *AvtoelectronicaBlock::Clone() const {
  return new AvtoelectronicaBlock();
}

AbstractModule *AvtoelectronicaBlock::GetModule() const {
  return m_module.get();
}

QString AvtoelectronicaBlock::GetType() const { return "AvtoelectronicaBlock"; }

void AvtoelectronicaBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetBaseAntenna(m_dialog->GetBaseAntenna());
    m_module->SetMaxSpeed(m_dialog->GetMaxSpeed());

    m_module->SetIsVARU(m_dialog->IsVARU());
    m_module->SetGain(m_dialog->GetGain());
    m_module->SetGainDynamic(m_dialog->GetGainDynamic());
    m_module->SetSetting(m_dialog->GetCountAcumm(), m_dialog->GetNumberRange(),
                         m_dialog->GetRangeAcumm(),
                         m_dialog->GetThresholAcumm(),
                         m_dialog->GetThresholAcummDynamic());
  }
}

void AvtoelectronicaBlock::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void AvtoelectronicaBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 8));
  painter->drawText(rect, Qt::AlignCenter, "Get Range");
  painter->setFont(QFont());
}

void AvtoelectronicaBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<AvtoelectronicaModule>();
  m_dialog = std::make_unique<AvtoelectronicaDialogBox>();
  CreateBlockPorts();
  // класс для отрисовки поля параметров
  SetAbstractDialogCommand(m_dialog.get());
}

void AvtoelectronicaBlock::CreateBlockPorts() {
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
  return new AvtoelectronicaBlock;
}

void AvtoelectronicaBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
