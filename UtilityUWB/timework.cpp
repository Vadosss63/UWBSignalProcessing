#include "timework.h"

namespace UtilityUWB {

TimeWork::TimeWork(): m_isStart(false){}

void TimeWork::Start()
{
    m_isStart = true;
    // запуск секундомера
    m_startTime = std::chrono::steady_clock::now();
}

void TimeWork::Stop()
{
    if(m_isStart){
        // остановка секундомера
        m_endTime = std::chrono::steady_clock::now();
        auto res_time = std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime);
        std::cout << "Время вычисления мили сек = " << res_time.count() << std::endl;
        m_isStart = false;
    }
}

} //UtilityUWB
