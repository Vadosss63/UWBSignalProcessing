#include "complextoiqmodule.h"

ComplexToIQModule::ComplexToIQModule()
    : IOModule(static_cast<uint8_t>(1), static_cast<uint8_t>(2)) {}

void ComplexToIQModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  InBuffer tempSigmal;
  if (!PopFromInput(tempSigmal))
    return;

  PushToOutput(std::move(tempSigmal.first), 0);
  PushToOutput(std::move(tempSigmal.second), 1);
}
