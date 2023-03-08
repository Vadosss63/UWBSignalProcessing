#ifndef COSTRANSFERMODULE_H
#define COSTRANSFERMODULE_H

#include "iomodule.h"
#include "csignal.h"

class CosTransferModule : public IOModule<csignal<double>, csignal<double>>
{

public:
    CosTransferModule();
    ~CosTransferModule() override = default;

    void Operate() override;

    void ChangeParametersGenerator(double frequency, bool phaseSin, bool simpleFrequency500MHz);

private:

    void StartGenerator();

    size_t m_lenSignal = 2048;         // Длина сигнала
    double m_frequency = (2 * 1E+6 * M_PI);         // Частота генератора (Гц)
    bool m_phaseSin = false;            // 0 или pi/2
    double m_sampleTime = 2E-9;        // Период дискретизации (сек)
    csignal <double> m_generatorArray;   // Cos генератор
};

#endif // COSTRANSFERMODULE_H
