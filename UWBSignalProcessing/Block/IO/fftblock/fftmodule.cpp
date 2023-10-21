#include "fftmodule.h"

FFTmodule::FFTmodule()
    : IOModule(static_cast<uint8_t>(1), static_cast<uint8_t>(2)), TempFFT(2048),
      m_sigTemp(2048) {
  //Создание плана для прямого преобразования Фурье
  plan_fft_forward =
      fftw_plan_dft_r2c_1d(TempFFT.size(), (double *)&m_sigTemp[0],
                           (fftw_complex *)&TempFFT[0], FFTW_ESTIMATE);
}

FFTmodule::~FFTmodule() { fftw_destroy_plan(plan_fft_forward); }

void FFTmodule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> temp;
  if (!PopFromInput(temp))
    return;

  // проверка размерности
  if (temp.size() != TempFFT.size())
    SetPlan(temp.size());

  std::copy(temp.begin(), temp.end(), m_sigTemp.begin());
  fftw_execute(plan_fft_forward);
  TempFFT.setServiceParameters(temp.azimuth(), temp.frameNumber());
  //отправка в порт 0 фазы
  PushToOutput(ArgComplex(TempFFT), 0);
  //отправка в порт 1 модуля амплитуды
  PushToOutput(AbsComplex(TempFFT), 1);
}

void FFTmodule::SetPlan(size_t len) {
  TempFFT.reset(len);
  m_sigTemp.reset(len);
  plan_fft_forward =
      fftw_plan_dft_r2c_1d(TempFFT.size(), (double *)&m_sigTemp[0],
                           (fftw_complex *)&TempFFT[0], FFTW_ESTIMATE);
}
