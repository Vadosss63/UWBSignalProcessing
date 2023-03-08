#include <iostream>
#include "testfiltermodule.h"


TestFilterModule::TestFilterModule() :
    IOModule(1, 1),
    m_filter(std::unique_ptr<BaseFilter>(new LowPassFilter(10, 0.004))){}

void TestFilterModule::Operate()
{
    csignal<double> temp1;

    if(!PopFromInput(temp1, 0))
        return;

    //Вариант Фильтра - 1 вход; 1 выход

    std::lock_guard<std::mutex> lock(operateLock);

    std::vector<double> input_sig(temp1.size());

    std::vector<double> output_sig(temp1.size());

    std::copy(temp1.begin(), temp1.end(), input_sig.begin());

    m_filter->FiltrationProcess(input_sig, output_sig);

    std::copy(output_sig.begin(), output_sig.end(), temp1.begin());

    //Вариант квадратурного фильтра

    if(CountInputPort() == 2)
    {
        csignal<double> temp2;

        if(!PopFromInput(temp2, 1))
            return;

        std::copy(temp2.begin(), temp2.end(), input_sig.begin());

        m_filter->FiltrationProcess(input_sig, output_sig);

        std::copy(output_sig.begin(), output_sig.end(), temp2.begin());

        PushToOutput(std::move(temp2),1);
    }

    PushToOutput(std::move(temp1), 0);
}

void TestFilterModule::SetFilterParameters(double SampleFreq, double CutFreq1, double CutFreq2, int order, FilterType type)
{
    auto freq_cut = 0.0;
    auto low_freq_cut = 0.0;
    auto hight_freq_cut = 0.0;

    switch(type)
    {
    case FilterType::Lowpass:
        freq_cut = 2*CutFreq1/SampleFreq;
        if (m_filter && (m_filter->GetFilterType() == FilterType::Lowpass))
        {
            m_filter->ChangeParams(order,freq_cut);
        }
        else
        {
            m_filter = std::unique_ptr<BaseFilter>(new LowPassFilter(order, freq_cut));
        }

        break;
    case FilterType::Hightpass:
        freq_cut = 2*CutFreq1/SampleFreq;

        if (m_filter && (m_filter->GetFilterType() == FilterType::Hightpass))
        {
            m_filter->ChangeParams(order,freq_cut);
        }
        else
        {
            m_filter = std::unique_ptr<BaseFilter>(new HightPassFilter(order,freq_cut));
        }
        break;

    case FilterType::Bandpass:
        low_freq_cut = 2*CutFreq1/SampleFreq;
        hight_freq_cut = 2*CutFreq2/SampleFreq;


        if (m_filter && (m_filter->GetFilterType() == FilterType::Bandpass))
        {
            m_filter->ChangeParams(order,low_freq_cut, hight_freq_cut);
        }
        else
        {
            m_filter = std::unique_ptr<BaseFilter>(new BandPassFilter(order,low_freq_cut, hight_freq_cut));
        }

        break;

    case FilterType::Bandstop:
        low_freq_cut = 2*CutFreq1/SampleFreq;
        hight_freq_cut = 2*CutFreq2/SampleFreq;

        if (m_filter && (m_filter->GetFilterType() == FilterType::Bandstop))
        {
            m_filter->ChangeParams(order,low_freq_cut, hight_freq_cut);
        }
        else
        {
            m_filter = std::unique_ptr<BaseFilter>(new BandStopFilter(order,low_freq_cut,hight_freq_cut));
        }


        break;
    case FilterType::Hilbert:

        if (m_filter && (m_filter->GetFilterType() == FilterType::Hilbert))
        {
            m_filter->ChangeParams();
        }
        else
        {
            m_filter = std::unique_ptr<BaseFilter>(new HilbertFilter());
        }

        break;
    }
    m_filter->ChangeFilter();
    //    emit DrawMagnitude();

}

TestFilterModule::BaseFilter::BaseFilter(size_t filterOrder)
    :m_filterOrder(filterOrder),
      m_b(filterOrder + 1),
      m_a(filterOrder + 1),
      m_spectrumBuilder(2048),
      m_signalBuilder(2048)
{
    m_a[0] = 1;
}

std::vector<double> TestFilterModule::BaseFilter::GetFilterCoeff() const
{
    return m_b;
}

void TestFilterModule::BaseFilter::FiltrationProcess(const std::vector<double> &inputSignal, std::vector<double> &outputSignal)
{
    const size_t sizeSignal = inputSignal.size();
    if(sizeSignal != m_sizeFFT)
    {
        m_sizeFFT = sizeSignal;
        ChangeFilter();
    }

    //Get spectrum of input signal
    auto sp1 = m_spectrumBuilder.BuildFFT(inputSignal);

    std::transform(sp1.begin(), sp1.end(), m_fil.begin(), m_sp3.begin(),
                   [](const std::complex<double>& s1, const std::complex<double>& s2)
    { return s1 * s2; });

    outputSignal = m_signalBuilder.BuildReverseFFT(m_sp3);
}

void TestFilterModule::BaseFilter::SimpleFiltrationProcess(const std::vector<double> &inputSignal, std::vector<double> &outputSignal)
{
    std::vector<double> tmp(inputSignal.size(),0);

    std::copy(m_b.begin(), m_b.end(),tmp.begin());

    auto sp3 = conv(inputSignal,tmp); //Convolution of input signals

    outputSignal.resize(inputSignal.size());

    std::copy(sp3.begin(),sp3.begin() + inputSignal.size(),outputSignal.begin());
}

void TestFilterModule::BaseFilter::ChangeFilter()
{
    if(!m_sizeFFT)
        return;

    std::vector<double> tmp(m_sizeFFT, 0);
    std::copy(m_b.begin(), m_b.end(), tmp.begin());
    //get spectrum of impulse response of our filter
    auto sp2 = m_spectrumBuilder.BuildFFT(tmp);
    //get conj of our filter response
    auto conj_sp2 = conj(sp2);
    m_fil.resize(m_sizeFFT);
    std::transform(sp2.begin(), sp2.end(), conj_sp2.begin(), m_fil.begin(),
                   [](const std::complex<double>& s1, const std::complex<double>& s2)
    { return s1 * s2; });

    m_sp3.resize(m_sizeFFT);
}

void TestFilterModule::BaseFilter::ReverseMagnitude()
{
    for(int index = 0; index < (int)m_b.size(); index++)
    {
        if(!(index % 2))
            m_b[index] = -m_b[index];
    }
}

TestFilterModule::LowPassFilter::LowPassFilter(size_t filterOrder, double freqCut)
    :BaseFilter(filterOrder),
      m_freqCut(freqCut)
{
    FilterInit();
}

TestFilterModule::FilterType TestFilterModule::LowPassFilter::GetFilterType()
{
    return FilterType::Lowpass;
}

void TestFilterModule::LowPassFilter::ChangeParams(size_t order, double freq_cut1, double)
{
    m_filterOrder = order;
    m_freqCut = freq_cut1;

    m_b.resize(m_filterOrder + 1);
    m_a.resize(m_filterOrder + 1);
    m_a[0] = 1;

    FilterInit();
}

void TestFilterModule::LowPassFilter::FilterInit()
{
    double index = 0;

    std::for_each(m_b.begin(), m_b.end(), [&index, this](double& impulseResponseCoefficient)
    {
        if(index == m_filterOrder / 2)
            impulseResponseCoefficient = m_freqCut;
        else
            impulseResponseCoefficient = m_freqCut*(sin((M_PI* (index - (m_filterOrder/2 ))) * m_freqCut)/((M_PI* (index - (m_filterOrder/2))) * m_freqCut));

        impulseResponseCoefficient = (0.54-0.46*cos(2*M_PI*index/m_filterOrder))*impulseResponseCoefficient;

        ++index;
    });
}

TestFilterModule::HightPassFilter::HightPassFilter(const size_t filterOrder, const double freqCut)
    :BaseFilter(filterOrder),
      m_freqCut(freqCut)
{
    FilterInit();
}

TestFilterModule::FilterType TestFilterModule::HightPassFilter::GetFilterType()
{
    return FilterType::Hightpass;
}

void TestFilterModule::HightPassFilter::ChangeParams(size_t order, double freq_cut1, double)
{
    m_filterOrder = order;
    m_freqCut = freq_cut1;
    m_b.resize(m_filterOrder + 1);
    m_a.resize(m_filterOrder + 1);
    m_a[0] = 1;
    FilterInit();
}

void TestFilterModule::HightPassFilter::FilterInit()
{
    LowPassFilter tmpLowFilter(m_filterOrder,1-m_freqCut);

    m_b = tmpLowFilter.GetFilterCoeff();

    ReverseMagnitude();
}

TestFilterModule::BandPassFilter::BandPassFilter(const size_t filterOrder, const double lowFreqCut, const double hightFreqCut)
    :BaseFilter(filterOrder),
      m_lowFreqCut(lowFreqCut),
      m_hightFreqCut(hightFreqCut)
{
    FilterInit();
}

TestFilterModule::FilterType TestFilterModule::BandPassFilter::GetFilterType()
{
    return FilterType::Bandpass;
}

void TestFilterModule::BandPassFilter::ChangeParams(size_t order, double freq_cut1, double freq_cut2)
{
    m_filterOrder = order;
    m_lowFreqCut = freq_cut1;
    m_hightFreqCut = freq_cut2;
    m_b.resize(m_filterOrder + 1);
    m_a.resize(m_filterOrder + 1);
    m_a[0] = 1;
    FilterInit();
}

void TestFilterModule::BandPassFilter::FilterInit()
{
    //Созданите ФВЧ

    HightPassFilter tmpHightFilter(m_filterOrder, m_lowFreqCut);

    std::vector<double> hightfilterCoeff = tmpHightFilter.GetFilterCoeff();

    //Создание ФНЧ
    LowPassFilter tmpLowFilter(m_filterOrder, m_hightFreqCut);

    std::vector<double> lowfilterCoeff = tmpLowFilter.GetFilterCoeff();

    //Свертка их
    std::vector<double> tmp1(lowfilterCoeff.size());
    std::vector<double> tmp2(lowfilterCoeff.size());

    std::copy(lowfilterCoeff.begin(), lowfilterCoeff.end(), tmp1.begin());
    std::copy(hightfilterCoeff.begin(), hightfilterCoeff.end(), tmp2.begin());

    auto res = conv(tmp1,tmp2);

    //Вырезание центральной части свертки
    std::copy(res.begin() + m_b.size()/2, res.begin() + m_b.size()/2 + m_b.size(), m_b.begin());
}

TestFilterModule::BandStopFilter::BandStopFilter(const size_t filterOrder, const double lowFreqCut, const double hightFreqCut)
    :   BaseFilter(filterOrder),
      m_lowFreqCut(lowFreqCut),
      m_hightFreqCut(hightFreqCut)
{
    FilterInit();
}

TestFilterModule::FilterType TestFilterModule::BandStopFilter::GetFilterType()
{
    return FilterType::Bandstop;
}

void TestFilterModule::BandStopFilter::ChangeParams(size_t order, double freq_cut1, double freq_cut2)
{
    m_filterOrder = order;
    m_lowFreqCut = freq_cut1;
    m_hightFreqCut = freq_cut2;
    m_b.resize(m_filterOrder + 1);
    m_a.resize(m_filterOrder + 1);
    m_a[0] = 1;
    FilterInit();
}

void TestFilterModule::BandStopFilter::FilterInit()
{
    //Создание ФНЧ
    LowPassFilter tmpLowFilter(m_filterOrder, m_lowFreqCut);

    std::vector<double> lowfilterCoeff = tmpLowFilter.GetFilterCoeff();

    //Созданите ФВЧ

    HightPassFilter tmpHightFilter(m_filterOrder, m_hightFreqCut);

    std::vector<double> hightfilterCoeff = tmpHightFilter.GetFilterCoeff();

    std::transform(lowfilterCoeff.begin(), lowfilterCoeff.end(), hightfilterCoeff.begin(), m_b.begin(),[](double& l, double& h)
    {
        return (l+h);
    });
}

TestFilterModule::HilbertFilter::HilbertFilter()
    : BaseFilter(){}

TestFilterModule::FilterType TestFilterModule::HilbertFilter::GetFilterType()
{
    return FilterType::Hilbert;
}

void TestFilterModule::HilbertFilter::FiltrationProcess(const std::vector<double> &inputSignal, std::vector<double> &outputSignal)
{

    ////
    /// TODO оптимизировать фильтрацию Гильберта
    ///
    std::vector<std::complex<double>> analyticSignal(inputSignal.size());

    std::copy(inputSignal.begin(),inputSignal.end(),analyticSignal.begin());

    std::vector<std::complex<double>> FFTOfAnalyticSignal(inputSignal.size());

    std::vector<std::complex<double>> frequencyMask(inputSignal.size());

    fftw_plan forwardFFTplan = fftw_plan_dft_1d(FFTOfAnalyticSignal.size(), (fftw_complex*) &analyticSignal[0], (fftw_complex*) &FFTOfAnalyticSignal[0],FFTW_FORWARD, FFTW_ESTIMATE );

    fftw_plan backwardFFTplan = fftw_plan_dft_1d(FFTOfAnalyticSignal.size(),(fftw_complex*) &FFTOfAnalyticSignal[0], (fftw_complex*) &analyticSignal[0], FFTW_BACKWARD ,FFTW_ESTIMATE );

    ///////////////////////

    if(inputSignal.size() % 2 == 0)
    {
        frequencyMask [0] = frequencyMask [inputSignal.size() / 2] = 1;

        for(size_t i = 1; i <= inputSignal.size()/2; i++)
        {
            frequencyMask[i] = 2;
        }
    }
    else
    {
        frequencyMask [0] = 1;
        for(size_t i = 1; i <= (inputSignal.size()+1)/2; i++)
        {
            frequencyMask[i] = 2;
        }
    }

    fftw_execute(forwardFFTplan);

    std::transform(FFTOfAnalyticSignal.begin(), FFTOfAnalyticSignal.end(), frequencyMask.begin(), FFTOfAnalyticSignal.begin(),
                   [](std::complex<double>& tempfft, std::complex<double>& mask)
    {
        return (tempfft=(tempfft)*(mask));
    });

    fftw_execute(backwardFFTplan);

    double normalizationCoeff = 1.0/analyticSignal.size();

    std::transform(analyticSignal.begin(), analyticSignal.end(), outputSignal.begin(),
                   [&normalizationCoeff](std::complex<double>& x)
    {
        return (x*normalizationCoeff).imag();
    });

    fftw_destroy_plan(backwardFFTplan);
    fftw_destroy_plan(forwardFFTplan);

}

void TestFilterModule::HilbertFilter::SimpleFiltrationProcess(const std::vector<double> &inputSignal, std::vector<double> &outputSignal)
{
    FiltrationProcess(inputSignal,outputSignal);
}

void TestFilterModule::HilbertFilter::FilterInit(){}
