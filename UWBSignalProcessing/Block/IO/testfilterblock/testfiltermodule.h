#ifndef TESTFILTERMODULE_H
#define TESTFILTERMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include "signalprocessing.h"
#include <atomic>
#include <memory>

class TestFilterModule : public IOModule<csignal<double>, csignal<double>> {
public:
  enum class FilterType : uint8_t {
    Lowpass = 0,
    Hightpass = 1,
    Bandpass = 2,
    Bandstop = 3,
    Hilbert = 4
  };

private:
  class FFTBuilder {
  public:
    FFTBuilder(const size_t fftLength)
        : m_fftLength(fftLength), m_spectr(m_fftLength),
          m_complexSignal(m_fftLength), m_plan(nullptr) {
      InitPlan();
    }

    FFTBuilder(const FFTBuilder &rhs)
        : m_fftLength(rhs.m_fftLength), m_spectr(rhs.m_spectr),
          m_complexSignal(rhs.m_complexSignal), m_plan(nullptr) {
      InitPlan();
    }

    ~FFTBuilder() {
      fftMutex.lock();

      if (m_plan)
        fftw_destroy_plan(m_plan);

      fftMutex.unlock();
    }

    std::vector<std::complex<double>>
    BuildFFT(const std::vector<double> &input) {
      std::lock_guard<std::mutex> lck(fftMutex);
      if (input.size() > m_fftLength) {
        ResizePlan(input.size());
      }

      std::vector<std::complex<double>> outSpectr(m_fftLength);

      std::copy(input.begin(), input.end(), m_complexSignal.begin());

      fftw_execute(m_plan);

      std::copy(m_spectr.begin(), m_spectr.end(), outSpectr.begin());

      return outSpectr;
    }

    size_t GetFFTLength() { return m_fftLength; }

    void ResizePlan(const size_t newSize) {
      m_fftLength = newSize;

      m_spectr.resize(newSize);

      m_complexSignal.resize(newSize);

      m_plan = fftw_plan_dft_r2c_1d(
          m_fftLength, reinterpret_cast<double *>(&m_complexSignal[0]),
          reinterpret_cast<fftw_complex *>(&m_spectr[0]), FFTW_ESTIMATE);
    }

  private:
    std::mutex fftMutex;

    size_t m_fftLength;

    std::vector<std::complex<double>> m_spectr;

    std::vector<double> m_complexSignal;

    fftw_plan m_plan;

    void InitPlan() {
      fftMutex.lock();

      if (!m_plan)
        m_plan = fftw_plan_dft_r2c_1d(
            m_fftLength, reinterpret_cast<double *>(&m_complexSignal[0]),
            reinterpret_cast<fftw_complex *>(&m_spectr[0]), FFTW_ESTIMATE);

      fftMutex.unlock();
    }
  };

  class ReverseFFTBuilder {
  public:
    ReverseFFTBuilder(size_t fftLength)
        : m_fftLength(fftLength), m_complexSignal(m_fftLength),
          m_spectr(m_fftLength), m_plan(nullptr) {
      InitPlan();
    }

    ~ReverseFFTBuilder() {
      fftMutex.lock();

      if (m_plan)
        fftw_destroy_plan(m_plan);

      fftMutex.unlock();
    }

    ReverseFFTBuilder(const ReverseFFTBuilder &rhs)
        : m_fftLength(rhs.m_fftLength), m_complexSignal(rhs.m_fftLength),
          m_spectr(rhs.m_fftLength), m_plan(nullptr) {
      InitPlan();
    }

    std::vector<double>
    BuildReverseFFT(const std::vector<std::complex<double>> &input) {
      std::lock_guard<std::mutex> lck(fftMutex);

      if (input.size() > m_fftLength) {
        ResizePlan(input.size());
      }

      std::vector<double> outSignal(input.size());

      std::copy(input.begin(), input.end(), m_spectr.begin());

      fftw_execute(m_plan);

      double N = 1.0 / m_fftLength;

      for (uint16_t i = 0; i < input.size(); i++)
        outSignal[i] = m_complexSignal[i] * N;

      return outSignal;
    }

    void ResizePlan(const int newSize) {
      m_fftLength = newSize;

      m_complexSignal.resize(newSize);

      m_spectr.resize(newSize);

      m_plan =
          fftw_plan_dft_c2r_1d(m_fftLength, (fftw_complex *)&m_spectr[0],
                               (double *)&m_complexSignal[0], FFTW_ESTIMATE);
    }

  private:
    std::mutex fftMutex;

    uint16_t m_fftLength;

    std::vector<double> m_complexSignal;

    std::vector<std::complex<double>> m_spectr;

    fftw_plan m_plan;

    void InitPlan() {
      fftMutex.lock();
      if (!m_plan)
        m_plan =
            fftw_plan_dft_c2r_1d(m_fftLength, (fftw_complex *)&m_spectr[0],
                                 (double *)&m_complexSignal[0], FFTW_ESTIMATE);

      fftMutex.unlock();
    }
  };

  //Базовый фильтр
  class BaseFilter {
  public:
    //Конструкторы/Деструкторы
    BaseFilter(size_t filterOrder = 0);

    BaseFilter(const BaseFilter &rhs) = delete;
    BaseFilter &operator=(const BaseFilter &rhs) = delete;
    virtual ~BaseFilter() = default;

    //Получение коэффициентов фильтра
    std::vector<double> GetFilterCoeff() const;

    //Выполнение фильтрации
    virtual void FiltrationProcess(const std::vector<double> &inputSignal,
                                   std::vector<double> &outputSignal);

    virtual void SimpleFiltrationProcess(const std::vector<double> &inputSignal,
                                         std::vector<double> &outputSignal);

    virtual FilterType GetFilterType() = 0;

    virtual void ChangeParams(size_t order = 0, double freq_cut1 = 0,
                              double freq_cut2 = 0) = 0;
    void ChangeFilter();

  protected:
    virtual void FilterInit() = 0;

    ///Внутренние переменные
    //Зеркальный поворот АЧХ по оси частот
    void ReverseMagnitude();

    //Порядок фильтра
    size_t m_filterOrder;

    //Коэффициенты фильтра
    std::vector<double> m_b;
    std::vector<double> m_a;
    // Прямое преобразование фурье
    FFTBuilder m_spectrumBuilder;
    // Обратное преобразование фурье
    ReverseFFTBuilder m_signalBuilder;

  private:
    // Спектральная характеристика фильтра
    std::vector<std::complex<double>> m_fil;
    // Спектральная характеристика фильтроваванного сигнала
    std::vector<std::complex<double>> m_sp3;

    size_t m_sizeFFT = 0;
  };

  //ФНЧ
  class LowPassFilter : public BaseFilter {
  public:
    LowPassFilter(size_t filterOrder, double freqCut);
    ~LowPassFilter() override = default;

    FilterType GetFilterType() override;

    void ChangeParams(size_t order, double freq_cut1, double) override;

  protected:
    void FilterInit() override;

  private:
    double m_freqCut;
  };

  //ФВЧ
  class HightPassFilter : public BaseFilter {
  public:
    HightPassFilter(const size_t filterOrder, const double freqCut);
    ~HightPassFilter() override = default;

    FilterType GetFilterType() override;

    void ChangeParams(size_t order, double freq_cut1, double) override;

  protected:
    void FilterInit() override;

  private:
    double m_freqCut;
  };

  //Полосовой фильтр
  class BandPassFilter : public BaseFilter {
  public:
    BandPassFilter(const size_t filterOrder, const double lowFreqCut,
                   const double hightFreqCut);
    ~BandPassFilter() override = default;

    FilterType GetFilterType() override;

    void ChangeParams(size_t order, double freq_cut1,
                      double freq_cut2) override;

  protected:
    void FilterInit() override;

  private:
    double m_lowFreqCut;
    double m_hightFreqCut;
  };

  //Режекторный фильтр
  class BandStopFilter : public BaseFilter {
  public:
    BandStopFilter(const size_t filterOrder, const double lowFreqCut,
                   const double hightFreqCut);
    ~BandStopFilter() override = default;

    FilterType GetFilterType() override;

    void ChangeParams(size_t order, double freq_cut1,
                      double freq_cut2) override;

  protected:
    void FilterInit() override;

  private:
    double m_lowFreqCut;
    double m_hightFreqCut;
  };

  //Фильтр Гильберта
  class HilbertFilter : public BaseFilter {
  public:
    HilbertFilter();
    ~HilbertFilter() override = default;
    FilterType GetFilterType() override;

    void FiltrationProcess(const std::vector<double> &inputSignal,
                           std::vector<double> &outputSignal) override;

    void SimpleFiltrationProcess(const std::vector<double> &inputSignal,
                                 std::vector<double> &outputSignal) override;

    void ChangeParams(size_t, double, double) override {}

  protected:
    void FilterInit() override;
  };

public:
  TestFilterModule();
  ~TestFilterModule() override = default;

  void Operate() override;
  void SetFilterParameters(double SampleFreq, double CutFreq1, double CutFreq2,
                           int order, FilterType type);

private:
  std::unique_ptr<BaseFilter> m_filter;
};

#endif // TESTFILTERMODULE_H
