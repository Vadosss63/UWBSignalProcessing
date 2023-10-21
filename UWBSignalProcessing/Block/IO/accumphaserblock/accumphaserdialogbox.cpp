#include "accumphaserdialogbox.h"

AccumPhaserDialogBox::AccumPhaserDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_wPhase = new CustomDialWidget("Поворот фазы, град.");
  m_wPhase->setMaxVal(180);
  m_wPhase->setMinVal(-180);
  m_gridLayoout->addWidget(m_wPhase);

  m_wCountAccum = new CustomDialWidget("Накопление");
  m_wCountAccum->setMaxVal(1024);
  m_wCountAccum->setMinVal(1);
  m_gridLayoout->addWidget(m_wCountAccum);

  SetTopLayout(m_gridLayoout);

  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

double AccumPhaserDialogBox::GetPhase() { return m_wPhase->getDisplayIntVal(); }

uint16_t AccumPhaserDialogBox::GetCountAccum() {
  return m_wCountAccum->getDisplayIntVal();
}

QList<std::pair<QString, QVariant>> AccumPhaserDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_wPhase", m_wPhase->getDisplayIntVal()));
  listSetting.append(
      std::make_pair("m_wCountAccum", m_wCountAccum->getDisplayIntVal()));
  return listSetting;
}

bool AccumPhaserDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_wPhase")
      m_wPhase->setValue(parmSetting.second.toInt());

    if (parmSetting.first == "m_wCountAccum")
      m_wCountAccum->setValue(parmSetting.second.toInt());
  }
  return true;
}
