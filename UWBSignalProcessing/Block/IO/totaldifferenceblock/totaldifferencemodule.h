#ifndef TOTALDIFFERENCEMODULE_H
#define TOTALDIFFERENCEMODULE_H

#include "csignal.h"
#include "iomodule.h"

class TotalTifferenceModule
    : public IOModule<csignal<double>, csignal<double>> {

public:
  TotalTifferenceModule();
  ~TotalTifferenceModule() override = default;

  void Operate() override;
  void StartEvent() override;

private:
  csignal<double> m_signalFirst;
  csignal<double> m_signalSecond;
  // номер сигнала
  uint32_t m_currentSignal = 0;
};

#endif // TOTALDIFFERENCEMODULE_H
