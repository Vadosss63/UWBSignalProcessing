#include "totaldifferencemodule.h"

TotalTifferenceModule::TotalTifferenceModule() : IOModule(1, 2) {}

void TotalTifferenceModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  if (m_currentSignal % 2) {
    if (!PopFromInput(m_signalSecond))
      return;

    if (m_signalSecond.azimuth() != m_signalFirst.azimuth()) {

      if ((m_signalSecond.azimuth() - m_signalFirst.azimuth()) == 1) {
        m_signalFirst.swap(m_signalSecond);
        m_currentSignal++;
      } else {
        // если азимуты не совподают чистим первый и второй сигнал
        m_signalSecond.clear();
        m_signalFirst.clear();
      }
    }
  } else {
    if (!PopFromInput(m_signalFirst))
      return;
  }

  // можно ввести проверку по фрайму
  m_currentSignal++;

  if ((m_signalSecond.size()) &&
      (m_signalFirst.size() == m_signalSecond.size())) {
    PushToOutput(m_signalFirst + m_signalSecond, 0);
    PushToOutput(m_signalFirst - m_signalSecond, 1);
  }
}

void TotalTifferenceModule::StartEvent() {
  m_signalFirst.clear();
  m_signalSecond.clear();
  // номер сигнала
  m_currentSignal = 0;
}
