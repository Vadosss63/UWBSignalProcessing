#ifndef SOURCEMODULE_H
#define SOURCEMODULE_H

#include "abstractmodule.h"
#include "portsconnect.h"
#include <vector>

template <typename BufferTypeOut> class SourceModule : public AbstractModule {

public:
  SourceModule(uint8_t outputs);

  ~SourceModule() override;
  SourceModule(const SourceModule &rhs) = delete;
  SourceModule &operator=(const SourceModule &rhs) = delete;

  //получение доступа к портам
  OutputPort<BufferTypeOut> *GetOutput(uint8_t port);
  // количество портов
  uint8_t CountOutputPort() const;

  //команды для изменения числа портов
  void SetOutputPortsNumber(uint8_t number);

protected:
  //функции для заполнения выходного буфера
  void PushToOutput(const BufferTypeOut &element, uint8_t outputNo = 0);

  void PushToOutput(BufferTypeOut &&element, uint8_t outputNo = 0);

private:
  std::vector<OutputPort<BufferTypeOut> *> m_outputPorts;
};

template <typename BufferTypeOut>
SourceModule<BufferTypeOut>::SourceModule(uint8_t outputs) {
  for (uint8_t i = 0; i < outputs; i++)
    m_outputPorts.push_back(new OutputPort<BufferTypeOut>(this));

  SetEnableTimer(true);
}

template <typename BufferTypeOut> SourceModule<BufferTypeOut>::~SourceModule() {
  for (auto *op : m_outputPorts) {
    delete op;
  }
}

template <typename BufferTypeOut>
OutputPort<BufferTypeOut> *
SourceModule<BufferTypeOut>::GetOutput(uint8_t port) {
  return m_outputPorts.at(port);
}

template <typename BufferTypeOut>
uint8_t SourceModule<BufferTypeOut>::CountOutputPort() const {
  return m_outputPorts.size();
}

template <typename BufferTypeOut>
void SourceModule<BufferTypeOut>::SetOutputPortsNumber(uint8_t number) {
  for (size_t i = 0; i < m_outputPorts.size(); i++)
    delete m_outputPorts[i];

  m_outputPorts.clear();

  for (uint8_t i = 0; i < number; i++)
    m_outputPorts.push_back(new OutputPort<BufferTypeOut>(this));
}

template <typename BufferTypeOut>
void SourceModule<BufferTypeOut>::PushToOutput(const BufferTypeOut &element,
                                               uint8_t outputNo) {
  m_outputPorts.at(outputNo)->PushToOutput(element);
}

template <typename BufferTypeOut>
void SourceModule<BufferTypeOut>::PushToOutput(BufferTypeOut &&element,
                                               uint8_t outputNo) {
  m_outputPorts.at(outputNo)->PushToOutput(std::move(element));
}

#endif // SOURCEMODULE_H
