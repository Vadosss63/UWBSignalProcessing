#ifndef PHASERMODULE_H
#define PHASERMODULE_H

#include <complex>
#include "iomodule.h"
#include "csignal.h"

class PhaserModule : public IOModule<csignal<double>,  csignal<double>>
{
public:

    PhaserModule();
    ~PhaserModule() override = default;

    void Operate() override;

    void SetPhase(double phase);// установка фазы в градусах

private:

    double m_phase = 0;    // фаза в радианах
};

#endif // PHASERMODULE_H
