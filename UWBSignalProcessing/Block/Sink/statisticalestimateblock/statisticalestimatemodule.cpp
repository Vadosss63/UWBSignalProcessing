#include "statisticalestimatemodule.h"
#include <algorithm>

StatisticaleStimateModule::StatisticaleStimateModule() : SinkModule(1) {}

void StatisticaleStimateModule::SetIndex(size_t startIndex, size_t stopIndex) {
  m_startIndex = startIndex;
  m_stopIndex = stopIndex;
}

void StatisticaleStimateModule::Operate() {

  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> signal;
  PopFromInput(signal);

  if (m_stopIndex >= signal.size())
    return;

  double mean = Mean(signal);
  double meanValue = MeanValue(signal);
  double variance = Variance(signal, mean);
  double osv = OneSigmaValue(variance);

  if (m_showStatisticaleStimate)
    m_showStatisticaleStimate->ShowStimate(mean, meanValue, variance, osv,
                                           osv / meanValue);
}

double StatisticaleStimateModule::Mean(const csignal<double> &signal) {
  double mean = 0;
  std::for_each(signal.begin() + m_startIndex, signal.begin() + m_stopIndex + 1,
                [&mean](double val) { mean += val; });

  return mean / (m_stopIndex - m_startIndex + 1);
}

double StatisticaleStimateModule::MeanValue(const csignal<double> &signal) {
  double mean = 0;
  std::for_each(signal.begin() + m_startIndex, signal.begin() + m_stopIndex + 1,
                [&mean](double val) { mean += std::abs(val); });

  double size = (m_stopIndex - m_startIndex + 1);

  return mean / std::sqrt(size * (size - 1));
}

double StatisticaleStimateModule::Variance(const csignal<double> &signal,
                                           double mean) {
  double variance = 0;
  std::for_each(
      signal.begin() + m_startIndex, signal.begin() + m_stopIndex + 1,
      [&variance, mean](double val) { variance += std::pow(val - mean, 2); });

  double size = (m_stopIndex - m_startIndex + 1);

  return variance / (size - 1);
}

double StatisticaleStimateModule::OneSigmaValue(double variance) {
  return std::sqrt(variance);
}

void StatisticaleStimateModule::SetShowStatisticaleStimate(
    ShowStatisticaleStimate *showStatisticaleStimate) {
  m_showStatisticaleStimate = showStatisticaleStimate;
}
