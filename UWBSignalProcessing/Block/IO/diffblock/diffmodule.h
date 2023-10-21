#ifndef DIFFMODULE_H
#define DIFFMODULE_H

#include "csignal.h"
#include "iomodule.h"

class DiffModule : public IOModule<csignal<double>, csignal<double>> {

public:
  DiffModule();
  ~DiffModule() override = default;

  void Operate() override;

  void SetDelta_x(int deltaX);

private:
  int m_deltaX = 1; // задержка сигнала
};

#endif // DIFFMODULE_H
