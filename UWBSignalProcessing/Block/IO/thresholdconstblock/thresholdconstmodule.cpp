#include "thresholdconstmodule.h"

ThresholdModule::ThresholdModule():
    IOModule(1, 1) {}

void ThresholdModule::SetParameters(double threshold)
{
    m_threshold = threshold;
}

void ThresholdModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;
    if(!PopFromInput(temp))
        return;

    std::for_each(temp.begin(), temp.end(),
                  [&](double& val)
    {
        val = val - m_threshold;
        val = val < 0 ? 0 : val;
    });

    PushToOutput(std::move(temp));
}

