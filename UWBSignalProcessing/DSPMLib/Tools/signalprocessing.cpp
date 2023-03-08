#include "signalprocessing.h"


std::vector<std::complex<double> > time_to_freq(const std::vector<double>& sig)
{
    auto sig_size = sig.size();

    std::vector<double> tmp(sig_size);

    std::vector<std::complex<double>> spectrum_signal(sig_size);

    std::copy(sig.begin(),sig.end(),tmp.begin());

    fftw_plan m_plan_fft_forward_signal = fftw_plan_dft_r2c_1d(static_cast<int>(sig_size), reinterpret_cast<double*>(&tmp[0]), reinterpret_cast<fftw_complex*>(&spectrum_signal[0]), FFTW_ESTIMATE );

    fftw_execute(m_plan_fft_forward_signal);

    fftw_destroy_plan(m_plan_fft_forward_signal);

    return spectrum_signal;
}

std::vector<double> freq_to_time(const std::vector<std::complex<double> >& input_sig)
{
    auto sig_size = input_sig.size();

    std::vector<double> output_sig(sig_size);

    std::vector<double> tmp(sig_size);

    fftw_plan m_plan_fft_backward= fftw_plan_dft_c2r_1d (sig_size, (fftw_complex*) &input_sig[0], (double*) &tmp[0], FFTW_ESTIMATE );

    fftw_execute(m_plan_fft_backward);

    double N = 1.0/sig_size;

    std::for_each(tmp.begin(),tmp.end(),[&N](double& h)
    {
        h*=N;
    });

    fftw_destroy_plan(m_plan_fft_backward);

    std::copy(tmp.begin(),tmp.end(),output_sig.begin());

    return output_sig;
}

std::vector<std::complex<double> > conj(const std::vector<std::complex<double>>& sig)
{
    auto tmp(sig);

    std::for_each(tmp.begin(),tmp.end(), []
                  (std::complex<double>& h)
    {
        h = std::conj(h);;
    });

    return tmp;
}

std::vector<double> conv(const std::vector<double> &sig1, const std::vector<double> &sig2)
{
    size_t len = sig1.size() + sig2.size() - 1;

    std::vector<double> inFirst(len, 0);

    std::vector<double> inSecond(len, 0);

    std::copy(sig1.begin(), sig1.end(), inFirst.begin());

    std::copy(sig2.begin(), sig2.end(), inSecond.begin());

    auto spectr1 =  time_to_freq(inFirst);

    auto spectr2 = time_to_freq(inSecond);

    std::vector<std::complex<double>> spectr3(spectr1.size());

    std::transform(spectr1.begin(), spectr1.end(), spectr2.begin(), spectr3.begin(), []
                   (const std::complex<double>& s1, const std::complex<double>& s2)
    {
        return s1*s2;
    });

    auto outputSignal = freq_to_time(spectr3);

    return outputSignal;
}
