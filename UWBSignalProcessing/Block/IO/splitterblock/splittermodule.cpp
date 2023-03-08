#include "splittermodule.h"

SplitterModule::SplitterModule() :
    IOModule(1,2){}

void SplitterModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> tempSigmal;
    if (!PopFromInput(tempSigmal))
        return;

    for (uint8_t i = 0; i < CountOutputPort() - 1; i++)
        PushToOutput(tempSigmal, i);

    PushToOutput(std::move(tempSigmal), CountOutputPort() - 1);
}

