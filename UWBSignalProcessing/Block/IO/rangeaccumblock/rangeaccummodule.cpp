#include "rangeaccummodule.h"

RangeAccumModule::RangeAccumModule() :
    IOModule(static_cast<uint8_t>(1), static_cast<uint8_t>(1))
{
    ResetSignal();
    SetPlan();
}

RangeAccumModule::~RangeAccumModule()
{
    ClearPlan();
}

void RangeAccumModule::SetSetting(size_t countAcumm, size_t startRange, size_t range, double threshol)
{
    SetCountAccum(countAcumm);
    SetStartRange(startRange);
    SetRange(range);
    m_threshol = threshol;
    SetPlan();
}

void RangeAccumModule::SetTypeThreshol(int type)
{
    m_typeThreshol = static_cast<TypeThreshol>(type);
}

void RangeAccumModule::SetTypeWindow(int type)
{
    m_typeWindow = static_cast<TypeWindow>(type);
}

void RangeAccumModule::SetIsVARU(bool isVARU)
{
    m_isVARU = isVARU;
}

void RangeAccumModule::SetCountAccum(size_t countAcumm)
{
    m_countAcumm = countAcumm;
    ResetSignal();
}

void RangeAccumModule::SetStartRange(size_t startRange)
{
    m_startRange = startRange;
    ResetSignal();
}

void RangeAccumModule::SetRange(size_t range)
{
    m_range = range;
}

void RangeAccumModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    InBuffer signalComplex;

    if(!PopFromInput(signalComplex))
        return;

    // Если диапозон накопления больше размера сигнала то ничего не делаем
    if(((m_startRange + m_range) > signalComplex.first.size()))
        return;

    if(m_currentAccum < m_countAcumm)
    {
        // выполняем накопление
        for (size_t range = m_startRange; range < (m_startRange + m_range); ++range)
        {
            double real = signalComplex.second[range] * m_window[m_currentAccum];
            double im = signalComplex.first[range] * m_window[m_currentAccum];
            int index = range - m_startRange;
            m_accumSignal[index][m_currentAccum] = std::complex<double>(real, im);
        }

        m_currentAccum ++;
    }
    else
    {
        Execute();
        ResetSignal();
    }
}

void RangeAccumModule::StopEvent()
{
    ResetSignal();
    IOModule::StopEvent();
}

void RangeAccumModule::ResetSignal()
{
    m_currentAccum = 0;
}

void RangeAccumModule::Execute()
{
    std::vector<std::valarray<double>> data(m_range);

    if(m_isVARU)
        VARU();

    for (size_t i = 0; i < m_range; ++i)
    {
        // выполняем преобразование
        fftw_execute(plan_fft[i]);

        switch (m_typeThreshol) {
        case TypeThreshol::Non:
            NonThreshol(data, i);
            break;

        case TypeThreshol::Bin:
            BinThreshol(data, i);
            break;

        case TypeThreshol::Indicating:
            IndicatingThreshol(data, i);
            break;

        case TypeThreshol::IndicatingAdapte:
            IndicatingAdapteThreshol(data, i);
            break;
        }
    }
    // отправка данных
    PushToOutput(std::move(data));
}

void RangeAccumModule::CreateWindow()
{    
    Window::GetWindow(m_window, m_countAcumm, m_typeWindow);
}

void RangeAccumModule::VARU()
{    
    for (size_t range = 0; range < plan_fft.size(); ++range) {
        double varu = (std::pow((double) range, 2) / std::pow((double)plan_fft.size(), 2));
        m_accumSignal[range] *= std::complex<double>(varu, 0);
    }
}

double RangeAccumModule::GetABSVal(size_t i, size_t j) const
{
    if(m_gain != 1.0)
        return std::abs(m_accumSignal[i][j]) * m_gain;

    return std::abs(m_accumSignal[i][j]);
}

void RangeAccumModule::NonThreshol(std::vector<std::valarray<double>>& data, size_t i)
{
    size_t indexData = m_range - i - 1;
    data[indexData].resize(m_countAcumm);
    // выполняем вычисление модуля
    size_t indexSpeed = 0;
    size_t median = m_countAcumm / 2;
    for (size_t j = 0; j < m_countAcumm; ++j)
    {
        indexSpeed = (median + j) % m_countAcumm;
        data[indexData][indexSpeed] = GetABSVal(i, j);
    }
}

void RangeAccumModule::BinThreshol(std::vector<std::valarray<double>>& data, size_t i)
{
    size_t indexData = m_range - i - 1;
    data[indexData].resize(m_countAcumm);
    // выполняем вычисление модуля
    size_t indexSpeed = 0;
    size_t median = m_countAcumm / 2;
    for (size_t j = 0; j < m_countAcumm; ++j)
    {
        indexSpeed = (median + j) % m_countAcumm;

        double val = GetABSVal(i, j);

        if(val < m_threshol)
            val = 0;
        else
            val = 2048;

        data[indexData][indexSpeed] = val;
    }

}

void RangeAccumModule::IndicatingThreshol(std::vector<std::valarray<double> >& data, size_t i)
{
    size_t indexData = m_range - i - 1;
    data[indexData].resize(m_countAcumm);
    // выполняем вычисление модуля
    size_t indexSpeed = 0;
    size_t median = m_countAcumm/2;
    for (size_t j = 0; j < m_countAcumm; ++j)
    {
        if(j < median)
            indexSpeed = median + j;
        else
            indexSpeed = j - median;

        double val = GetABSVal(i, j);

        if(val < m_threshol)
            val = 0;

        data[indexData][indexSpeed] = val;
    }
}

void RangeAccumModule::IndicatingAdapteThreshol(std::vector<std::valarray<double>>& data, size_t i)
{
    size_t indexData = m_range - i - 1;
    data[indexData].resize(m_countAcumm);
    // выполняем вычисление модуля
    size_t indexSpeed = 0;
    size_t median = m_countAcumm / 2;

    for (size_t j = 0; j < median; ++j)
    {
        double val = GetABSVal(i, j);
        if(val >= m_threshol)
        {
            double val2 = GetABSVal(i, m_countAcumm - 1 - j);

            if(val2 >= m_threshol)
            {
                indexSpeed = median + j;

                if(val < val2)
                    indexSpeed = (m_countAcumm - 1 - indexSpeed);

                data[indexData][indexSpeed] = val + val2;
            }
        }

    }

}

void RangeAccumModule::SetGain(double gain)
{
    m_gain = gain;
}

void RangeAccumModule::ClearPlan()
{
    for (auto& p : plan_fft)
        fftw_destroy_plan(p);
}

void RangeAccumModule::SetPlan()
{
    CreateWindow();

    if(m_accumSignal.size() == m_range && m_accumSignal.front().size() == m_countAcumm)
        return;
    // Количество накоплений
    m_accumSignal.resize(m_range);
    plan_fft.resize(m_range);

    // Диапазон перобразований
    for (size_t i = 0; i < m_range; ++i)
    {
        // создаем план
        m_accumSignal[i].resize(m_countAcumm);
        plan_fft[i] = fftw_plan_dft_1d(static_cast<int>(m_countAcumm), (fftw_complex*) &m_accumSignal[i][0],
                (fftw_complex*) &m_accumSignal[i][0], FFTW_FORWARD, FFTW_ESTIMATE);
    }
}
