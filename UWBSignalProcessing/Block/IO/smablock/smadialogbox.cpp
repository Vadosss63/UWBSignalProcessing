#include "smadialogbox.h"

SMADialogBox::SMADialogBox() {
  m_gridLayout = new QGridLayout;
  m_widget = new CustomDialWidget("Размер скользящиго окна");
  m_widget->setMaxVal(1024);
  m_widget->setMinVal(3);
  m_gridLayout->addWidget(m_widget);
  SetTopLayout(m_gridLayout);
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

int SMADialogBox::GetSizeWindow() { return m_widget->getDisplayIntVal(); }

QList<std::pair<QString, QVariant>> SMADialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_widget", m_widget->getDisplayIntVal()));
  return listSetting;
}

bool SMADialogBox::SetSetting(QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_widget")
      m_widget->setValue(parmSetting.second.toInt());
  }
  return true;
}
