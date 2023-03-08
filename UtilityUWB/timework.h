#ifndef TIMEWORK_H
#define TIMEWORK_H

#include <iostream>
#include <chrono>

namespace UtilityUWB {

class TimeWork
{
public:
    TimeWork();
    void Start();
    void Stop();
private:
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::steady_clock::time_point m_endTime;
    bool m_isStart;
};

} // UtilityUWB
#endif // TIMEWORK_H
