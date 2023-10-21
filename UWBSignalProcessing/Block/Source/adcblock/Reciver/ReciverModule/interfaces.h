#ifndef INTERFACES_H
#define INTERFACES_H

#include "observer.h"
#include <string>
#include <vector>

class ControlWord;
/// инкапсулирует команды от userinterface
class Command {
public:
  virtual ~Command();
  virtual void ResetAnswer() = 0;
  virtual void ResetRequest() = 0;
  virtual void SetParameter(std::string name, uint8_t val) = 0;
  virtual void SetParameter(std::string name, uint16_t val) = 0;
  virtual void SetParameter(std::string name, uint32_t val) = 0;
  virtual std::string Name() const = 0;
  virtual std::vector<uint8_t> GetRequest() = 0;
  virtual std::vector<uint8_t> &GetAnswer() = 0;
  virtual std::vector<uint8_t> GetAnswerData() const = 0;
  virtual std::string GetError() = 0;
  virtual void SetError(const std::string &error) = 0;
  virtual bool IsNoError() const = 0;
  virtual bool CheckCRC() const = 0;
};

// получет команду от пользователя и преобразует ее к интерфейсу
class ControllerCommands : public Subject {
public:
  virtual ~ControllerCommands();
  virtual Command *ReadCommand();
  virtual void Start();
  virtual void Stop();
};

class HandlerDevice {
public:
  virtual ~HandlerDevice();
  // выполнение команды управления
  virtual bool ExicuteCommand(Command *);
  virtual bool Open(const std::string &);
  virtual std::string GetError();
  virtual bool IsNoError();
};

#endif // INTERFACES_H
