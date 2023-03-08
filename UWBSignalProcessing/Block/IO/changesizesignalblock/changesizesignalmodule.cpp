#include "changesizesignalmodule.h"

ChangeSizeSignalModule::ChangeSizeSignalModule() :
    IOModule(1, 1) {}

void ChangeSizeSignalModule::SetParameters(size_t start, size_t size)
{
    m_start = start;
    m_size = size;
}

void ChangeSizeSignalModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;

    if(!PopFromInput(temp))
        return;

    if(temp.size() < m_size + m_start) return;

    csignal<double> newSig(m_size);

    std::copy(temp.begin() + m_start, temp.begin() + m_start + m_size, newSig.begin());
    newSig.setServiceParameters(temp.azimuth(), temp.frameNumber());
    PushToOutput(std::move(newSig));
}



