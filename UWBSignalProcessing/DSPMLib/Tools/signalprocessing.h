#ifndef SIGNALPROCESSING_H
#define SIGNALPROCESSING_H

#include <complex>
#include <vector>
#include <algorithm>
#include <fftw3.h>

std::vector<std::complex<double>> time_to_freq(const std::vector<double>&);
std::vector<double> freq_to_time(const std::vector<std::complex<double> >&);
std::vector<std::complex<double>> conj(const std::vector<std::complex<double> > &);
std::vector<double> conv(const std::vector<double>&,const std::vector<double>&);

#endif // SIGNALPROCESSING_H
