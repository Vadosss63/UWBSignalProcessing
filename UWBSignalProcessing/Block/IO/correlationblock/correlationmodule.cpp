#include "correlationmodule.h"

Correlationmodule::Correlationmodule():IOModule<csignal<double>, csignal<double>>(2, 1)
{
    GeneratePlan(2048);
}

Correlationmodule::~Correlationmodule()
{
    fftw_destroy_plan(m_plan_fft_backwardSeq1);
    fftw_destroy_plan(m_plan_fft_forwardSeq1);
    fftw_destroy_plan(m_plan_fft_forwardSeq2);
}

void Correlationmodule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    csignal<double> temp;
    if (!PopFromInput(temp, 0))
        return;

    csignal<double> temp2;

    if(!PopFromInput(temp2, 1))
        return;

    size_t size = temp.size() > temp2.size() ? temp.size() * 2 - 1 : temp2.size() * 2 - 1;

    if(size != m_Seq1.size())
        GeneratePlan(size);

    std::copy(temp.begin(), temp.end(), m_Seq1.begin());
    std::copy(temp2.begin(), temp2.end(), m_Seq2.begin() + (temp2.size()-1));
    std::fill(m_Seq1.begin() + temp.size(), m_Seq1.end(), 0);
    std::fill(m_Seq2.begin(), m_Seq2.begin() + temp2.size(), 0);


    fftw_execute(m_plan_fft_forwardSeq1);

    fftw_execute(m_plan_fft_forwardSeq2);

    std::transform(m_res1FFT.begin(), m_res1FFT.end(), m_res2FFT.begin(), m_res1FFT.begin(),
                   [](std::complex<double>& res1, std::complex<double>& res2)
    {
        return res1 * std::conj(res2);
    });

    fftw_execute(m_plan_fft_backwardSeq1);
    temp.reset(size);

    std::transform(m_Seq1.begin(), m_Seq1.end(), temp.begin(),
                   [&size](double res1)
    {
        return res1 / size;
    });
    PushToOutput(std::move(temp));
}

void Correlationmodule::GeneratePlan(size_t size)
{
    if(!m_Seq1.empty())
    {
        fftw_destroy_plan(m_plan_fft_backwardSeq1);
        fftw_destroy_plan(m_plan_fft_forwardSeq1);
        fftw_destroy_plan(m_plan_fft_forwardSeq2);
    }
    m_Seq1.reset(size);
    m_Seq2.reset(size);
    m_res1FFT.reset(size);
    m_res2FFT.reset(size);

    //Создание плана для прямого преобразования Фурье
    m_plan_fft_forwardSeq1 = fftw_plan_dft_r2c_1d(m_Seq1.size(), (double*) &m_Seq1[0], (fftw_complex*) &m_res1FFT[0], FFTW_ESTIMATE );
    //Создание плана для прямого преобразования Фурье
    m_plan_fft_forwardSeq2 = fftw_plan_dft_r2c_1d(m_Seq2.size(), (double*) &m_Seq2[0], (fftw_complex*) &m_res2FFT[0], FFTW_ESTIMATE );
    //Создание плана для обратного преобразования Фурье
    m_plan_fft_backwardSeq1 = fftw_plan_dft_c2r_1d (m_res1FFT.size(), (fftw_complex*) &m_res1FFT[0], (double*) &m_Seq1[0], FFTW_ESTIMATE );
}
