#ifndef SQRTMODULE_H
#define SQRTMODULE_H

#include "csignal.h"
#include "iomodule.h"

using InBuffer = std::pair<csignal<double>, csignal<double>>;
class SqrtModule : public IOModule<InBuffer, csignal<double>> {

public:
  SqrtModule();
  ~SqrtModule() override = default;

  void Operate() override;
  void SetIsSqrt(bool isSqrt);

private:
  bool m_isSqrt = true;
};

#endif // SQRTMODULE_H
