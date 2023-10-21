#include "diffmodule.h"

DiffModule::DiffModule() : IOModule<csignal<double>, csignal<double>>(1, 1) {}

void DiffModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> inSig;
  if (!PopFromInput(inSig))
    return;
  csignal<double> outSig(inSig.size());
  outSig.setServiceParameters(inSig.azimuth(), inSig.frameNumber());

  std::transform(inSig.begin(),
                 inSig.begin() + (static_cast<int>(inSig.size()) - m_deltaX),
                 inSig.begin() + m_deltaX, outSig.begin(),
                 [](double x1, double x2) {
                   double delta = x2 - x1;
                   return delta;
                 });

  PushToOutput(std::move(outSig));
}

void DiffModule::SetDelta_x(int deltaX) { m_deltaX = deltaX; }
