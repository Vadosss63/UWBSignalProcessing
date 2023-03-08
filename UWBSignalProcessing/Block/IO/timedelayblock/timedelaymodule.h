#ifndef TIMEDELAYMODULE_H
#define TIMEDELAYMODULE_H

#include "iomodule.h"
#include "csignal.h"

class TimeDelayModule : public  IOModule<csignal<double>, csignal<double>>
{

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
