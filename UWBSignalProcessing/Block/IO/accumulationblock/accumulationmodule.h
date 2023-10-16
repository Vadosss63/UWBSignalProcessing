#ifndef ACCUMULATIONMODULE_H
#define ACCUMULATIONMODULE_H

#undef emit

#include <algorithm>
#include <numeric>

#include <boost/circular_buffer.hpp>
#include <tbb/concurrent_queue.h>
#include "iomodule.h"
#include "csignal.h"

enum TypeAccumulation
{
    constAccumulationType = 0,
    constAndSendIterationAccumulationType,
    cyclicAccumulationType
};

class AbstractAccumulation;

class AccumulationModule : public IOModule<csignal<double>, csignal<double>>
{

public:
    AccumulationModule();
    ~AccumulationModule() override = default;

    void Operate() override;

    void StopEvent() override;

    void SetSizeBuf(size_t sizeBuf, TypeAccumulation type);

private:    
    AbstractAccumulation * m_abstractAccumulation ;
    size_t m_frame = 0;
};

class AbstractAccumulation
{

public:
    AbstractAccumulation(size_t sizeBuf = 1024);
    virtual ~AbstractAccumulation() = default;
    virtual void buffer(csignal<double>& signal);
    //  возвращает true если сигнал накоплен
    virtual bool Accumulation(csignal<double> &temp) = 0;
    // производит сбрасывание буфера в нулевое состояния
    virtual void clearBuf();
    // установка количества итераций
    void SetSizeBuf(size_t sizeBuf);

protected:
    // доступ к итерации
    size_t nIteration() const;
    // на полноту буфера
    bool IsTrueBuffer() const;
    void PushSignal(csignal<double>& signal);
    // проверка на пустоту буфера
    bool BufEmpty() const;
    // получение текущего азимута накопления
    uint32_t azimuth() const;
    // выполняет вычисление суммы
    void RezultSum();
private:
    // буфер для накопления сигнала
    boost::circular_buffer <csignal<double>> m_buffer;
    // значение текущего азимута
    uint32_t m_azimuth = 0;
    // буфер для накопления
    csignal <double> m_sum;
    // размер буфера накопления
    size_t m_sizeBuf;
};

// Накопления происходит до n-го
// принятого сигнала. В последствии чего сигнал усредняется и выталкивается
class ConstAccumulation : public AbstractAccumulation
{
public:
    // конструктор принимает параметр nIteration - число итераций прежде чем выталкнуть в порт
    ConstAccumulation(size_t sizeBuf);
    ~ConstAccumulation() override = default;
    //  возвращает true если сигнал накоплен
    bool Accumulation(csignal<double> &temp) override;
};

// Накопления происходит до n -го
// принятого сигнала. На каждой итерации происходит выталкивания сигнала
// с предворительным усреднением на итерацию
class ConstAndSendIterationAccumulation : public AbstractAccumulation /// надо доделать!!!
{

public:
    ConstAndSendIterationAccumulation(size_t sizeBuf);
    ~ConstAndSendIterationAccumulation() override = default;
    //  возвращает true если сигнал накоплен
    bool Accumulation(csignal<double> &temp) override;
};

// Накопления происходит до n -го
// принятого сигнала. После чего продалжается циклически(один сигнал удаляется,
// а на место его записывается другой). Накаждой итерации происходит выталкивание сигнала
// с усреднением на итерацию (до n-й итерации) и с усреднением на n (после n-й итерации)
class CyclicAccumulation : public AbstractAccumulation /// надо доделать!!!
{
public:
    // конструктор принимает параметр nIteration - число итераций прежде чем выталкнуть в порт
    CyclicAccumulation(size_t sizeBuf);
    ~CyclicAccumulation() override = default;
    //  возвращает true если сигнал накоплен
    bool Accumulation(csignal<double> &temp) override;
};

#endif // ACCUMULATIONMODULE_H
