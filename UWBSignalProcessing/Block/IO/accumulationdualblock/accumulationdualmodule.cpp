#include "accumulationdualmodule.h"
#include <iostream>

//////////////////////////////////////////
///  Накопление
//////////////////////////////////////////
AccumulationDualModule::AccumulationDualModule() :
    IOModule(1,1){}

void AccumulationDualModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> inSig;
    if(!PopFromInput(inSig))
        return;

    if(m_sizeBuf >= (2 * m_sizeFA))
    {
        if(azimuth() != inSig.azimuth()) // проверка на азимут
        {
            m_sumSignal.clear();
            m_counterBuff = 0;
            SetAzimuth(inSig.azimuth());
        }
        m_counterBuff++;

        if((m_counterBuff - 1) < m_sizeFA)
        {
            m_sumSignal += inSig;
        } else  if((m_counterBuff) > (m_sizeBuf - m_sizeFA))
        {
            m_sumSignal -= inSig;
        }

        if((m_counterBuff) >= m_sizeBuf)
        {
            m_sumSignal /= m_sizeFA;
            m_sumSignal.setFrameNumber(m_frame++);
            PushToOutput(m_sumSignal);
            m_counterBuff = 0;
            m_sumSignal.clear();
        }
    }

}

uint32_t AccumulationDualModule::azimuth() const
{
    return m_azimuth;
}

void AccumulationDualModule::buffer(csignal<double>& signal)
{
    signal = std::move(m_sumSignal);
}

void AccumulationDualModule::SetAzimuth(const uint32_t& az)
{
    m_azimuth = az;
}

void AccumulationDualModule::StopEvent()
{
    m_frame = 0;
    m_counterBuff = 0;
    m_sumSignal.clear();
    IOModule::StopEvent();
}

void AccumulationDualModule::SetSizeBuf(size_t sizeBuf, size_t sizeFA)
{
    std::lock_guard<std::mutex> lock(operateLock);
    m_sizeBuf = sizeBuf;
    m_sizeFA = sizeFA;
    m_frame = 0; //?
    m_counterBuff = 0;
    m_sumSignal.clear();
}
