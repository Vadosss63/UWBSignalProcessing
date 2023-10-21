#ifndef SINKMODULE_H
#define SINKMODULE_H

#include "abstractmodule.h"
#include "portsconnect.h"
#include <vector>

template <typename BufferTypeIn> class SinkModule : public AbstractModule {

public:
  //конструктор выделяет память для ModuleInput, указывая себя в качестве его
  //родителя
  SinkModule(uint8_t inputs);

  ~SinkModule() override;

  SinkModule(const SinkModule<BufferTypeIn> &rhs) = delete;
  SinkModule &operator=(const SinkModule<BufferTypeIn> &rhs) = delete;

  //получение доступа к портам
  InputPort<BufferTypeIn> *GetInput(uint8_t port);
  // количество портов
  uint8_t CountInputPort() const;

  //команды для изменения числа портов
  void SetInputPortsNumber(uint8_t number);

  //слот для обработки пришедших внешних или внутренних сигналов
  //в нем сигналы обрабатываются и в результате генерируется сигнал
  //OperationRequest()
  void QueueOperation() override;

  //терминальная функция для окончания работы модуля
  void StopEvent() override;

protected:
  //функция для получения данных из входного буфера
  bool PopFromInput(BufferTypeIn &element, uint8_t inputNo = 0);

private:
  std::vector<InputPort<BufferTypeIn> *> inputPorts;
};

template <typename BufferTypeIn>
SinkModule<BufferTypeIn>::SinkModule(uint8_t inputs) {
  for (uint8_t i = 0; i < inputs; i++)
    inputPorts.push_back(new InputPort<BufferTypeIn>(this));
}

template <typename BufferTypeIn> SinkModule<BufferTypeIn>::~SinkModule() {
  for (auto *ip : inputPorts) {
    delete ip;
  }
}

template <typename BufferTypeIn>
InputPort<BufferTypeIn> *SinkModule<BufferTypeIn>::GetInput(uint8_t port) {
  return inputPorts.at(port);
}

template <typename BufferTypeIn>
uint8_t SinkModule<BufferTypeIn>::CountInputPort() const {
  return inputPorts.size();
}

template <typename BufferTypeIn>
void SinkModule<BufferTypeIn>::SetInputPortsNumber(uint8_t number) {
  for (size_t i = 0; i < inputPorts.size(); i++)
    delete inputPorts[i];

  inputPorts.clear();

  for (uint8_t i = 0; i < number; i++)
    inputPorts.push_back(new InputPort<BufferTypeIn>(this));
}

template <typename BufferTypeIn>
void SinkModule<BufferTypeIn>::QueueOperation() {
  if (std::all_of(
          inputPorts.begin(), inputPorts.end(),
          [](const InputPort<BufferTypeIn> *ip) { return !ip->Empty(); }))
    AbstractModule::QueueOperation();
}

template <typename BufferTypeIn> void SinkModule<BufferTypeIn>::StopEvent() {
  std::for_each(inputPorts.begin(), inputPorts.end(),
                [](InputPort<BufferTypeIn> *ip) { ip->ClearDataQueue(); });
}

template <typename BufferTypeIn>
bool SinkModule<BufferTypeIn>::PopFromInput(BufferTypeIn &element,
                                            uint8_t inputNo) {
  return inputPorts.at(inputNo)->PopFromInput(element);
}

#endif // SINKMODULE_H
