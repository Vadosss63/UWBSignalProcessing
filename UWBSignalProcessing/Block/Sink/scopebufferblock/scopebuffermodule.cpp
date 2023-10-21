#include "scopebuffermodule.h"

ScopeBufferModule::ScopeBufferModule() : SinkModule(1) {}

bool ScopeBufferModule::PopSignal(buffer &csignal) {
  return m_dataCsignal.try_pop(csignal);
}

void ScopeBufferModule::Operate() {
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
  emit BufferUpdated();
}

void ScopeBufferModule::StopEvent() {
  m_dataCsignal.clear();
  SinkModule::StopEvent();
}
