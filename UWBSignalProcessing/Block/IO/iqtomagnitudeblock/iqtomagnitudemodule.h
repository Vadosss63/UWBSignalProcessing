#ifndef IQTOMAGNITUDE_ANGLEMODULE_H
#define IQTOMAGNITUDE_ANGLEMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <fftw3.h>

class IQtoMagnitude_AngleModule
    : public IOModule<csignal<double>, csignal<double>> {
public:
  IQtoMagnitude_AngleModule();
  ~IQtoMagnitude_AngleModule() override;

  void Operate() override;

private:
  void SetPlan(size_t len);

  csignal<std::complex<double>> TempFFT;
  fftw_plan plan_fft;
};

#endif // IQTOMAGNITUDE_ANGLEMODULE_H
