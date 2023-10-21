#ifndef SINGENERATORMODULE_H
#define SINGENERATORMODULE_H

#include "csignal.h"
#include "sourcemodule.h"

class SinGeneratorModule : public SourceModule<csignal<double>> {

public:
  SinGeneratorModule();
  ~SinGeneratorModule() override = default;

  void Operate() override;
  void ChangeParametersGenerator(double frequency, double amplitude,
                                 double phase, bool sampleTime);

  void StopEvent() override;
  void TimerTimout() override;

private:
  // Запуск генератора
  // создание массива sin
  void StartGenerator();
  size_t m_lenSignal;  // Длина сигнала
  double m_frequency;  // Частота генератора (Гц)
  double m_amplitude;  // Амплитуда
  double m_phase;      // Фаза (Градусах)
  double m_sampleTime; // Период дискретизации (Сек)
  csignal<double> m_generatorArray; // sin генератор
  uint8_t m_az;
  uint8_t m_frame;
};

#endif // SINGENERATORMODULE_H
