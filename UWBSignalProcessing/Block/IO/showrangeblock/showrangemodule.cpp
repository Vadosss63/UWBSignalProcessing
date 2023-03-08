#include "showrangemodule.h"

ShowRangeModule::ShowRangeModule() :
    IOModule(1, 1){}

void ShowRangeModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    std::vector<csignal<double>> m_scan;
    if(!PopFromInput(m_scan))
        return;

    if(m_scan.empty())
        return;

    if(m_range < m_scan[0].size())
    {
        csignal<double> range(m_scan.size());

        for (size_t i = 0; i < m_scan.size(); ++i)
            range[i] = m_scan[i][m_range];

        PushToOutput(std::move(range));
    }
}

void ShowRangeModule::SetRange(size_t range)
{
    std::lock_guard<std::mutex> lock(operateLock);
    m_range = range;
}
