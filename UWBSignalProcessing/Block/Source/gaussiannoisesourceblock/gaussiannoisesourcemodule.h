#ifndef GAUSSIANNOISESOURCE_H
#define GAUSSIANNOISESOURCE_H

#include "csignal.h"
#include "sourcemodule.h"
#include <chrono>
#include <random>

class GaussianNoiseSourceModule : public SourceModule<csignal<double>> {

public:
  GaussianNoiseSourceModule();
  ~GaussianNoiseSourceModule() override = default;

  void Operate() override;
  void TimerTimout() override;

  void SetParameters(size_t length, double deviation);

private:
  size_t m_signalLength;
  std::default_random_engine m_engine;
  std::normal_distribution<double> m_distribution;
};

#endif // GAUSSIANNOISESOURCE_H
