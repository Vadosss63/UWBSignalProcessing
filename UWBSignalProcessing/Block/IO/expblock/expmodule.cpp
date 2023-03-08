#include "expmodule.h"

ExpModule::ExpModule(): IOModule(1, 1) {}

void ExpModule::SetParameters(double threshold)
{
    m_threshold = threshold;
}

void ExpModule::Operate()
{
    csignal<double> temp;
    if(!PopFromInput(temp))
        return;

    std::for_each(temp.begin(), temp.end(),
                  [&](double& val)
    {
        val = std::exp(val - m_threshold);
    });

    PushToOutput(std::move(temp));
}
