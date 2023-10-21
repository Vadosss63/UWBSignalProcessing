#include "3dmodule.h"

D3Module::D3Module(UpdateData *update) : SinkModule(1), m_update(update) {}

void D3Module::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  inBuffer signal;

  if (!PopFromInput(signal))
    return;

  for (size_t speed = 0; speed < 256; ++speed)
    m_update->AddData(std::move(signal[speed]), speed);
  m_update->Update();
}
