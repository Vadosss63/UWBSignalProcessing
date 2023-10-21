#include "convolutionmodule.h"
#include "qdebug.h"

dspm::ConvolutionModule::ConvolutionModule() {}

dspm::ConvolutionModule::ConvolutionModule(const int &fft_length)
    : m_tempFFT(fft_length), m_tempFFT_filter(fft_length),
      m_sigTemp(fft_length), m_filterTemp(fft_length),
      m_fft_length(fft_length) {

  //Создание плана для прямого преобразования Фурье сигнала
  m_plan_fft_forward_signal =
      fftw_plan_dft_r2c_1d(m_tempFFT.size(), (double *)&m_sigTemp[0],
                           (fftw_complex *)&m_tempFFT[0], FFTW_ESTIMATE);
  //Создание плана для прямого преобразования Фурье фильтра
  m_plan_fft_forward_filter =
      fftw_plan_dft_r2c_1d(m_tempFFT_filter.size(), (double *)&m_filterTemp[0],
                           (fftw_complex *)&m_tempFFT_filter[0], FFTW_ESTIMATE);
  //Создание плана для обратноог преобразования Фурье
  m_plan_fft_backward =
      fftw_plan_dft_c2r_1d(m_tempFFT.size(), (fftw_complex *)&m_tempFFT[0],
                           (double *)&m_sigTemp[0], FFTW_ESTIMATE);
}

dspm::ConvolutionModule::~ConvolutionModule() {
  fftw_destroy_plan(m_plan_fft_backward);
  fftw_destroy_plan(m_plan_fft_forward_signal);
  fftw_destroy_plan(m_plan_fft_forward_filter);
}

void dspm::ConvolutionModule::ChangeLength(const int &n_length) {
  m_fft_length = n_length;

  m_tempFFT.resize(n_length);
  m_tempFFT_filter.resize(n_length);
  m_sigTemp.resize(n_length);
  m_filterTemp.resize(n_length);

  std::fill(m_sigTemp.begin(), m_sigTemp.end(), 0);
  std::fill(m_filterTemp.begin(), m_filterTemp.end(), 0);

  //    //Удаление старых планов
  //    fftw_destroy_plan(m_plan_fft_backward);
  //    fftw_destroy_plan(m_plan_fft_forward_signal);
  //    fftw_destroy_plan(m_plan_fft_forward_filter);

  //Создание новых

  //Создание плана для прямого преобразования Фурье сигнала
  m_plan_fft_forward_signal =
      fftw_plan_dft_r2c_1d(m_tempFFT.size(), (double *)&m_sigTemp[0],
                           (fftw_complex *)&m_tempFFT[0], FFTW_ESTIMATE);
  //Создание плана для прямого преобразования Фурье фильтра
  m_plan_fft_forward_filter =
      fftw_plan_dft_r2c_1d(m_tempFFT_filter.size(), (double *)&m_filterTemp[0],
                           (fftw_complex *)&m_tempFFT_filter[0], FFTW_ESTIMATE);
  //Создание плана для обратноог преобразования Фурье
  m_plan_fft_backward =
      fftw_plan_dft_c2r_1d(m_tempFFT.size(), (fftw_complex *)&m_tempFFT[0],
                           (double *)&m_sigTemp[0], FFTW_ESTIMATE);
}

void dspm::ConvolutionModule::LetConv(csignal<double> &in,
                                      const std::vector<double> &filter) {
  auto start_time = std::chrono::steady_clock::now();
  std::vector<double> temp;

  temp = filter;

  std::copy(in.begin(), in.end(), m_sigTemp.begin());
  std::copy(temp.begin(), temp.end(), m_filterTemp.begin());

  fftw_execute(m_plan_fft_forward_signal);
  fftw_execute(m_plan_fft_forward_filter);

  std::transform(m_tempFFT.begin(), m_tempFFT.end(), m_tempFFT_filter.begin(),
                 m_tempFFT.begin(),
                 [](std::complex<double> &tempfft, std::complex<double> &mask) {
                   return (tempfft = (tempfft) * (mask));
                 });

  fftw_execute(m_plan_fft_backward);

  double N = 1.0 / m_sigTemp.size();
  std::transform(m_sigTemp.begin(), m_sigTemp.end(), in.begin(),
                 [&N](double x) { return x * N; });

  auto end_time = std::chrono::steady_clock::now();

  auto res_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
      end_time - start_time);
  qDebug() << res_time.count();
}

void dspm::ConvolutionModule::LetConv(csignal<double> &in1,
                                      csignal<double> &in2,
                                      const std::vector<double> &filter) {
  std::vector<double> temp;

  temp = filter;

  //Фильтрация первого сигнала
  std::copy(in1.begin(), in1.end(), m_sigTemp.begin());
  std::copy(temp.begin(), temp.end(), m_filterTemp.begin());

  fftw_execute(m_plan_fft_forward_signal);
  fftw_execute(m_plan_fft_forward_filter);

  std::transform(m_tempFFT.begin(), m_tempFFT.end(), m_tempFFT_filter.begin(),
                 m_tempFFT.begin(),
                 [](std::complex<double> &tempfft, std::complex<double> &mask) {
                   return (tempfft = (tempfft) * (mask));
                 });

  fftw_execute(m_plan_fft_backward);

  double N = 1.0 / m_sigTemp.size();
  std::transform(m_sigTemp.begin(), m_sigTemp.end(), in1.begin(),
                 [&N](double x) { return x * N; });

  //Фильтрация второго сигнала
  std::copy(in2.begin(), in2.end(), m_sigTemp.begin());
  fftw_execute(m_plan_fft_forward_signal);

  std::transform(m_tempFFT.begin(), m_tempFFT.end(), m_tempFFT_filter.begin(),
                 m_tempFFT.begin(),
                 [](std::complex<double> &tempfft, std::complex<double> &mask) {
                   return (tempfft = (tempfft) * (mask));
                 });

  fftw_execute(m_plan_fft_backward);

  // N = 1.0/m_sigTemp.size();
  std::transform(m_sigTemp.begin(), m_sigTemp.end(), in2.begin(),
                 [&N](double x) { return x * N; });
}

void dspm::ConvolutionModule::LetConv(std::vector<double> &in,
                                      const std::vector<double> &filter) {
  std::vector<double> temp;
  temp = filter;
  in.resize(m_sigTemp.size());
  std::fill(m_sigTemp.begin(), m_sigTemp.end(), 0);
  std::fill(m_filterTemp.begin(), m_filterTemp.end(), 0);

  std::copy(in.begin(), in.end(), m_sigTemp.begin());
  std::copy(temp.begin(), temp.end(), m_filterTemp.begin());

  fftw_execute(m_plan_fft_forward_signal);
  fftw_execute(m_plan_fft_forward_filter);

  std::transform(m_tempFFT.begin(), m_tempFFT.end(), m_tempFFT_filter.begin(),
                 m_tempFFT.begin(),
                 [](std::complex<double> &tempfft, std::complex<double> &mask) {
                   return (tempfft = (tempfft) * (mask));
                 });

  fftw_execute(m_plan_fft_backward);

  double N = 1.0 / m_sigTemp.size();
  std::transform(m_sigTemp.begin(), m_sigTemp.end(), in.begin(),
                 [&N](double x) { return x * N; });

  //Компенсация сдвига фазы
}

dspm::csignal<double>
dspm::ConvolutionModule::LetSimpConv(csignal<double> &in,
                                     const std::vector<double> &filter) {
  auto start_time = std::chrono::steady_clock::now();
  csignal<double> out(in.size() + filter.size());
  out.set_service_parameters(in.azimuth(), in.frame_number());

  for (int j = (int)filter.size() - 1; j < (int)in.size(); ++j) {
    for (int i = 0; i < (int)filter.size(); ++i) {

      out[j] += in[j - i] * filter[i];
    }
  }

  auto end_time = std::chrono::steady_clock::now();

  auto res_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
      end_time - start_time);
  qDebug() << res_time.count();

  //    for (int i = 0; i < in.size(); ++i)
  //       {
  //           for (int j = 0; j < filter.size(); ++j)
  //           {

  //               out[j+i] += in[i]*(filter[j]);
  //           }

  //       }

  ////    std::copy(out.begin(), out.end(), in.begin());

  return (std::move(out));
}
