#include "3dtargetmodule.h"

D3TargetModule::D3TargetModule(UpdateDataTarget *update)
    : SinkModule(1), m_update(update) {}

void D3TargetModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  inBuffer signal;

  if (!PopFromInput(signal))
    return;

  m_update->AddData(signal);
  m_update->Update();
}
