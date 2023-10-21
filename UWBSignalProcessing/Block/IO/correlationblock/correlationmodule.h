#ifndef CORRELATIONMODULE_H
#define CORRELATIONMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <fftw3.h>

//Модуль корреляции
//Принимает два входных сигнала и коррелирует их между собой

class Correlationmodule : public IOModule<csignal<double>, csignal<double>> {
public:
  Correlationmodule();
  Correlationmodule(const Correlationmodule &rhs) = delete;
  ~Correlationmodule() override;
  Correlationmodule &operator=(const Correlationmodule &rhs) = delete;

  void Operate() override;

private:
  void GeneratePlan(size_t size);

  csignal<std::complex<double>> m_res1FFT;
  csignal<std::complex<double>> m_res2FFT;

  csignal<double> m_Seq1;
  csignal<double> m_Seq2;

  fftw_plan m_plan_fft_forwardSeq1;
  fftw_plan m_plan_fft_forwardSeq2;

  fftw_plan m_plan_fft_backwardSeq1;
};

#endif // CORRELATIONMODULE_H
