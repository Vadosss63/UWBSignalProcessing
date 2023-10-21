#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <mutex>

#include "abstractoperationmanager.h"
#include "timerevent.h"

class AbstractModule : public Task,
                       public EventOperationManager,
                       public TimerEvent {

public:
  AbstractModule() = default;
  ~AbstractModule() override;

  AbstractModule(const AbstractModule &rhs) = delete;
  AbstractModule &operator=(const AbstractModule &rhs) = delete;

  // Получаем указательна пул потоков
  void RegistrOperationManager(AbstractOperationManager *operationManager);
  // Событие призапуске пула потоков
  void StartEvent() override;
  // Событие при остановке пула потоков
  void StopEvent() override;
  // Событие при срабатывании таймера
  void TimerTimout() override;
  // Функция постановки модуля в пул потоков
  virtual void QueueOperation();

protected:
  void StopedModule();
  //блокировка однократного доступа к Operate()
  std::mutex operateLock;

private:
  // указатель на пул потоков
  AbstractOperationManager *m_operationManager = nullptr;
};

#endif // ABSTRACTMODULE_H
