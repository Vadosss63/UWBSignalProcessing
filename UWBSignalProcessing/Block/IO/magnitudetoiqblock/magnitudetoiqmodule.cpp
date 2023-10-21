#include "magnitudetoiqmodule.h"

Magnitude_AngleModuleToIQModule::Magnitude_AngleModuleToIQModule()
    : IOModule(static_cast<uint8_t>(2), static_cast<uint8_t>(2)),
      TempFFT(2048) {
  plan_fft = fftw_plan_dft_1d(TempFFT.size(), (fftw_complex *)&TempFFT[0],
                              (fftw_complex *)&TempFFT[0], FFTW_BACKWARD,
                              FFTW_ESTIMATE);
}

Magnitude_AngleModuleToIQModule::~Magnitude_AngleModuleToIQModule() {
  fftw_destroy_plan(plan_fft);
}

void Magnitude_AngleModuleToIQModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  csignal<double> tempAngle;
  if (!PopFromInput(tempAngle, 0))
    return;

  csignal<double> tempMagnitude;
  if (!PopFromInput(tempMagnitude, 1))
    return;

  // проверка размерности
  if (tempAngle.size() != TempFFT.size())
    SetPlan(tempAngle.size());
  Complex(tempMagnitude, tempAngle, TempFFT);

  fftw_execute(plan_fft);
  // для номировки ОБПФ
  double N = 1.0 / TempFFT.size();
  // double N = 1;
  for (size_t i = 0; i < TempFFT.size(); ++i) {
    // получаем реальную часть
    tempAngle[i] = N * TempFFT[i].real();
    // получаем мнимую часть
    tempMagnitude[i] = N * TempFFT[i].imag();
  }
  //отправка в порт 1 I-канал
  PushToOutput(std::move(tempAngle), 0);
  //отправка в порт 0 Q-канал
  PushToOutput(std::move(tempMagnitude), 1);
}

void Magnitude_AngleModuleToIQModule::SetPlan(size_t len) {
  TempFFT.reset(len);
  plan_fft = fftw_plan_dft_1d(TempFFT.size(), (fftw_complex *)&TempFFT[0],
                              (fftw_complex *)&TempFFT[0], FFTW_FORWARD,
                              FFTW_ESTIMATE);
}
