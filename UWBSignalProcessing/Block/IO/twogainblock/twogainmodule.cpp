#include "twogainmodule.h"

TwoGainModule::TwoGainModule():
    IOModule(1, 1){}

void TwoGainModule::SetIndexOtherGainSignal(size_t indexOtherGainSignal)
{
    m_indexOtherGainSignal = indexOtherGainSignal;
}

bool TwoGainModule::CheckSizeSignal(size_t size)
{
    return m_firstSignal.size() == size;
}

void TwoGainModule::ResetParameters()
{
    m_firstSignal.clear();
    m_isFirst = true;
}

void TwoGainModule::StartEvent()
{
    ResetParameters();
}

void TwoGainModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    csignal<double> temp;
    if(!PopFromInput(temp))
        return;

    if(m_isFirst)// инициализиуем первый сигнал
    {
        m_firstSignal = std::move(temp);
        m_isFirst = false;
        return;
    }

    if(!CheckSizeSignal(temp.size())) // проверяем размерность
    {
        ResetParameters();
        return;
    }

    if(temp.size() <= m_indexOtherGainSignal) // проверям значение индекса на размерность сигнала
    {
        ResetParameters();
        return;
    }

    // копируем данные из первого сигнала во второй
    std::copy(m_firstSignal.begin(), m_firstSignal.begin() + m_indexOtherGainSignal, temp.begin());
    m_isFirst = true;
    PushToOutput(std::move(temp));
}
