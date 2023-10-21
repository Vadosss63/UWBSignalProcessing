#include "varymodule.h"
#include <algorithm>

VARYModule::VARYModule() : IOModule(1, 1) {}

void VARYModule::Operate() {
  csignal<double> temp;
  if (!PopFromInput(temp))
    return;

  if (temp.size() <= m_VARYCoeff.size()) {
    std::transform(temp.begin(), temp.end(), m_VARYCoeff.begin(), temp.begin(),
                   [](double val, double coef) { return val * coef; });
  }

  PushToOutput(std::move(temp));
}

void VARYModule::SetVARYCoeff(const std::vector<double> &coefficients) {
  m_VARYCoeff = coefficients;
}
