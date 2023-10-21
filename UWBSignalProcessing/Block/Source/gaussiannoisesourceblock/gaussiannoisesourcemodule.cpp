#include "gaussiannoisesourcemodule.h"

GaussianNoiseSourceModule::GaussianNoiseSourceModule()
    : SourceModule<csignal<double>>(1), m_signalLength(2048),
      m_engine(std::chrono::system_clock::now().time_since_epoch().count()),
      m_distribution(std::normal_distribution<double>(0.0, 1)) {}

void GaussianNoiseSourceModule::SetParameters(size_t length, double deviation) {
  m_signalLength = length;
  m_distribution = std::normal_distribution<double>(0.0, deviation);
}

void GaussianNoiseSourceModule::Operate() {
  csignal<double> temp(m_signalLength);
  for (double &it : temp)
    it = m_distribution(m_engine);

  PushToOutput(std::move(temp));
}

void GaussianNoiseSourceModule::TimerTimout() { QueueOperation(); }
