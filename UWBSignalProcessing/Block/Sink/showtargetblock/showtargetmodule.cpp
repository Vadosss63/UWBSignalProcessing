#include "showtargetmodule.h"

ShowTargetModule::ShowTargetModule() : SinkModule(1) {}

void ShowTargetModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  std::vector<Target> targetList;

  if (!PopFromInput(targetList))
    return;

  m_updateScane->AddTarget(std::move(targetList));
}

void ShowTargetModule::SetUpdateScane(UpdateScane *updateScane) {
  m_updateScane = updateScane;
}
