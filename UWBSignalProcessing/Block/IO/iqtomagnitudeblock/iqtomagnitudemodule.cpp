#include "iqtomagnitudemodule.h"

IQtoMagnitude_AngleModule::IQtoMagnitude_AngleModule()
    : IOModule(static_cast<uint8_t>(2), static_cast<uint8_t>(2)),
      TempFFT(2048) {
  plan_fft = fftw_plan_dft_1d(TempFFT.size(), (fftw_complex *)&TempFFT[0],
                              (fftw_complex *)&TempFFT[0], FFTW_FORWARD,
                              FFTW_ESTIMATE);
}

IQtoMagnitude_AngleModule::~IQtoMagnitude_AngleModule() {
  fftw_destroy_plan(plan_fft);
}

void IQtoMagnitude_AngleModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  csignal<double> tempI;
  if (!PopFromInput(tempI, 0))
    return;

  csignal<double> tempQ;
  if (!PopFromInput(tempQ, 1))
    return;

  // проверка размерности
  if (tempI.size() != TempFFT.size())
    SetPlan(tempI.size());

  std::transform(tempI.begin(), tempI.end(), tempQ.begin(), TempFFT.begin(),
                 [](double I, double Q) { return std::complex<double>(I, Q); });

  TempFFT.setServiceParameters(tempI.azimuth(), tempI.frameNumber());

  fftw_execute(plan_fft);
  //отправка в порт 0 фазы
  PushToOutput(ArgComplex(TempFFT), 0);
  //отправка в порт 1 модуля амплитуды
  PushToOutput(AbsComplex(TempFFT), 1);
}

void IQtoMagnitude_AngleModule::SetPlan(size_t len) {
  TempFFT.reset(len);
  plan_fft = fftw_plan_dft_1d(TempFFT.size(), (fftw_complex *)&TempFFT[0],
                              (fftw_complex *)&TempFFT[0], FFTW_FORWARD,
                              FFTW_ESTIMATE);
}
