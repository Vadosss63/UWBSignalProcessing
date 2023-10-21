#include "varygainmodule.h"

VaryGainModule::VaryGainModule() : IOModule(1, 1) {}

void VaryGainModule::SetIndexConst(size_t index) { m_index_const = index; }

void VaryGainModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> temp;

  if (!PopFromInput(temp))
    return;

  for (size_t i = 0; i < m_index_R2; ++i)
    temp[i] = 0;

  for (size_t i = m_index_R2; i < m_index_const; ++i)
    temp[i] *= (std::pow(static_cast<double>(i), 2) /
                std::pow(static_cast<double>(m_index_const), 2));

  PushToOutput(std::move(temp));
}
