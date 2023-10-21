#ifndef TRANSFERFREQUENCYMODULE_H
#define TRANSFERFREQUENCYMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <complex>

class TransferFrequencyModule
    : public IOModule<csignal<double>, csignal<double>> {

public:
  TransferFrequencyModule();
  ~TransferFrequencyModule() override = default;

  void Operate() override;
  //Установка частоты переноса
  void SetTransferFrequency(double transfreq, size_t samp);

private:
  // иницилизация генератора
  void Init();
  // длиина сигнала
  size_t m_lengthSig = 8192;
  double m_frequency = (2 * M_PI * (500 * 1E+6));
  double m_sampleTime = 5E-10;
  // комплекстный гетродин
  csignal<std::complex<double>> m_gen;
};

#endif // TRANSFERFREQUENCYMODULE_H
