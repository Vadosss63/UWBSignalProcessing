#ifndef RECIEVERCONTROLMODULE_H
#define RECIEVERCONTROLMODULE_H

#include "interfaces.h"

// Класс представляет интерфейс настройки приемника, подключенного по интерфейсу
// RS485 Для настройки используются основные команды для данного интерфейса,
// описанные в классе RS485Handler

class RecieverControlModule : public Observer {
public:
  explicit RecieverControlModule();
  ~RecieverControlModule() override;
  RecieverControlModule(const RecieverControlModule &rhs) = delete;
  RecieverControlModule &operator=(const RecieverControlModule &rhs) = delete;

  void SetControllerCommands(ControllerCommands *controllerCommands);
  void SetHandlerDevice(HandlerDevice *handlerDevice);

  void Start();
  void Stop();

private:
  // вызывает Execute();
  void Update() override;
  // выполнение команды
  void Execute();

  ControllerCommands *m_controllerCommands = new ControllerCommands;
  HandlerDevice *m_handlerDevice = new HandlerDevice;
};

#endif // RECIEVERCONTROLMODULE_H
