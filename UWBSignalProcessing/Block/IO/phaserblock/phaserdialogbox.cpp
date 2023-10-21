#include "phaserdialogbox.h"

PhaserDialogBox::PhaserDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_widget = new CustomDialWidget("Поворот фазы, град.");
  m_widget->setMaxVal(360);
  m_widget->setMinVal(0);
  m_gridLayoout->addWidget(m_widget);
  SetTopLayout(m_gridLayoout);

  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

int PhaserDialogBox::GetPhase() { return m_widget->getDisplayIntVal(); }

QList<std::pair<QString, QVariant>> PhaserDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_widget", m_widget->getDisplayIntVal()));
  return listSetting;
}

bool PhaserDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_widget")
      m_widget->setValue(parmSetting.second.toInt());
  }
  return true;
}
