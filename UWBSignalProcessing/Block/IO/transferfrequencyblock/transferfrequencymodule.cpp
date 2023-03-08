#include "transferfrequencymodule.h"

TransferFrequencyModule::TransferFrequencyModule():
    IOModule(2, 2), m_gen(m_lengthSig)
{
    Init();
}

void TransferFrequencyModule::Operate()
{
    csignal<double> tempI;
    if (!PopFromInput(tempI, 0))
        return;

    csignal<double> tempQ;
    if(!PopFromInput(tempQ, 1))
        return;


    // сигнал в комплекстном виде
    std::lock_guard<std::mutex> lock(operateLock);

    csignal<std::complex<double>> cSignal(tempQ.size());
    std::transform(tempI.begin(), tempI.end(), tempQ.begin(), cSignal.begin(),
                   [](double a, double b)
    {
        return std::complex<double>(a, b);
    });

    tempQ.reset(tempQ.size());
    auto interQ = tempQ.begin();
    std::transform(cSignal.begin(), cSignal.end(), m_gen.begin(), tempI.begin(),
                   [&interQ](std::complex<double>& sig, const std::complex<double>& gen)
    {
        std::complex<double> result = sig*gen;
        *interQ = result.imag();
        interQ++;
        return result.real();
    });

    //отправка в порт 0 Re
    PushToOutput(std::move(tempI));
    //отправка в порт 1 Im
    PushToOutput(std::move(tempQ), 1);
}

void TransferFrequencyModule::Init()
{
    //   m_gen =  csignal<std::complex<double>>(m_lengthSig);
    // частота переноса 500 МГц
    //  double  m_frequency = 2 * M_PI *(0.5*1E+8);
    //    // период дискретизации 0.5 НСек
    // double   m_sampleTime = 5E-10;
    // период дискретизации 1 НСек
    //double m_sampleTime = 2.5E-10;
    // вычисление x для синуса и косинуса
    double argum = m_frequency * m_sampleTime;

    for (size_t i = 0; i < m_lengthSig; ++i)
    {
        // m_gen[i] = std::complex<double>(std::sin(argum * i),
        // -std::cos(argum * i));
        m_gen[i] = std::complex<double>(std::cos(argum * i), -std::sin(argum * i));
    }
}

void TransferFrequencyModule::SetTransferFrequency(double transfreq, size_t samp)
{
    double sampfreq;

    if(samp == 2048)
        sampfreq = 500;
    else if(samp == 4096)
        sampfreq = 1000;
    else
        sampfreq = 2000;

    m_lengthSig = samp;

    m_gen.reset(m_lengthSig);

    m_frequency = 2 * M_PI*transfreq;
    m_sampleTime = (1.0/(sampfreq));
    Init();
}
