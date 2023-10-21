#include "iqtocomplexmodule.h"

IQToComplexModule::IQToComplexModule()
    : IOModule(static_cast<uint8_t>(2), static_cast<uint8_t>(1)) {}

void IQToComplexModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  InBuffer signalI;
  if (!PopFromInput(signalI, 0))
    return;

  InBuffer signalQ;
  if (!PopFromInput(signalQ, 1))
    return;

  PushToOutput(std::make_pair(std::move(signalI), std::move(signalQ)));
}
