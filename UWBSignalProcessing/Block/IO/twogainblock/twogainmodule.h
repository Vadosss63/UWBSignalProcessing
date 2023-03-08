#ifndef TWOGAINMODULE_H
#define TWOGAINMODULE_H

#include "iomodule.h"
#include "csignal.h"

class TwoGainModule : public IOModule< csignal<double>,  csignal<double>>
{
public:

    TwoGainModule();
    ~TwoGainModule() override = default;

    void Operate() override;
    void StartEvent() override;
    void SetIndexOtherGainSignal(size_t indexOtherGainSignal);

private:

    bool CheckSizeSignal(size_t size);
    void ResetParameters();
    // сигнал для накопления
    csignal<double> m_firstSignal;
    // начальный индекс усиления
    size_t m_indexOtherGainSignal = 1;
    // признак что сигнал является первым
    bool m_isFirst = true;

};

#endif // TWOGAINMODULE
