#include "syntheticaperturemodule.h"

SyntheticApertureModule::SyntheticApertureModule() : IOModule(1, 1) {}

void SyntheticApertureModule::SetIndexStop(std::vector<size_t> indexStop) {
  m_indexStop = std::move(indexStop);
}

void SyntheticApertureModule::PushBuffer(csignal<double> &signal) {
  if (m_bufferAperture.empty()) {
    m_bufferAperture.emplace_back(std::move(signal));
    return;
  }

  if (m_bufferAperture.size() < m_sizeBuffer) {
    if (m_bufferAperture.front().azimuth() == signal.azimuth()) {
      m_bufferAperture.emplace_back(std::move(signal));
    } else {
      ClearBuffer();
      m_bufferAperture.emplace_back(std::move(signal));
      return;
    }
  }

  if (m_sizeBuffer == m_bufferAperture.size()) {
    SyntheticAperture();
    ClearBuffer();
  }
}

void SyntheticApertureModule::SyntheticAperture() {
  size_t size = m_bufferAperture.front().size();
  csignal<double> signal(size);
  /// TODO сделать правильно фрайм
  signal.setServiceParameters(m_bufferAperture.front().azimuth(),
                              m_bufferAperture.front().frameNumber());

  size_t count = m_indexStop.size();
  size_t start = 0;
  size_t stop = 0;
  for (size_t i = 0; i < count; ++i) {
    stop = m_indexStop[i];
    AccumSignal(signal, start, stop, static_cast<size_t>(std::pow(2, i + 1)));
    start = m_indexStop[i];
  }

  PushToOutput(std::move(signal));
}

void SyntheticApertureModule::AccumSignal(csignal<double> &signal, size_t start,
                                          size_t stop, size_t countAntennes) {
  size_t startAntenna = m_sizeBuffer / countAntennes - 1;
  size_t stopAntenna = startAntenna + countAntennes - 1;

  for (size_t i = start; i <= stop; ++i) {
    for (size_t j = startAntenna; j <= stopAntenna; ++j)
      signal[i] += m_bufferAperture[j][i];

    signal[i] /= countAntennes;
  }
}

void SyntheticApertureModule::ClearBuffer() { m_bufferAperture.clear(); }

void SyntheticApertureModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  csignal<double> temp;
  if (!PopFromInput(temp))
    return;

  PushBuffer(temp);
}
