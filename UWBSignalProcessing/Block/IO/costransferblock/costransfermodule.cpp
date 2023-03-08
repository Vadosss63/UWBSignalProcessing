#include "costransfermodule.h"

CosTransferModule::CosTransferModule()
    :IOModule(1, 1), m_generatorArray(m_lenSignal)
{
    StartGenerator();
}

void CosTransferModule::StartGenerator()
{
    double argum = m_frequency * m_sampleTime;
    if(m_phaseSin)
        for(size_t i = 0; i < m_lenSignal; i++)
            m_generatorArray[i] = std::sin(argum * i);
    else
        for(size_t i = 0; i < m_lenSignal; i++)
            m_generatorArray[i] = std::cos(argum * i);
}

void CosTransferModule::ChangeParametersGenerator(double frequency, bool phaseSin, bool simpleFrequency500MHz)
{
    m_frequency = 2 * M_PI * frequency * 1E+6;
    m_phaseSin = phaseSin;
    if (simpleFrequency500MHz)
    {
        m_sampleTime = 2E-9;
        m_lenSignal = 2048;
        m_generatorArray.reset(m_lenSignal);
    }
    else
    {
        m_sampleTime = 5E-10;
        m_lenSignal = 8192;
        m_generatorArray.reset(m_lenSignal);
    }
    StartGenerator();
}

void CosTransferModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;
    if(!PopFromInput(temp))
        return;
    if (temp.size() == m_generatorArray.size())
    {
        PushToOutput(temp * m_generatorArray);
    }
    else
    {
        m_lenSignal = temp.size();
        StartGenerator();
        PushToOutput(temp * m_generatorArray);
    }
}
