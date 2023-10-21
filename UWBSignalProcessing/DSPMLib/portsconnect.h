#ifndef PORTSCONNECT_H
#define PORTSCONNECT_H

#undef emit

#include "abstractmodule.h"
#include "moduleconnectports.h"
#include <tbb/concurrent_queue.h>

template <class BufferType> class InputPort;
template <typename BufferType> class OutputPort : public OPort {
  friend class InputPort<BufferType>;

public:
  explicit OutputPort(AbstractModule *parent);
  ~OutputPort() override;

  OutputPort(const OutputPort &rhs) = delete;
  OutputPort &operator=(const OutputPort &rhs) = delete;

  //функция для выталкивания обработанного сигнала в выходную очередь
  void PushToOutput(const BufferType &element);
  void PushToOutput(BufferType &&element);

  //возвращает информацию о том, присоединен ли элемент или нет
  bool Connected() const;

  //вспомогательная функция для установления соединений
  bool Connect(IPort *newFront) override;

  //функции для разрыва связи со смежным элементом
  void Disconnect() override;

  //функция, возвращающая указатель на модуль, в который входит данный порт
  AbstractModule *ParentModule() const;

private:
  //указатель на присоединенный входной порт
  InputPort<BufferType> *m_connectedInputPort = nullptr;
  AbstractModule *m_parent = nullptr;
};

template <typename BufferType> class InputPort : public IPort {

  friend class OutputPort<BufferType>;

public:
  explicit InputPort(AbstractModule *parent);
  ~InputPort();

  InputPort(const InputPort &rhs) = delete;
  InputPort &operator=(const InputPort &rhs) = delete;

  //функция для получения сигналов из входной очереди
  bool PopFromInput(BufferType &element);

  // кладем данные в буффер входного пота
  void PushToBuffer(const BufferType &element);
  void PushToBuffer(BufferType &&element);

  // Функция ставит данные в свою очередь для возможности повторного вычитования
  void ReturnInputBuffer(BufferType &&element);
  //очистка очереди данных
  void ClearDataQueue();

  //возвращает информацию о том, присоединен ли элемент или нет
  bool Connected() const;

  //возвращает флаг пустого порта
  bool Empty() const;

  // вспомогательная функция для установления соединений
  bool Connect(OPort *newBack) override;

  //функции для разрыва связи со смежным элементом
  void Disconnect() override;

  //функция, возвращающая указатель на модуль, в который входит данный порт
  AbstractModule *ParentModule() const;

private:
  //входная очередь сигналов, предназначенных для обработки
  tbb::concurrent_queue<BufferType> m_dataBuffer;

  // собственная очередь для обработки
  tbb::concurrent_queue<BufferType> m_myDataBuffer;

  //указатель на присоединенный выходной порт
  OutputPort<BufferType> *m_connectedOutputPort = nullptr;
  AbstractModule *m_parent = nullptr;
};

template <typename BufferType>
OutputPort<BufferType>::OutputPort(AbstractModule *parent) : m_parent(parent) {}

template <typename BufferType> OutputPort<BufferType>::~OutputPort() {
  Disconnect();
}

template <typename BufferType>
void OutputPort<BufferType>::PushToOutput(const BufferType &element) {
  //вытолкнуть данные в выходной буфер
  if (Connected())
    m_connectedInputPort->PushToBuffer(element);
}

template <typename BufferType>
void OutputPort<BufferType>::PushToOutput(BufferType &&element) {
  if (Connected())
    m_connectedInputPort->PushToBuffer(std::move(element));
}

template <typename BufferType> bool OutputPort<BufferType>::Connected() const {
  return m_connectedInputPort;
}

template <typename BufferType>
bool OutputPort<BufferType>::Connect(IPort *newFront) {
  return newFront->Connect(this);
}

template <typename BufferType> void OutputPort<BufferType>::Disconnect() {
  if (Connected())
    m_connectedInputPort->Disconnect();
}

template <typename BufferType>
AbstractModule *OutputPort<BufferType>::ParentModule() const {
  return m_parent;
}

template <typename BufferType>
InputPort<BufferType>::InputPort(AbstractModule *parent) : m_parent(parent) {}

template <typename BufferType> InputPort<BufferType>::~InputPort() {
  Disconnect();
}

template <typename BufferType>
bool InputPort<BufferType>::PopFromInput(BufferType &element) {
  bool result = false;
  if (m_myDataBuffer.try_pop(element))
    return true;

  result = m_dataBuffer.try_pop(element);
  return result;
}

template <typename BufferType>
void InputPort<BufferType>::PushToBuffer(const BufferType &element) {
  //вытолкнуть данные в выходной буфер
  m_dataBuffer.push(element);
  // вызываем постановку на
  if (m_parent)
    m_parent->QueueOperation();
}

template <typename BufferType>
void InputPort<BufferType>::PushToBuffer(BufferType &&element) {
  // вытолкнуть данные в выходной буфер
  m_dataBuffer.push(element);
  // вызываем постановку на
  if (m_parent)
    m_parent->QueueOperation();
}

template <typename BufferType>
void InputPort<BufferType>::ReturnInputBuffer(BufferType &&element) {
  m_myDataBuffer.push(element);
}

template <typename BufferType> void InputPort<BufferType>::ClearDataQueue() {
  if (!m_dataBuffer.empty())
    m_dataBuffer.clear();
}

template <typename BufferType> bool InputPort<BufferType>::Connected() const {
  return m_connectedOutputPort;
}

template <typename BufferType> bool InputPort<BufferType>::Empty() const {
  return m_dataBuffer.empty();
}

template <typename BufferType>
bool InputPort<BufferType>::Connect(OPort *newBack) {
  // проверка на соответствие портов входного и выходного порта
  OutputPort<BufferType> *outputPort =
      dynamic_cast<OutputPort<BufferType> *>(newBack);

  if (!outputPort)
    return false;

  // проверка на зациклинность входного и выходного порта у обного модуля
  if (ParentModule() == outputPort->ParentModule())
    return false;

  Disconnect();

  m_connectedOutputPort = outputPort;
  outputPort->m_connectedInputPort = this;
  return true;
}

template <typename BufferType> void InputPort<BufferType>::Disconnect() {
  // выполняем очистку входного буфера
  ClearDataQueue();
  if (Connected()) // обнуляем указатели если присутствует входной порт
  {
    m_connectedOutputPort->m_connectedInputPort = nullptr;
    m_connectedOutputPort = nullptr;
  }
}

template <typename BufferType>
AbstractModule *InputPort<BufferType>::ParentModule() const {
  return m_parent;
}

#endif // PORTSCONNECT_H
