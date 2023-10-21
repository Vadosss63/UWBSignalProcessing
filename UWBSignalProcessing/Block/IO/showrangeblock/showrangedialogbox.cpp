#include "showrangedialogbox.h"

ShowRangeDialogBox::ShowRangeDialogBox() { CreateWidget(); }

int ShowRangeDialogBox::GetRange() { return m_range->value(); }

QList<std::pair<QString, QVariant>> ShowRangeDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_range", m_range->value()));
  return listSetting;
}

bool ShowRangeDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  if (listSetting.first().first == "m_range")
    m_range->setValue(listSetting.first().second.toUInt());

  return true;
}

void ShowRangeDialogBox::CreateWidget() {
  auto mainLayout = new QHBoxLayout;
  m_range = new QSpinBox;
  m_range->setRange(0, 8912);
  mainLayout->addWidget(new QLabel("Дальность - "));
  mainLayout->addWidget(m_range);
  SetTopLayout(mainLayout);
}
