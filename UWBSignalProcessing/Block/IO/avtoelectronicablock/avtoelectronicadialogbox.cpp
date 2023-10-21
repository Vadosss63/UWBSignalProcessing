#include "avtoelectronicadialogbox.h"
#include <qmath.h>

QList<std::pair<QString, QVariant>> AvtoelectronicaDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("numberRange", m_numberRange->value()));
  listSetting.append(std::make_pair("countAcumm", m_countAcumm->value()));
  listSetting.append(std::make_pair("range", m_range->value()));
  listSetting.append(std::make_pair("threshol", m_threshol->value()));
  listSetting.append(
      std::make_pair("m_thresholDynamic", m_thresholDynamic->value()));
  listSetting.append(std::make_pair("baseAntenna", m_baseAntenna->value()));
  listSetting.append(std::make_pair("maxSpeed", m_maxSpeed->value()));
  listSetting.append(std::make_pair("varu", m_varu->isChecked()));
  listSetting.append(std::make_pair("m_gain", m_gain->value()));
  listSetting.append(std::make_pair("m_gainDynamic", m_gainDynamic->value()));
  return listSetting;
}

bool AvtoelectronicaDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() < 10)
    return false;

  for (auto &val : listSetting) {
    if (val.first == "numberRange")
      m_numberRange->setValue(val.second.toInt());

    if (val.first == "countAcumm")
      m_countAcumm->setValue(val.second.toInt());

    if (val.first == "range")
      m_range->setValue(val.second.toInt());

    if (val.first == "threshol")
      m_threshol->setValue(val.second.toInt());

    if (val.first == "m_thresholDynamic")
      m_thresholDynamic->setValue(val.second.toInt());

    if (val.first == "baseAntenna")
      m_baseAntenna->setValue(val.second.toDouble());

    if (val.first == "maxSpeed")
      m_maxSpeed->setValue(val.second.toInt());

    if (val.first == "varu")
      m_varu->setChecked(val.second.toBool());

    if (val.first == "m_gain")
      m_gain->setValue(val.second.toDouble());
    if (val.first == "m_gainDynamic")
      m_gainDynamic->setValue(val.second.toDouble());
  }

  return true;
}

size_t AvtoelectronicaDialogBox::GetNumberRange() {
  return static_cast<size_t>(m_numberRange->value());
}

size_t AvtoelectronicaDialogBox::GetCountAcumm() {
  return static_cast<size_t>(m_countAcumm->value());
}

size_t AvtoelectronicaDialogBox::GetRangeAcumm() {
  return static_cast<size_t>(m_range->value());
}

double AvtoelectronicaDialogBox::GetThresholAcumm() {
  return static_cast<double>(m_threshol->value());
}

double AvtoelectronicaDialogBox::GetThresholAcummDynamic() {
  return static_cast<double>(m_thresholDynamic->value());
}

double AvtoelectronicaDialogBox::GetBaseAntenna() {
  return m_baseAntenna->value();
}

int AvtoelectronicaDialogBox::GetMaxSpeed() { return m_maxSpeed->value(); }

bool AvtoelectronicaDialogBox::IsVARU() { return m_varu->isChecked(); }

double AvtoelectronicaDialogBox::GetGain() { return m_gain->value(); }

double AvtoelectronicaDialogBox::GetGainDynamic() {
  return m_gainDynamic->value();
}

void AvtoelectronicaDialogBox::CreateWidget() {
  auto layout = new QGridLayout;

  m_numberRange = new QSpinBox;
  m_countAcumm = new QSpinBox;
  m_range = new QSpinBox;

  m_threshol = new QSpinBox;
  m_threshol->setRange(0, 2048);

  m_thresholDynamic = new QSpinBox;
  m_thresholDynamic->setRange(0, 2048);

  int maxRange = 8192;

  m_numberRange->setRange(0, maxRange - 1);
  m_range->setRange(1, maxRange);
  m_range->setValue(1);

  connect(m_numberRange, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int i) { m_range->setMaximum(maxRange - i); });

  m_countAcumm->setRange(1, 1024);

  layout->addWidget(new QLabel("Накопление "), 0, 0);
  layout->addWidget(m_countAcumm, 0, 1);
  layout->addWidget(new QLabel("Начальная дальность "), 0, 2);
  layout->addWidget(m_numberRange, 0, 3);
  layout->addWidget(new QLabel("Количество "), 0, 4);
  layout->addWidget(m_range, 0, 5);

  m_baseAntenna = new QDoubleSpinBox;
  m_baseAntenna->setRange(0, 2.0);
  m_baseAntenna->setDecimals(3);
  m_baseAntenna->setValue(0.328);
  layout->addWidget(new QLabel("База антенны "), 1, 0);
  layout->addWidget(m_baseAntenna, 1, 1);

  m_maxSpeed = new QSpinBox;
  m_maxSpeed->setRange(1, 100);
  m_maxSpeed->setValue(60);

  layout->addWidget(new QLabel("Максимальная скорость"), 1, 2);
  layout->addWidget(m_maxSpeed, 1, 3);

  layout->addWidget(new QLabel("Порог "), 1, 4);
  layout->addWidget(m_threshol, 1, 5);
  m_varu = new QCheckBox("ВАРУ");
  layout->addWidget(m_varu, 2, 0);

  m_gain = new QDoubleSpinBox;
  m_gain->setRange(-1000.0, 1000.0);
  m_gain->setValue(1.0);
  m_gain->setDecimals(6);
  layout->addWidget(new QLabel("Усиление "), 2, 1);
  layout->addWidget(m_gain, 2, 2);

  m_gainDynamic = new QDoubleSpinBox;
  m_gainDynamic->setRange(-1000.0, 1000.0);
  m_gainDynamic->setValue(1.0);
  m_gainDynamic->setDecimals(6);
  layout->addWidget(new QLabel("Усиление и порог СДЦ "), 2, 3);
  layout->addWidget(m_gainDynamic, 2, 4);

  // layout->addWidget(new QLabel("Порог СДЦ"), 2, 4);
  layout->addWidget(m_thresholDynamic, 2, 5);

  SetTopLayout(layout);
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

AvtoelectronicaDialogBox::AvtoelectronicaDialogBox() { CreateWidget(); }
