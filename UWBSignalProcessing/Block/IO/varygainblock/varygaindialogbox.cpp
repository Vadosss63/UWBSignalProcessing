#include "varygaindialogbox.h"
#include <qmath.h>

VaryGainDialogBox::VaryGainDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_label = new QLabel("Максимальная дальность");
  m_QLCDNumber = new QLCDNumber;
  m_QDial = new QDial;
  m_QDial->setMaximum(1024);
  m_QDial->setNotchesVisible(true);

  m_gridLayoout->addWidget(m_label, 0, 0);
  m_gridLayoout->addWidget(m_QLCDNumber, 1, 0);
  m_gridLayoout->addWidget(m_QDial, 2, 0);

  SetTopLayout(m_gridLayoout);

  connect(m_QDial, SIGNAL(valueChanged(int)), m_QLCDNumber, SLOT(display(int)));
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
  m_QDial->setValue(333);
  //    m_QLCDNumber->display(333);
}

size_t VaryGainDialogBox::GetIndex() {
  return static_cast<size_t>(m_QLCDNumber->value());
}

QList<std::pair<QString, QVariant>> VaryGainDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_QLCDNumber", m_QLCDNumber->value()));
  return listSetting;
}

bool VaryGainDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    /// TODO проверить
    if (parmSetting.first == "m_QLCDNumber") {
      int intVAl = static_cast<int>(qAbs(parmSetting.second.toDouble()));
      m_QDial->setValue(intVAl);
    }
  }
  return true;
}
