#include "singeneratormodule.h"

SinGeneratorModule::SinGeneratorModule()
    :SourceModule< csignal<double>>(static_cast<uint8_t>(1)), m_lenSignal(2048),
      m_frequency(2 * M_PI* 1E+6), m_amplitude(1), m_phase(0), m_sampleTime(2E-9), m_generatorArray(m_lenSignal), m_az(0), m_frame(0)
{
    StartGenerator();
    m_frame = 0;
    csignal<double>::setAzimuthVolume(64);
    //m_generatorArray.set_azimuth_volume(4*64);
}

void SinGeneratorModule::StartGenerator()
{
    // аргумент синуса
    double argum = m_frequency * m_sampleTime;
    for(size_t i = 0; i < m_lenSignal; i++)
        m_generatorArray[i] = m_amplitude * std::sin(argum * i  + m_phase);
}

void SinGeneratorModule::ChangeParametersGenerator(double frequency, double amplitude,
                                                   double phase, bool sampleTime)
{    
    m_frequency = 2 * M_PI * frequency * 1E+6;
    m_amplitude = amplitude;
    m_phase = (2 * M_PI * phase)/360;
    if(!sampleTime)
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

void SinGeneratorModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    m_generatorArray.setFrameNumber(m_frame++);
    m_generatorArray.setAzimuth(m_az++);
    PushToOutput(m_generatorArray);
}

void SinGeneratorModule::StopEvent()
{
    m_frame = 0;
    m_az = 0;
    SourceModule::StopEvent();
}

void SinGeneratorModule::TimerTimout()
{
    QueueOperation();
}
