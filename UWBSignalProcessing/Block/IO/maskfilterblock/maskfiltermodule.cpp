#include "maskfiltermodule.h"

MaskFilterModule::MaskFilterModule()
    : IOModule(static_cast<uint8_t>(1), static_cast<uint8_t>(1)),
      m_tempFFT(8192), m_sigTemp(8192), m_mask(8192, 1),
      m_expectedMutex(false) {
  //Создание плана для прямого преобразования Фурье
  m_plan_fft_forward =
      fftw_plan_dft_r2c_1d(m_tempFFT.size(), (double *)&m_sigTemp[0],
                           (fftw_complex *)&m_tempFFT[0], FFTW_ESTIMATE);
  //Создание плана для обратноог преобразования Фурье
  m_plan_fft_backward =
      fftw_plan_dft_c2r_1d(m_tempFFT.size(), (fftw_complex *)&m_tempFFT[0],
                           (double *)&m_sigTemp[0], FFTW_ESTIMATE);
}

MaskFilterModule::~MaskFilterModule() {
  fftw_destroy_plan(m_plan_fft_backward);
  fftw_destroy_plan(m_plan_fft_forward);
}

void MaskFilterModule::Operate() {
  csignal<double> temp;
  if (!PopFromInput(temp))
    return;
  bool flag = false;
  // выполняем проверку есть ли кто в функции
  if (m_expectedMutex.compare_exchange_strong(flag, true)) {
    if (temp.size() == 8192) {
      std::copy(temp.begin(), temp.end(), m_sigTemp.begin());
      fftw_execute(m_plan_fft_forward);

      std::transform(m_tempFFT.begin(), m_tempFFT.end(), m_mask.begin(),
                     m_tempFFT.begin(),
                     [](std::complex<double> &tempfft, double mask) {
                       return tempfft * mask;
                     });

      fftw_execute(m_plan_fft_backward);

      double N = 1.0 / m_sigTemp.size();
      std::transform(m_sigTemp.begin(), m_sigTemp.end(), temp.begin(),
                     [&N](double x) { return x * N; });
    }

    PushToOutput(std::move(temp));
    m_expectedMutex.store(false);
    return;
  }
}

void MaskFilterModule::SetMask(const std::vector<double> &coefficients) {
  if (m_mask.size() == coefficients.size())
    std::copy(coefficients.begin(), coefficients.end(), m_mask.begin());
}
