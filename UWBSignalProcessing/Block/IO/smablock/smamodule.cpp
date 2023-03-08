#include "smamodule.h"

SMAModule::SMAModule() :IOModule <csignal<double>, csignal<double>>(1, 1){}

void SMAModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> inSig;
    if(!PopFromInput(inSig))
        return;

    PushToOutput(inSig.SimpleMovingAverage(m_sizeWindow));
}

void SMAModule::SetSizeWindow(size_t sizeWindow)
{
    m_sizeWindow = sizeWindow;
}

// Exponential Moving Average
// output[i] = alpha*input[i] + (1-alpha)*output[i-1]
// 0<=alpha<=1
