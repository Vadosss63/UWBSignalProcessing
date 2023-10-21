#include "adcnewblock.h"

ADCNewBlock::ADCNewBlock() : SourceBlock("АЦП new") {
  setToolTip(
      "Получет информацию от группы АЦП и\nотправляет ее на I и Q порты");
}

ADCNewBlock::~ADCNewBlock() { ResetBlock(); }

AbstractBlock *ADCNewBlock::Clone() const { return new ADCNewBlock(); }

AbstractModule *ADCNewBlock::GetModule() const { return m_module; }

QString ADCNewBlock::GetType() const { return "ADCNewBlock"; }

void ADCNewBlock::Change() {
  if (m_module && m_dialog) {

    m_module->SetFormatAngel(m_dialog->GetFormatAngel());
    m_module->SetAdcMode(m_dialog->GetAdcSync(), m_dialog->GetAdcMode(),
                         m_dialog->GetSampleNum(), m_dialog->GetAverNum(),
                         m_dialog->GetAverMode());
    m_module->SetShiftQ(m_dialog->GetShiftQ());

    m_module->SetShiftLevelI(m_dialog->GetShiftLevelI());
    m_module->SetShiftLevelQ(m_dialog->GetShiftLevelQ());
    m_module->SetShiftLevelI(m_dialog->GetShiftLevelI());
    m_module->SetShiftLevelQ(m_dialog->GetShiftLevelQ());

    if (m_dialog->isImitationMode()) {
      m_module->SetImitationMode(m_dialog->GetX(), m_dialog->GetY(),
                                 m_dialog->GetV());
      m_module->SetIsOneRun(m_dialog->isOneRun());
    } else {
      m_module->SetNormalMode();
    }
  }
}

void ADCNewBlock::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ADCNewBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->drawText(rect, Qt::AlignCenter, "АЦП");
  return;
}

void ADCNewBlock::ResetBlock() {
  if (m_module)
    delete m_module;

  if (m_dialog)
    delete m_dialog;

  m_module = nullptr;
  m_dialog = nullptr;
}

void ADCNewBlock::InitBlock(QWidget *) {
  ResetBlock();
  m_module = new ADCNewModule();
  m_dialog = new ADCNewDialogBox();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog);
}

void ADCNewBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountOutputPort();
  QVector<OPort *> ports;
  for (uint8_t i = 0; i < size; ++i)
    ports.push_back(m_module->GetOutput(i));

  CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new ADCNewBlock; }

void ADCNewBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
