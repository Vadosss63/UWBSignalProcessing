#ifndef CONVOLUTIONMODULE_H
#define CONVOLUTIONMODULE_H
#include "csignal.h"
#include <fftw3.h>
#include <vector>

namespace dspm {

class ConvolutionModule {

public:
  ConvolutionModule();
  ConvolutionModule(const int &fft_length);
  ConvolutionModule(const ConvolutionModule &rhs) = delete;
  ~ConvolutionModule();
  ConvolutionModule &operator=(const ConvolutionModule &rhs) = delete;

  void LetConv(csignal<double> &in, const std::vector<double> &filter);
  void LetConv(csignal<double> &in1, csignal<double> &in2,
               const std::vector<double> &filter);
  void LetConv(std::vector<double> &in, const std::vector<double> &filter);
  dspm::csignal<double> LetSimpConv(csignal<double> &in,
                                    const std::vector<double> &filter);

  void ChangeLength(const int &n_length);

  int GetFFTLength() const { return m_fft_length; }

private:
  //План БПФ
  fftw_plan m_plan_fft_forward_signal;
  fftw_plan m_plan_fft_forward_filter;

  fftw_plan m_plan_fft_backward;

  std::vector<std::complex<double>> m_tempFFT;
  std::vector<std::complex<double>> m_tempFFT_filter;

  std::vector<double> m_sigTemp;
  std::vector<double> m_filterTemp;

  int m_fft_length;
};
} // namespace dspm
#endif // CONVOLUTIONMODULE_H
