#include "accumulationdialogbox.h"

AccumulationDialogBox::AccumulationDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_widget = new CustomDialWidget("Число накоплений сигнала");
  m_widget->setMaxVal(1024);
  m_widget->setMinVal(1);
  m_gridLayoout->addWidget(m_widget, 0, 0, 1, 2);
  m_gridLayoout->addWidget(new QLabel("Тип накопления"), 1, 0);
  m_comboBox = new QComboBox;
  m_comboBox->addItem("Простое накопление");
  m_comboBox->addItem("Простое с отправкой накопление");
  m_comboBox->addItem("Цикличное накопление");
  m_gridLayoout->addWidget(m_comboBox, 1, 1);

  SetTopLayout(m_gridLayoout);
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
  m_widget->setValue(m_widget->maxVal());
  SendCommandAbstractModule();
}

size_t AccumulationDialogBox::GetSizeBuf() {
  return static_cast<size_t>(m_widget->getDisplayIntVal());
}

TypeAccumulation AccumulationDialogBox::GetTypeAccum() {
  return static_cast<TypeAccumulation>(m_comboBox->currentIndex());
}

QList<std::pair<QString, QVariant>> AccumulationDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_widget", m_widget->getDisplayIntVal()));
  listSetting.append(std::make_pair("m_comboBox", m_comboBox->currentIndex()));
  return listSetting;
}

bool AccumulationDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_widget")
      m_widget->setValue(parmSetting.second.toInt());

    if (parmSetting.first == "m_comboBox")
      m_comboBox->setCurrentIndex(parmSetting.second.toInt());
  }
  return true;
}
