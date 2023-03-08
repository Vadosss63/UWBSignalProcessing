#ifndef ACCUMULATIONDUALMODULE_H
#define ACCUMULATIONDUALMODULE_H

#include <boost/circular_buffer.hpp>
#include <tbb/concurrent_queue.h>
#include "iomodule.h"
#include "csignal.h"

class AbstractAccumulationDual;

class AccumulationDualModule : public IOModule<csignal<double>, csignal<double>>
{

public:

    AccumulationDualModule();
    ~AccumulationDualModule() override = default;

    void Operate() override;
    void StopEvent() override;

    void SetSizeBuf(size_t sizeBuf, size_t sizeFA);

private:

    uint32_t azimuth() const;
    void buffer(csignal<double>& signal);
    void SetAzimuth(const uint32_t &az);

    size_t m_frame = 0;
    csignal<double> m_sumSignal;
    size_t m_sizeBuf  = 256; // число принимаемых сигналов
    size_t m_sizeFA = 2; // начало и конец
    size_t m_counterBuff = 0; // число принятых сигналов
    uint32_t m_azimuth = 0;
};

#endif // ACCUMULATIONDUALMODULE_H
