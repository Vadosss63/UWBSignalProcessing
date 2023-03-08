#ifndef ABSTRACTOPERATIONMANAGER_H
#define ABSTRACTOPERATIONMANAGER_H

class Task
{
public:
    virtual ~Task() = default;
    // выполняемая операция
    virtual void Operate() = 0;
};

// интерфейс событий менэджера потоков
class EventOperationManager
{
public:
    virtual ~EventOperationManager() = default;
    virtual void StartEvent() = 0;
    virtual void StopEvent() = 0;
};

class AbstractOperationManager
{

public:
    virtual ~AbstractOperationManager() = default;
    // добавления модуля для выполнения задания
    virtual void AddTask(Task* module) = 0;
    // функция запуска пула потоков
    virtual void StartManager() = 0;
    // функция остановки пула потоков
    virtual void StopManager() = 0;
    // добавление слушателя событий
    virtual void AttachEvent(EventOperationManager* event) = 0;
    // удаления слушателя событий
    virtual void DetachEvent(EventOperationManager* event) = 0;
    // клонирования оператора пула потоков
    virtual AbstractOperationManager* Clone() = 0;
};

#endif // ABSTRACTOPERATIONMANAGER_H
