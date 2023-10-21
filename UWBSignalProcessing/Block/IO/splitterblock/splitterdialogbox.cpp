#include "splitterdialogbox.h"

SplitterDialogBox::SplitterDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_label = new QLabel("Количество портов:");
  m_spinBox = new QSpinBox;
  m_gridLayoout->addWidget(m_label, 0, 0);
  m_gridLayoout->addWidget(m_spinBox, 0, 1);
  SetTopLayout(m_gridLayoout);

  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
  m_spinBox->setRange(2, 10);
}

uint8_t SplitterDialogBox::GetOutputPortsNumber() {
  return static_cast<uint8_t>(m_spinBox->value());
}

QList<std::pair<QString, QVariant>> SplitterDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_spinBox", m_spinBox->value()));

  return listSetting;
}

bool SplitterDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_spinBox")
      m_spinBox->setValue(parmSetting.second.toInt());
  }
  return true;
}
