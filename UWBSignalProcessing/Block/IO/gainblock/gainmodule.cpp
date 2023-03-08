#include "gainmodule.h"

GainModule::GainModule():
    IOModule(1, 1){}

void GainModule::SetGainSignal(double setGainSignal)
{
    m_gainSignal = setGainSignal;
}

void GainModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;
    if(!PopFromInput(temp))
        return;

    temp *= m_gainSignal;
    PushToOutput(std::move(temp));
}
