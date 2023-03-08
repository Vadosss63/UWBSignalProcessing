#ifndef IOMODULE_H
#define IOMODULE_H

#include <vector>
#include <iostream>
#include "abstractmodule.h"
#include "portsconnect.h"

template<typename BufferTypeIn, typename BufferTypeOut>
class IOModule: public AbstractModule
{
public:

    IOModule(uint8_t inputs, uint8_t outputs);
    ~IOModule() override;

    IOModule(const IOModule& rhs) = delete;
    IOModule& operator=(const IOModule& rhs) = delete;

    // получение доступа к портам
    InputPort<BufferTypeIn>* GetInput(uint8_t port);
    OutputPort<BufferTypeOut>* GetOutput(uint8_t port);

    // количество портов
    uint8_t CountInputPort() const;
    uint8_t CountOutputPort() const;

    //команды для изменения числа портов
    void SetInputPortsNumber(uint8_t number);
    void SetOutputPortsNumber(uint8_t number);

    //слот для обработки пришедших внешних или внутренних сигналов
    //в нем сигналы обрабатываются и в результате генерируется сигнал OperationRequest()
    void QueueOperation() override;

    //терминальная функция для окончания работы модуля
    void StopEvent() override;

protected:
    //функции для заполнения выходного буфера
    void PushToOutput(const BufferTypeOut& element, uint8_t outputNo = 0);

    void PushToOutput(BufferTypeOut &&element, uint8_t outputNo = 0);

    //функция для получения данных из входного буфера
    bool PopFromInput(BufferTypeIn& element, uint8_t inputNo = 0);

    void ReturnInputBuffer(BufferTypeIn &&element, uint8_t inputNo = 0);

private:
    std::vector<InputPort<BufferTypeIn>*> m_inputPorts;
    std::vector<OutputPort<BufferTypeOut>*> m_outputPorts;
};


template<typename BufferTypeIn, typename BufferTypeOut>
IOModule<BufferTypeIn, BufferTypeOut>::IOModule(uint8_t inputs, uint8_t outputs)
{
    for (uint8_t i = 0; i < inputs; i++)
        m_inputPorts.push_back(new InputPort<BufferTypeIn>(this));

    for (uint8_t i = 0; i < outputs; i++)
        m_outputPorts.push_back(new OutputPort<BufferTypeOut>(this));
}

template<typename BufferTypeIn, typename BufferTypeOut>
IOModule<BufferTypeIn, BufferTypeOut>::~IOModule()
{
    for (auto* op : m_outputPorts)
        delete op;

    for (auto* ip : m_inputPorts)
        delete ip;
}

template<typename BufferTypeIn, typename BufferTypeOut>
InputPort<BufferTypeIn>* IOModule<BufferTypeIn, BufferTypeOut>::GetInput(uint8_t port)
{
    return m_inputPorts.at(port);
}

template<typename BufferTypeIn, typename BufferTypeOut>
OutputPort<BufferTypeOut>* IOModule<BufferTypeIn, BufferTypeOut>::GetOutput(uint8_t port)
{
    return m_outputPorts.at(port);
}

template<typename BufferTypeIn, typename BufferTypeOut>
uint8_t IOModule<BufferTypeIn, BufferTypeOut>::CountInputPort() const
{
    return m_inputPorts.size();
}

template<typename BufferTypeIn, typename BufferTypeOut>
uint8_t IOModule<BufferTypeIn, BufferTypeOut>::CountOutputPort() const
{
    return m_outputPorts.size();
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::SetInputPortsNumber(uint8_t number)
{
    for(size_t i = 0; i < m_inputPorts.size(); i++)
        delete m_inputPorts[i];

    m_inputPorts.clear();

    for (uint8_t i = 0; i < number; i++)
        m_inputPorts.push_back(new InputPort<BufferTypeIn>(this));
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::SetOutputPortsNumber(uint8_t number)
{
    for(size_t i = 0; i < m_outputPorts.size(); i++)
        delete m_outputPorts[i];

    m_outputPorts.clear();

    for (uint8_t i = 0; i < number; i++)
        m_outputPorts.push_back(new OutputPort<BufferTypeOut>(this));
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::QueueOperation()
{
    if (std::all_of(m_inputPorts.begin(), m_inputPorts.end(), [](const InputPort<BufferTypeIn>* ip){ return !ip->Empty(); }))
        AbstractModule::QueueOperation();
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::StopEvent()
{
    std::for_each(m_inputPorts.begin(), m_inputPorts.end(), [](InputPort<BufferTypeIn>* ip){ ip->ClearDataQueue(); });
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::PushToOutput(const BufferTypeOut& element, uint8_t outputNo)
{
    m_outputPorts.at(outputNo)->PushToOutput(element);
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::PushToOutput(BufferTypeOut&& element, uint8_t outputNo)
{
    m_outputPorts.at(outputNo)->PushToOutput(std::move(element));
}

template<typename BufferTypeIn, typename BufferTypeOut>
bool IOModule<BufferTypeIn, BufferTypeOut>::PopFromInput(BufferTypeIn& element, uint8_t inputNo)
{
    return m_inputPorts.at(inputNo)->PopFromInput(element);
}

template<typename BufferTypeIn, typename BufferTypeOut>
void IOModule<BufferTypeIn, BufferTypeOut>::ReturnInputBuffer(BufferTypeIn&& element, uint8_t inputNo)
{
    m_inputPorts.at(inputNo)->ReturnInputBuffer(std::move(element));
}

#endif // IOMODULE_H
