#ifndef STATISTICALESTIMATEMODULE_H
#define STATISTICALESTIMATEMODULE_H

#include "csignal.h"
#include "showstatisticalestimate.h"
#include "sinkmodule.h"
#include <string>

class StatisticaleStimateModule : public SinkModule<csignal<double>> {

public:
  StatisticaleStimateModule();
  ~StatisticaleStimateModule() override = default;

  void Operate() override;
  void SetIndex(size_t startIndex, size_t stopIndex);
  void
  SetShowStatisticaleStimate(ShowStatisticaleStimate *showStatisticaleStimate);

private:
  // Матожидание
  double Mean(const csignal<double> &signal);
  // Среднее значение
  double MeanValue(const csignal<double> &signal);
  // Дисперсия
  double Variance(const csignal<double> &signal, double mean = 0);
  // СКО
  double OneSigmaValue(double variance);

  // начало измерений
  size_t m_startIndex = 0;
  // конец измерений
  size_t m_stopIndex = 2047;

  ShowStatisticaleStimate *m_showStatisticaleStimate = nullptr;
};

#endif // STATISTICALESTIMATEMODULE_H
