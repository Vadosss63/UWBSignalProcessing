#ifndef IQTOMAGNITUDE_ANGLEMODULE_H
#define IQTOMAGNITUDE_ANGLEMODULE_H

#include <fftw3.h>
#include "iomodule.h"
#include "csignal.h"

class IQtoMagnitude_AngleModule : public IOModule< csignal<double>,  csignal<double>>
{    
public:

    IQtoMagnitude_AngleModule();
    ~IQtoMagnitude_AngleModule() override;

    void Operate() override;

private:

    void SetPlan(size_t len);

    csignal<std::complex<double>> TempFFT;
    fftw_plan plan_fft;
};

#endif // IQTOMAGNITUDE_ANGLEMODULE_H
