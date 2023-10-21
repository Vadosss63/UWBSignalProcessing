#include "singeneratordialogbox.h"

SinGeneratorDialogBox::SinGeneratorDialogBox() {
  m_gridLayoout = new QGridLayout;
  CreateFrequencyWidget();
  CreateAmplitudeWidget();
  CreatePhaseWidget();

  QGridLayout *layoutSampleTime = new QGridLayout;
  CreateSTWidget(layoutSampleTime);

  m_gridLayoout->addLayout(layoutSampleTime, 0, 0);
  m_gridLayoout->addWidget(m_frequency);
  m_gridLayoout->addWidget(m_amplitude);
  m_gridLayoout->addWidget(m_phase);

  SetTopLayout(m_gridLayoout);
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
  m_amplitude->setValue(1);
}

void SinGeneratorDialogBox::CreateFrequencyWidget() {
  m_frequency = new CustomDialWidget("Частота, МГц");
  m_frequency->setMaxVal(249);
  m_frequency->setMinVal(1);
}

void SinGeneratorDialogBox::CreateAmplitudeWidget() {
  m_amplitude = new CustomDialWidget("Амплитуда");
  m_amplitude->setMaxVal(1000);
  m_amplitude->setMinVal(0);
}

void SinGeneratorDialogBox::CreatePhaseWidget() {
  m_phase = new CustomDialWidget("Фаза, град.");
  m_phase->setMaxVal(360);
  m_phase->setMinVal(0);
}

void SinGeneratorDialogBox::CreateSTWidget(QGridLayout *layoutSampleTime) {
  QLabel *labelTS = new QLabel("Частота дискретизации:");
  m_comboBoxTS = new QComboBox;
  m_comboBoxTS->addItem("0.5 ГГц");
  m_comboBoxTS->addItem("2 ГГц");
  layoutSampleTime->addWidget(labelTS, 0, 0);
  layoutSampleTime->addWidget(m_comboBoxTS, 0, 1);
  connect(m_comboBoxTS, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetMaxRangeFrequecy(int)));
}

void SinGeneratorDialogBox::SetMaxRangeFrequecy(int index) {
  if (index)
    m_frequency->setMaxVal(999);
  else
    m_frequency->setMaxVal(249);
}

QList<std::pair<QString, QVariant>> SinGeneratorDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(
      std::make_pair("m_comboBoxTS", m_comboBoxTS->currentIndex()));
  listSetting.append(
      std::make_pair("m_frequency", m_frequency->getDisplayIntVal()));
  listSetting.append(
      std::make_pair("m_amplitude", m_amplitude->getDisplayIntVal()));
  listSetting.append(std::make_pair("m_phase", m_phase->getDisplayIntVal()));
  return listSetting;
}

bool SinGeneratorDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 4)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_comboBoxTS")
      m_comboBoxTS->setCurrentIndex(parmSetting.second.toInt());
    if (parmSetting.first == "m_frequency")
      m_frequency->setValue(parmSetting.second.toInt());
    if (parmSetting.first == "m_amplitude")
      m_amplitude->setValue(parmSetting.second.toInt());
    if (parmSetting.first == "m_phase")
      m_phase->setValue(parmSetting.second.toInt());
  }
  return true;
}

double SinGeneratorDialogBox::GetFrequency() const {
  return m_frequency->getDisplayIntVal();
}

double SinGeneratorDialogBox::GetAmplitude() const {
  return m_amplitude->getDisplayIntVal();
}

double SinGeneratorDialogBox::GetPhase() const {
  return m_phase->getDisplayIntVal();
}

bool SinGeneratorDialogBox::GetSignalSize() const {
  return m_comboBoxTS->currentIndex();
}
