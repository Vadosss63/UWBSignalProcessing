#include "timedelaymodule.h"

TimeDelayModule::TimeDelayModule() :
     IOModule(1, 1){}

void TimeDelayModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> tempSig;
    if(!PopFromInput(tempSig))
        return;

    tempSig.shift(m_numberOfZeros);
    PushToOutput(std::move(tempSig));
}

void TimeDelayModule::SetNumberOfZeros(int numberOfZeros)
{
    m_numberOfZeros = -1 * numberOfZeros;
}

