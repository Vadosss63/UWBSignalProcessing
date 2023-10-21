#include "scopecomplexmodule.h"

ScopeComplexModule::ScopeComplexModule() : SinkModule(1) {}

bool ScopeComplexModule::PopSignal(buffer &csignal) {
  return m_dataCsignal.try_pop(csignal);
}

void ScopeComplexModule::Operate() {
  if (!m_dataCsignal.empty()) {
    buffer buffer;
    PopFromInput(buffer);
    return;
  }

  std::lock_guard<std::mutex> lock(operateLock);

  buffer buffer;
  if (!PopFromInput(buffer))
    return;

  m_dataCsignal.push(std::move(buffer));
  BufferUpdated();
}

void ScopeComplexModule::StopEvent() {
  m_dataCsignal.clear();
  SinkModule::StopEvent();
}
