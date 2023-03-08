#ifndef OPERATIONMANAGER_H
#define OPERATIONMANAGER_H

#include <future>
#include <thread>
#include <functional>
#include <vector>
#include <forward_list>
#include <tbb/concurrent_queue.h>
#include "abstractoperationmanager.h"

//обертка пула потоков для подключение средств Qt
class OperationManager: public AbstractOperationManager
{
    //класс - пул потоков
    //содержит набор рабочих потоков, очередь задач,
    //а так же методы добавления задач в очередь и назначения задачи потоку из списка
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t countThread);
        ~ThreadPool();
        //добавление задачи в виде указателя на выполняемый модуль в очередь задач
        void AddTask(Task* module);

    private:
        //основная функция, в которой находятся все потоки
        //пока флаг работы true фунция проверяет наличие задач в очереди
        //если задача есть - забирает ее себе на выполение
        //в противном случае ресурсы потока отдаются операционной системе
        //для перераспределения
        void ThreadTask();
        //флаг работы пула потоков
        std::atomic_bool m_operating;
        //очередь задач для выполнения пулом потоков
        tbb::concurrent_queue<Task*> m_operationQueue;
        //набор потоков, принадлежащих пулу
        std::vector<std::thread> m_pool;
    };

public:

    OperationManager() = default;
    ~OperationManager() override = default;

    // функция запуска пула потоков - создает объект ThreadPool
    void StartManager() override;
    // функция остановки пула потоков - удаляет объект ThreadPool
    void StopManager() override;
    // добаления задания
    void AddTask(Task* module) override;

    void AttachEvent(EventOperationManager* event) override;
    void DetachEvent(EventOperationManager* event) override;

    AbstractOperationManager* Clone() override;
    // методы установки количества потоков в пуле
    static size_t countThread();
    static void setCountThread(size_t countThread);

private:

    // Коректная остановка патоков
    void StopThreads();
    // Оповещение о запуске менэджера
    void StartEvent();
    // Оповещение о остановке менэджера
    void StopEvent();

    std::unique_ptr<ThreadPool> m_theadPool;
    // количество рабочих потоков
    static size_t m_countThread;
    // лист событий для вызова во время старта и остановки менэджера
    std::forward_list<EventOperationManager*> m_listEvets;
    std::future<void> m_stop;
    volatile bool m_isStart = false;
};

#endif // OPERATIONMANAGER_H
