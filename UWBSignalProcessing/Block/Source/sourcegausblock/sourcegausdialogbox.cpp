#include "sourcegausdialogbox.h"

SourceGausDialogBox::SourceGausDialogBox() {
  QGridLayout *m_gridLayoout = new QGridLayout;

  CreateExpectationWidget();
  CreateAmplitudeWidget();
  CreateSigmaWidget();
  QGridLayout *layoutSampleTime = new QGridLayout;
  CreateSTWidget(layoutSampleTime);

  m_gridLayoout->addLayout(layoutSampleTime, 0, 0);
  m_gridLayoout->addWidget(m_expectation);
  m_gridLayoout->addWidget(m_amplitude);
  m_gridLayoout->addWidget(m_sigma);

  SetTopLayout(m_gridLayoout);
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());

  m_expectation->setValue(500);
  m_amplitude->setValue(200);
  m_sigma->setValue(3);
}

void SourceGausDialogBox::CreateExpectationWidget() {
  m_expectation = new CustomDialWidget("Матожидание");
  m_expectation->setMaxVal(1024);
  m_expectation->setMinVal(0);
}

void SourceGausDialogBox::CreateAmplitudeWidget() {
  m_amplitude = new CustomDialWidget("Амплитуда");
  m_amplitude->setMaxVal(500);
  m_amplitude->setMinVal(0);
}

void SourceGausDialogBox::CreateSigmaWidget() {
  m_sigma = new CustomDialWidget("Ширина");
  m_sigma->setMaxVal(1024);
  m_sigma->setMinVal(0);
}

void SourceGausDialogBox::CreateSTWidget(QGridLayout *layoutSampleTime) {
  QLabel *labelTS = new QLabel("Частота дискретизации:");
  m_comboBoxTS = new QComboBox;
  m_comboBoxTS->addItem("0.5 ГГц");
  m_comboBoxTS->addItem("2 ГГц");
  m_comboBoxTS->addItem("16 ГГц");
  layoutSampleTime->addWidget(labelTS, 0, 0);
  layoutSampleTime->addWidget(m_comboBoxTS, 0, 1);
  connect(m_comboBoxTS, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetMaxRangeFrequecy(int)));
}

int SourceGausDialogBox::GetLenght() {
  int len = 2048;
  switch (m_comboBoxTS->currentIndex()) {
  case 0:
    len = 2048;
    break;
  case 1:
    len = 8192;
    break;
  case 2:
    len = 65536;
    break;
  }
  return len;
}

int SourceGausDialogBox::GetAmplitude() {
  return m_amplitude->getDisplayIntVal();
}

int SourceGausDialogBox::GetSigma() { return m_sigma->getDisplayIntVal(); }

int SourceGausDialogBox::GetExpectation() {
  return m_expectation->getDisplayIntVal();
}

void SourceGausDialogBox::SetMaxRangeFrequecy(int index) {
  if (index == 2)
    m_expectation->setMaxVal(32768);
  else if (index == 1)
    m_expectation->setMaxVal(4096);
  else
    m_expectation->setMaxVal(1024);
}

QList<std::pair<QString, QVariant>> SourceGausDialogBox::GetSetting() {

  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(
      std::make_pair("m_comboBoxTS", m_comboBoxTS->currentIndex()));
  listSetting.append(
      std::make_pair("m_amplitude", m_amplitude->getDisplayIntVal()));
  listSetting.append(std::make_pair("m_sigma", m_sigma->getDisplayIntVal()));
  listSetting.append(
      std::make_pair("m_expectation", m_expectation->getDisplayIntVal()));
  return listSetting;
}

bool SourceGausDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 4)
    return false;

  foreach (auto parmSetting, listSetting) {

    if (parmSetting.first == "m_comboBoxTS")
      m_comboBoxTS->setCurrentIndex(parmSetting.second.toInt());
    if (parmSetting.first == "m_amplitude")
      m_amplitude->setValue(parmSetting.second.toInt());
    if (parmSetting.first == "m_sigma")
      m_sigma->setValue(parmSetting.second.toInt());
    if (parmSetting.first == "m_expectation")
      m_expectation->setValue(parmSetting.second.toInt());
  }
  return true;
}
