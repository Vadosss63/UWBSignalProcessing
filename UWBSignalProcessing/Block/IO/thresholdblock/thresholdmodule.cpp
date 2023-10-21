#include "thresholdmodule.h"

ThresholdModule::ThresholdModule() : IOModule(2, 1) {}

void ThresholdModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  csignal<double> sigThreshold;
  if (!PopFromInput(sigThreshold, 0))
    return;

  csignal<double> sig;
  if (!PopFromInput(sig, 1))
    return;

  if (sigThreshold.size() == sigThreshold.size()) {
    std::transform(sig.begin(), sig.end(), sigThreshold.begin(), sig.begin(),
                   [](double &sigVal, double &thresholdVal) {
                     return sigVal > thresholdVal ? thresholdVal : sigVal;
                   });
  }
  PushToOutput(std::move(sig));
}
