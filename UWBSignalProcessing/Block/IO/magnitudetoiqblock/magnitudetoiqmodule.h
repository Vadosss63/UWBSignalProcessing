#ifndef MAGNITUDE_ANGLEMODULETOIQMODULE_H
#define MAGNITUDE_ANGLEMODULETOIQMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <fftw3.h>

class Magnitude_AngleModuleToIQModule
    : public IOModule<csignal<double>, csignal<double>> {

public:
  Magnitude_AngleModuleToIQModule();
  ~Magnitude_AngleModuleToIQModule() override;

  void Operate() override;

private:
  void SetPlan(size_t len);
  csignal<std::complex<double>> TempFFT;
  fftw_plan plan_fft;
};
#endif // MAGNITUDE_ANGLEMODULETOIQMODULE_H
