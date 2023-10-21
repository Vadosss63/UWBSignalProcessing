#ifndef FFTMODULE_H
#define FFTMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <fftw3.h>

class FFTmodule : public IOModule<csignal<double>, csignal<double>> {
public:
  FFTmodule();
  ~FFTmodule() override;

  void Operate() override;

private:
  void SetPlan(size_t len);
  fftw_plan plan_fft_forward;
  csignal<std::complex<double>> TempFFT;
  csignal<double> m_sigTemp;
};

#endif // FFTMODULE_H
