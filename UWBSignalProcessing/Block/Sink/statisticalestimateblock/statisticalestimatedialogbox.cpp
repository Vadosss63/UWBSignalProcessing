#include "statisticalestimatedialogbox.h"

StatisticaleStimateDialogBox::StatisticaleStimateDialogBox() { CreateWidget(); }

size_t StatisticaleStimateDialogBox::GetStartIndex() const {
  return static_cast<size_t>(m_startIndex->value());
}

size_t StatisticaleStimateDialogBox::GetStopIndex() const {
  return static_cast<size_t>(m_stopIndex->value());
}

QList<std::pair<QString, QVariant>> StatisticaleStimateDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("startIndex", m_startIndex->value()));
  listSetting.append(std::make_pair("stopIndex", m_stopIndex->value()));
  return listSetting;
}

bool StatisticaleStimateDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "startIndex")
      m_startIndex->setValue(parmSetting.second.toInt());

    if (parmSetting.first == "stopIndex")
      m_stopIndex->setValue(parmSetting.second.toInt());
  }
  return true;
}

void StatisticaleStimateDialogBox::ShowStimate(double mean, double meanValue,
                                               double variance, double osv,
                                               double osv_to_meanValue) {
  m_textMean->setText(QString("%1").arg(mean));
  m_textMeanValue->setText(QString("%1").arg(meanValue));
  m_textVariance->setText(QString("%1").arg(variance));
  m_textOSV->setText(QString("%1").arg(osv));
  m_textOSV_to_meanValue->setText(QString("%1").arg(osv_to_meanValue));
}

void StatisticaleStimateDialogBox::CreateWidget() {
  QGridLayout *layout = new QGridLayout;

  QLabel *textIndex = new QLabel("Индексы оценки(старт, стоп): ");
  layout->addWidget(textIndex, 0, 0, 1, 1);

  m_startIndex = new QSpinBox;
  m_stopIndex = new QSpinBox;
  m_startIndex->setRange(0, 8192);
  m_stopIndex->setRange(0, 8192);
  m_stopIndex->setValue(2047);

  layout->addWidget(m_startIndex, 0, 1, 1, 1);
  layout->addWidget(m_stopIndex, 0, 2, 1, 1);

  QLabel *textMean = new QLabel("Матожидание  ");
  layout->addWidget(textMean, 1, 0, 1, 1);

  m_textMean = new QLabel("");
  layout->addWidget(m_textMean, 1, 1, 1, 2);

  QLabel *textMeanValue = new QLabel("Среднее значение  ");
  layout->addWidget(textMeanValue, 2, 0, 1, 1);

  m_textMeanValue = new QLabel("");
  layout->addWidget(m_textMeanValue, 2, 1, 1, 2);

  QLabel *textVariance = new QLabel("Дисперсия  ");
  layout->addWidget(textVariance, 3, 0, 1, 1);

  m_textVariance = new QLabel("");
  layout->addWidget(m_textVariance, 3, 1, 1, 2);

  QLabel *textOSV = new QLabel("СКО  ");
  layout->addWidget(textOSV, 4, 0, 1, 1);

  m_textOSV = new QLabel("");
  layout->addWidget(m_textOSV, 4, 1, 1, 2);

  QLabel *textOSV_to_meanValue = new QLabel("СКО/Среднее значение  ");
  layout->addWidget(textOSV_to_meanValue, 5, 0, 1, 1);

  m_textOSV_to_meanValue = new QLabel("");
  layout->addWidget(m_textOSV_to_meanValue, 5, 1, 1, 2);

  SetTopLayout(layout);
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}
