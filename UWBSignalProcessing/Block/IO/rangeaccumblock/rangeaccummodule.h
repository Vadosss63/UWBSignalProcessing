#ifndef RANGEACCUMMODULE_H
#define RANGEACCUMMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include "windows.h"
#include <fftw3.h>

enum TypeThreshol { Non = 0, Bin, Indicating, IndicatingAdapte };

typedef std::pair<csignal<double>, csignal<double>> InBuffer;

class RangeAccumModule
    : public IOModule<InBuffer, std::vector<std::valarray<double>>> {

public:
  RangeAccumModule();
  ~RangeAccumModule() override;

  void SetSetting(size_t countAcumm, size_t startRange, size_t range,
                  double threshol);
  void SetTypeThreshol(int type);
  void SetTypeWindow(int type);
  void SetIsVARU(bool m_isVARU);
  void SetGain(double gain);
  void Operate() override;
  void StopEvent() override;

private:
  void SetCountAccum(size_t countAcumm);
  void SetStartRange(size_t startRange);
  void SetRange(size_t range);

  void ResetSignal();

  // изменение размера сигнала
  void SetPlan();
  // Удаление плана
  void ClearPlan();

  void Execute();

  void CreateWindow();

  void VARU();

  inline double GetABSVal(size_t i, size_t j) const;

  void NonThreshol(std::vector<std::valarray<double>> &data, size_t i);
  void BinThreshol(std::vector<std::valarray<double>> &data, size_t i);
  void IndicatingThreshol(std::vector<std::valarray<double>> &data, size_t i);

  void IndicatingAdapteThreshol(std::vector<std::valarray<double>> &data,
                                size_t i);

  double m_threshol = 100;

  // начальное значение накопления
  size_t m_startRange = 0;
  // диапазон дальностей
  size_t m_range = 1;
  // количество накоплений
  size_t m_countAcumm = 1;
  // Значение текущего накопления
  size_t m_currentAccum = 0;
  // ВАРУ
  bool m_isVARU = false;
  // Нормирование по N
  double m_gain = false;
  // Тип порога
  TypeThreshol m_typeThreshol = TypeThreshol::Non;
  // Тип окна
  TypeWindow m_typeWindow = TypeWindow::noWindow;
  // окно hamminga
  std::vector<double> m_window;

  // буффер для накопления
  std::vector<std::valarray<std::complex<double>>> m_accumSignal;
  // набор планов преобразования fft
  std::vector<fftw_plan> plan_fft;
};

#endif // RANGEACCUMMODULE_H
