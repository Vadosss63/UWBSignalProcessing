#ifndef PHASERMODULE_H
#define PHASERMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <complex>

class PhaserModule : public IOModule<csignal<double>, csignal<double>> {
public:
  PhaserModule();
  ~PhaserModule() override = default;

  void Operate() override;

  void SetPhase(double phase); // установка фазы в градусах

private:
  double m_phase = 0; // фаза в радианах
};

#endif // PHASERMODULE_H
