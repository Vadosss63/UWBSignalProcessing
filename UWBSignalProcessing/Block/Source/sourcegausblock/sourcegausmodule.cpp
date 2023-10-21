#include "sourcegausmodule.h"
#include <cmath>

SourceGausModule::SourceGausModule()
    : SourceModule<csignal<double>>(static_cast<uint8_t>(1)),
      m_generatorArray(2048), m_lenSignal(2048), m_amplitude(200), m_sigma(3),
      m_expectation(500), m_az(0) {
  StartGenerator();
  m_frame = 0;
}

void SourceGausModule::StartGenerator() {
  for (size_t i = 0; i < m_lenSignal; ++i) {
    m_generatorArray[i] = m_amplitude * exp(-(pow((i - m_expectation), 2)) /
                                            (pow(m_sigma, 2) * 2));
  }
}

void SourceGausModule::ChangeParametersGenerator(double amplitude,
                                                 size_t lenSignal, double sigma,
                                                 double expectation) {
  m_amplitude = amplitude;
  m_lenSignal = lenSignal;
  m_sigma = sigma;
  m_expectation = expectation;
  m_generatorArray = csignal<double>(m_lenSignal);
  StartGenerator();
}

void SourceGausModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  m_generatorArray.setFrameNumber(m_frame++);
  m_generatorArray.setAzimuth(m_az++);
  PushToOutput(m_generatorArray);
}

void SourceGausModule::StopEvent() {
  m_frame = 0;
  SourceModule::StopEvent();
}

void SourceGausModule::TimerTimout() { QueueOperation(); }
