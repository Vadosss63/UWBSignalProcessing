#include "timerevent.h"

void TimerListEvents::AttachEvent(TimerEvent* event)
{
    if(!event->IsEnableTimer())
        return;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_listEvent.push_front(event);
    event->RegisterTimer(this);
}

void TimerListEvents::DetachEvent(TimerEvent* event)
{
    if(!event->IsEnableTimer())
        return;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_listEvent.remove(event);
}

void TimerListEvents::Timout()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::for_each(m_listEvent.begin(), m_listEvent.end(), [](TimerEvent* event){event->TimerTimout();});
}

TimerEvent::~TimerEvent()
{
    m_timer->DetachEvent(this);
}

void TimerEvent::TimerTimout(){}

void TimerEvent::SetEnableTimer(bool isEnable)
{
    m_isEnable = isEnable;
}

bool TimerEvent::IsEnableTimer()
{
    return m_isEnable;
}

void TimerEvent::RegisterTimer(TimerListEvents* timer)
{
    m_timer = timer;
}
