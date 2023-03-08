#include "accumphasermodule.h"
#include <iostream>

AccumPhaserModule::AccumPhaserModule():
    IOModule(2, 2){}

void AccumPhaserModule::ResetCurrentAccumulation()
{
    m_currentAccumalation = 0;
}

void AccumPhaserModule::SetFrameToSignals()
{
    m_sigI.setFrameNumber(m_currentFrame++);
    m_sigQ.setFrameNumber(m_currentFrame++);
}

void AccumPhaserModule::ScalingSignals()
{
    m_sigI /= m_currentAccumalation + 1;
    m_sigQ /= m_currentAccumalation + 1;
}

bool AccumPhaserModule::CheckAzimut(uint32_t azimuthI, uint32_t azimuthQ)
{
    bool azimutTrue = (m_sigQ.azimuth() == azimuthQ) && (m_sigI.azimuth() == azimuthI);
    return azimutTrue;
}

void AccumPhaserModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    csignal<double>sigI;

    if(!PopFromInput(sigI, 0))
        return;

    csignal<double>sigQ;
    if(!PopFromInput(sigQ, 1))
        return;

    // проверка размера сигнала
    size_t sizeSig = sigQ.size();
    ResizeAccumulations(sizeSig);

    if(m_countAccum == 1)
    {
        // расчет сдвига фазы
        // накопление со сдвигом
        for (size_t i = 0; i < sizeSig; ++i)
        {
            m_expJ = std::exp(std::complex<double>(0, - m_phase * m_currentAccumalation));

            std::complex<double>temp = std::complex<double>(sigI[i], sigQ[i]) * m_expJ;
            sigI[i] = temp.real();
            sigQ[i] = temp.imag();
        }

        m_currentAccumalation++;

        //отправка I канала
        PushToOutput(std::move(sigI), 0);
        //отправка Q канала
        PushToOutput(std::move(sigQ), 1);
        return;
    }


    // инициализация первого накопления
    if(!m_currentAccumalation)
    {
        m_sigI = std::move(sigI);
        m_sigQ = std::move(sigQ);
        m_currentAccumalation++;
        return;
    }
    // проверка на совпадения углов
    bool azimutTrue = CheckAzimut(sigI.azimuth(), sigQ.azimuth());
    if(azimutTrue)
    {
        // расчет сдвига фазы
        std::complex<double> expJ = std::exp(std::complex<double>(0, - m_phase * m_currentAccumalation));
        // накопление со сдвигом
        for (size_t i = 0; i < sizeSig; ++i)
        {
            std::complex<double>temp = std::complex<double>(sigI[i], sigQ[i]) * expJ;
            m_sigI[i] += temp.real();
            m_sigQ[i] += temp.imag();
        }

        m_currentAccumalation++;
        // проверка выполнения буфера

            if(m_currentAccumalation < m_countAccum)
                return;
    }

    // выполняем маштобирование сигнала
    ScalingSignals();
    // установка фрейма
    SetFrameToSignals();

    //отправка I канала
    PushToOutput(std::move(m_sigI), 0);
    //отправка Q канала
    PushToOutput(std::move(m_sigQ), 1);

    //сброс текущего счетчика накопления в ноль
    ResetCurrentAccumulation();
}

void AccumPhaserModule::SetCountAccum(uint16_t countAccum)
{
    m_countAccum = countAccum;
}

void AccumPhaserModule::SetPhase(double phase)
{
    m_phase = (phase * M_PI)/(180);
}

void AccumPhaserModule::ResizeAccumulations(size_t newSize)
{
    if(m_sigI.size() != newSize)
    {
        m_sigI.reset(newSize);
        m_sigQ.reset(newSize);
        if(m_countAccum != 1)
            m_currentAccumalation = 0;
    }
}

void AccumPhaserModule::StartEvent()
{
    m_currentAccumalation = 0;
    m_currentFrame = 0;
}
