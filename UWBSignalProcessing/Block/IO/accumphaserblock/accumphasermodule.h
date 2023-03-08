#ifndef ACCUMPHASERMODULE_H
#define ACCUMPHASERMODULE_H

#include <complex>
#include "iomodule.h"
#include "csignal.h"

class  AccumPhaserModule: public IOModule< csignal<double>,  csignal<double>>
{
public:
    AccumPhaserModule();
    ~AccumPhaserModule() override = default;

    void Operate() override;
    void StartEvent() override;

    void SetCountAccum(uint16_t countAccum);
    // установка фазы в градусах
    void SetPhase(double phase);

private:

    void ResizeAccumulations(size_t newSize);
    void ResetCurrentAccumulation();
    void SetFrameToSignals();
    void ScalingSignals();
    bool CheckAzimut(uint32_t azimuthI, uint32_t azimuthQ);

    csignal<double> m_sigI;
    csignal<double> m_sigQ;
    int m_currentAccumalation = 0;
    std::complex<double> m_expJ;

    // фаза в радианах
    double m_phase = 0;
    uint16_t m_countAccum = 1;
    uint16_t m_currentFrame = 0;
};

#endif // ACCUMPHASERMODULE_H
