#include "transferfrequencydialogbox.h"

TransferFrequencyDialogBox::TransferFrequencyDialogBox() {
  auto layout = new QGridLayout;
  CreateFreqBox(layout);

  SetTopLayout(layout);

  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

double TransferFrequencyDialogBox::GetTransferFrequency() const {
  return m_box->value();
}

QList<std::pair<QString, QVariant>> TransferFrequencyDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_box", m_box->value()));
  listSetting.append(std::make_pair("m_sampFreq", m_sampFreq->currentIndex()));
  return listSetting;
}

bool TransferFrequencyDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_box")
      m_box->setValue(parmSetting.second.toDouble());

    if (parmSetting.first == "m_sampFreq")
      m_sampFreq->setCurrentIndex(parmSetting.second.toInt());
  }
  return true;
}

void TransferFrequencyDialogBox::CreateFreqBox(QGridLayout *layout) {
  QLabel *labelTS = new QLabel("Частота гетеродина, МГц :");
  m_box = new QDoubleSpinBox;
  m_sampFreq = new QComboBox;
  m_sampFreq->addItem("500");
  m_sampFreq->addItem("1000");
  m_sampFreq->addItem("2000");
  m_sampFreq->setCurrentIndex(1);
  QLabel *labelFreq = new QLabel("Частота дискретизации, МГц :");

  m_box->setMaximum(1000);
  m_box->setMinimum(-1000);
  m_box->setValue(500);

  layout->addWidget(labelFreq, 0, 0);
  layout->addWidget(m_sampFreq, 0, 1);
  layout->addWidget(labelTS, 1, 0);
  layout->addWidget(m_box, 1, 1);
}

double TransferFrequencyDialogBox::GetSampFreq() const {
  switch (m_sampFreq->currentIndex()) {
  case 0:
    return 2048;
  case 1:
    return 4096;
  case 2:
    return 8192;
  }
  return 0;
}
