#include "avtoelectronicarawblock.h"

AvtoelectronicaRawBlock::AvtoelectronicaRawBlock() : IOBlock("AVTO") {
  setToolTip(
      "Модуль формирования сырой радиолокационной информации автоэлектроники");
}

AbstractBlock *AvtoelectronicaRawBlock::Clone() const {
  return new AvtoelectronicaRawBlock();
}

AbstractModule *AvtoelectronicaRawBlock::GetModule() const {
  return m_module.get();
}

QString AvtoelectronicaRawBlock::GetType() const {
  return "AvtoelectronicaRawBlock";
}

void AvtoelectronicaRawBlock::Change() {
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

void AvtoelectronicaRawBlock::paint(QPainter *painter,
                                    const QStyleOptionGraphicsItem *option,
                                    QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void AvtoelectronicaRawBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 8));
  painter->drawText(rect, Qt::AlignCenter, "RAW");
  painter->setFont(QFont());
}

void AvtoelectronicaRawBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<AvtoelectronicaRawModule>();
  m_dialog = std::make_unique<AvtoelectronicaRawDialogBox>();
  CreateBlockPorts();
  // класс для отрисовки поля параметров
  SetAbstractDialogCommand(m_dialog.get());
}

void AvtoelectronicaRawBlock::CreateBlockPorts() {
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
  return new AvtoelectronicaRawBlock;
}

void AvtoelectronicaRawBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
