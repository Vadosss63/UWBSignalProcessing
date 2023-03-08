#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>

// выполняет вызов callback-а через определенный промежуток времени.
// timeType - определяет формат времени
template<typename TimeType>
class Timer
{
public:
    // конструктор по умочанию устанавливает заглушку на колбек
    Timer() noexcept
        :m_callback([](){}){}

    ~Timer()
    {
        if(IsStart())
            Stop();
    }

    // Приннимает на вход колбек и список параметров
    template<typename Func, typename... CallbackArg>
    void AddCallback(const Func callback, const CallbackArg&... callbackArg)
    {
        if(IsStart())
            Stop();
        m_callback = std::bind(callback, callbackArg...);
    }

    // Запуск таймера
    void Start(u_int time) noexcept
    {
        if(IsStart())
            Stop();

        m_isStart.store(true);
        m_time = TimeType(time);
        m_thread = std::thread(&Timer::Worker, this);

    }
    // Остановка таймера
    void Stop() noexcept
    {
        // остановка цикла
        m_isStart.store(false);
        // корректное завершения потока
        if (m_thread.joinable())
            m_thread.join();
    }
    // Проверка включен ли таймер
    bool IsStart() noexcept
    {
        return m_isStart.load();
    }

private:
    // функция вызова callback
    void Worker()
    {
        while(m_isStart)
        {
            std::this_thread::sleep_for(m_time);
            m_callback();
        }
    }
    // callback вызываемый таймером
    std::function<void()> m_callback;
    // Переменная работы цикла
    std::atomic<bool> m_isStart;
    // поток выполняющий callback
    std::thread m_thread;
    // формат врени типа std::chrono::TimeType
    TimeType m_time;
};

#endif // TIMER_H
