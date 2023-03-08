#ifndef GAINMODULE_H
#define GAINMODULE_H

#include "iomodule.h"
#include "csignal.h"

class GainModule: public IOModule<csignal<double>,  csignal<double>>
{

public:

    GainModule();
    ~GainModule() override = default;

    void Operate() override;
    void SetGainSignal(double setGainSignal);

private:    

    double m_gainSignal = 1;
};

#endif // GAINMODULE_H
