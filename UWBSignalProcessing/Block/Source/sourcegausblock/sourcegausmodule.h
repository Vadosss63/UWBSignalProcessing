#ifndef SOURCEGAUSMODULE_H
#define SOURCEGAUSMODULE_H

#include "sourcemodule.h"
#include "csignal.h"

class SourceGausModule : public SourceModule< csignal<double>>
{

public:

    SourceGausModule();
    ~SourceGausModule() override = default;

    void Operate() override;
    void StopEvent() override;
    void TimerTimout() override;
    // Запуск генератора с новыми параметрами
    void ChangeParametersGenerator(double amplitude, size_t lenSignal, double sigma, double expectation);

private:

    // расчет генератора по первой производной гаусса
    void StartGenerator();
    // буфер для генерации сигнала
    csignal<double> m_generatorArray;
    // длина сигнала
    size_t m_lenSignal;
    // амплитуда
    double m_amplitude;
    // дисперсия (длительность)
    double m_sigma;
    // матожидание (смещение)
    double m_expectation;
    unsigned short m_az;
    unsigned short m_frame;
};

#endif // SOURCEGAUS_H
