#ifndef MASKFILTERMODULE_H
#define MASKFILTERMODULE_H

#include <atomic>
#include <fftw3.h>
#include "iomodule.h"
#include "csignal.h"

class MaskFilterModule: public IOModule< csignal<double>,  csignal<double>>
{

public:

    MaskFilterModule();
    ~MaskFilterModule() override;

    void Operate() override;
    // установка коэффециентов для фильтра
    void SetMask(const std::vector<double>& coefficients);

private:

    fftw_plan m_plan_fft_forward;
    fftw_plan m_plan_fft_backward;
    csignal<std::complex<double>> m_tempFFT;
    csignal<double> m_sigTemp;
    // коэффециенты фильтра
    csignal<double> m_mask;
    //ожидаемый номер кадра для поддержания последовательности
    std::atomic_bool m_expectedMutex;
};


#endif // MASKFILTERMODULE_H
