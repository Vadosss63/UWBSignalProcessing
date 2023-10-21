#include "gaindialogbox.h"
#include <qmath.h>

GainDialogBox::GainDialogBox() {
  m_gridLayoout = new QGridLayout;
  m_label = new QLabel("Коэфициент усиления сигнала");
  m_QLCDNumber = new QLCDNumber;
  m_QDial = new QDial;
  m_QDial->setNotchesVisible(true);
  m_incButton = new QPushButton("+");
  m_incButton->setFixedWidth(40);
  m_incButton->setFixedHeight(40);
  m_decButton = new QPushButton("-");
  m_decButton->setFixedWidth(40);
  m_decButton->setFixedHeight(40);
  m_signed = new QCheckBox("Отрицательное усиление");

  m_labelSpinBox = new QLabel("Множитель 10^");

  m_spinBox = new QSpinBox;
  m_spinBox->setRange(-9, 8);

  m_gridLayoout->addWidget(m_label, 0, 0, 1, 2);
  m_gridLayoout->addWidget(m_QLCDNumber, 1, 0, 2, 1);
  m_gridLayoout->addWidget(m_incButton, 1, 1);
  m_gridLayoout->addWidget(m_decButton, 2, 1);
  m_gridLayoout->addWidget(m_QDial, 3, 0, 1, 2);
  m_gridLayoout->addWidget(m_labelSpinBox, 4, 0, 1, 1);
  m_gridLayoout->addWidget(m_spinBox, 4, 1, 1, 1);
  m_gridLayoout->addWidget(m_signed, 5, 0, 1, 2);
  SetTopLayout(m_gridLayoout);

  connect(m_QDial, SIGNAL(valueChanged(int)), this, SLOT(ChangedQDial(int)));
  connect(this, SIGNAL(ValueChanged(double)), m_QLCDNumber,
          SLOT(display(double)));
  connect(m_incButton, SIGNAL(clicked()), this, SLOT(SetIncDouble()));
  connect(m_decButton, SIGNAL(clicked()), this, SLOT(SetDecDouble()));
  connect(m_signed, SIGNAL(clicked()), this, SLOT(SetSigned()));

  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
  m_QDial->setValue(1);
}

void GainDialogBox::ChangedQDial(int val) {
  m_numderInt = static_cast<uint8_t>(val);
  emit ValueChanged(m_signedGain * (static_cast<double>(m_numderInt) +
                                    static_cast<double>(m_numderDouble) / 10));
}

void GainDialogBox::SetIncDouble() {

  if (m_numderDouble >= 9) {
    if (m_numderInt != 99) {
      m_numderDouble = 0;
      m_numderInt++;
      m_QDial->setValue(m_numderInt);
    }
  } else {
    m_numderDouble++;
    emit ValueChanged(m_signedGain *
                      (static_cast<double>(m_numderInt) +
                       static_cast<double>(m_numderDouble) / 10));
  }
}

void GainDialogBox::SetDecDouble() {

  if (m_numderDouble <= 0) {
    if (m_numderInt != 0) {
      m_numderDouble = 9;
      m_numderInt--;
      m_QDial->setValue(m_numderInt);
    }
  } else {
    m_numderDouble--;
    emit ValueChanged(m_signedGain *
                      (static_cast<double>(m_numderInt) +
                       static_cast<double>(m_numderDouble) / 10));
  }
}

void GainDialogBox::SetSigned() {
  m_signed->isChecked() ? m_signedGain = -1 : m_signedGain = 1;
  ChangedQDial(m_numderInt);
}

double GainDialogBox::GetGain() {
  return m_QLCDNumber->value() * std::pow(10, m_spinBox->value());
}

QList<std::pair<QString, QVariant>> GainDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_QLCDNumber", m_QLCDNumber->value()));
  listSetting.append(std::make_pair("m_spinBox", m_spinBox->value()));
  return listSetting;
}

bool GainDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  foreach (auto parmSetting, listSetting) {
    /// TODO проверить
    if (parmSetting.first == "m_QLCDNumber") {
      int intVAl = static_cast<int>(qAbs(parmSetting.second.toDouble()));
      m_QDial->setValue(intVAl);
      m_numderDouble = static_cast<uint8_t>(
                           qAbs(parmSetting.second.toDouble()) - m_numderInt) *
                       10;
      m_signed->setChecked((parmSetting.second.toDouble() < 0));
      SetSigned();
    }

    if (parmSetting.first == "m_spinBox")
      m_spinBox->setValue(parmSetting.second.toInt());
  }
  return true;
}
