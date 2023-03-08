#include "accumulationmodule.h"

//////////////////////////////////////////
///  Накопление
//////////////////////////////////////////
AccumulationModule::AccumulationModule() :
    IOModule(1,1), m_abstractAccumulation(new ConstAccumulation(1024))
{}

void AccumulationModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;
    if(!PopFromInput(temp))
        return;
    if(m_abstractAccumulation->Accumulation(temp))
    {
        m_abstractAccumulation->buffer(temp);
        temp.setFrameNumber(m_frame++);
        PushToOutput(std::move(temp));
    }
}

void AccumulationModule::StopEvent()
{
    m_frame = 0;
    IOModule::StopEvent();
}

void AccumulationModule::SetSizeBuf(size_t sizeBuf, TypeAccumulation type)
{
    std::lock_guard<std::mutex> lock(operateLock);
    
    delete m_abstractAccumulation;

    switch (type) {
    case constAccumulationType:
        m_abstractAccumulation = new ConstAccumulation(sizeBuf);
        break;
    case constAndSendIterationAccumulationType:
        m_abstractAccumulation = new ConstAndSendIterationAccumulation(sizeBuf);
        break;
    case cyclicAccumulationType:
        m_abstractAccumulation = new CyclicAccumulation(sizeBuf);
        break;
    default:
        break;
    }
}

//////////////////////////////////////////
///накопление Абстрактный класс
//////////////////////////////////////////
AbstractAccumulation::AbstractAccumulation(size_t sizeBuf): m_buffer(sizeBuf),  m_sizeBuf(sizeBuf){}

void AbstractAccumulation::buffer(csignal<double>& signal)
{
    signal = std::move(m_sum);
}

void AbstractAccumulation::clearBuf()
{
    m_buffer = boost::circular_buffer <csignal<double>>(m_sizeBuf);
}

bool AbstractAccumulation::IsTrueBuffer() const
{
    return m_sizeBuf <= nIteration();// сигнал накоплен
}

void AbstractAccumulation::PushSignal(csignal<double> &signal)
{
    m_azimuth = signal.azimuth();
    m_buffer.push_front(std::move(signal));
}

bool AbstractAccumulation::BufEmpty() const
{
    return m_buffer.empty();
}
uint32_t AbstractAccumulation::azimuth() const
{
    return m_azimuth;
}

void AbstractAccumulation::RezultSum()
{
    if(m_buffer.empty())
        return;

    m_sum.reset(m_buffer.back().size());
    m_sum = std::accumulate(m_buffer.begin(), m_buffer.end(), m_sum);
    // выполняется усреднение сигнала и установка кадра
    m_sum /= m_buffer.size();
    m_sum.setAzimuth(m_azimuth);
    // выполняем чистку буфера
    clearBuf();
}

size_t AbstractAccumulation::nIteration() const
{
    return m_buffer.size();
}


//////////////////////////////////////////
///Константное накопление
//////////////////////////////////////////

ConstAccumulation::ConstAccumulation(size_t sizeBuf):AbstractAccumulation(sizeBuf){}

bool ConstAccumulation::Accumulation(csignal<double>& temp)
{
    if (BufEmpty())// проверка на размер буфера
    {
        PushSignal(temp);
        if(IsTrueBuffer())
        {
            RezultSum();
            return true;
        }
        return false;
    }

//    // проверка на совпадения азимута
//    if (azimuth() == temp.azimuth())
//    {
        PushSignal(temp);
        if(IsTrueBuffer())
        {
            RezultSum();
            return true;
        }
        return false;

//    }

    RezultSum();
    PushSignal(temp);
    return true;
}

////////////////////////////////////////////////////////////////////
/// Константное накопление с отправкой на каждой итерации усреднения
///////////////////////////////////////////////////////////////////

ConstAndSendIterationAccumulation::ConstAndSendIterationAccumulation(size_t sizeBuf):AbstractAccumulation(sizeBuf){}

bool ConstAndSendIterationAccumulation::Accumulation(csignal<double> &temp)
{
    if (BufEmpty())// проверка на размер буфера
    {
        PushSignal(temp);
        RezultSum();
        return true;
    }
    // проверка на совпадения азимута
    if (azimuth() == temp.azimuth())
    {
        PushSignal(temp);
        RezultSum();
        return true;
    }

    RezultSum();
    PushSignal(temp);
    return true;
}

/////////////////////////////////////////////////////////////
/// Циклическое накопление
/////////////////////////////////////////////////////////////

CyclicAccumulation::CyclicAccumulation(size_t sizeBuf):AbstractAccumulation(sizeBuf){}

bool CyclicAccumulation::Accumulation(csignal<double> &temp)
{
    if (BufEmpty())// проверка на размер буфера
    {
        PushSignal(temp);
        RezultSum();
        return true;
    }

    if(azimuth() == temp.azimuth()) // проверка на азимут
    {
        PushSignal(temp);
        RezultSum();
        return true;
    }
    
    
        RezultSum();
        PushSignal(temp);
        return true;
    
}
