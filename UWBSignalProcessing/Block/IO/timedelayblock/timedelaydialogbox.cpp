#include "timedelaydialogbox.h"

TimeDelayDialogBox::TimeDelayDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_widget = new CustomDialWidget("Задержка сигнала");
  m_widget->setMaxVal(512);
  m_widget->setMinVal(-512);
  m_gridLayoout->addWidget(m_widget);
  SetTopLayout(m_gridLayoout);
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

int TimeDelayDialogBox::GetNumberOfZeros() const {
  return m_widget->getDisplayIntVal();
}

QList<std::pair<QString, QVariant>> TimeDelayDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_widget", m_widget->getDisplayIntVal()));
  return listSetting;
}

bool TimeDelayDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_widget")
      m_widget->setValue(parmSetting.second.toInt());
  }
  return true;
}
