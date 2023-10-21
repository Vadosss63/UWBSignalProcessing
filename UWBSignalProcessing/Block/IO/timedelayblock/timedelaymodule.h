#ifndef TIMEDELAYMODULE_H
#define TIMEDELAYMODULE_H

#include "csignal.h"
#include "iomodule.h"

class TimeDelayModule : public IOModule<csignal<double>, csignal<double>> {

public:
  TimeDelayModule();
  ~TimeDelayModule() override = default;

  void Operate() override;
  void SetNumberOfZeros(int numberOfZeros);

private:
  // задержка сигнала
  int m_numberOfZeros = 0;
};

#endif // TIMEDELAYMODULE_H
