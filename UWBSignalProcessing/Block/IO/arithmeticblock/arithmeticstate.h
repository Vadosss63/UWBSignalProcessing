#ifndef ARITHMETICSTATE_H
#define ARITHMETICSTATE_H

#include "csignal.h"

class ArithmeticState
{
public:
    virtual ~ArithmeticState() = default;
    virtual void execute(csignal<double>& sigA, csignal<double>& sigB) = 0;

};

class SumState: public ArithmeticState
{
public:
    void execute(csignal<double>& sigA, csignal<double>& sigB) override;
};

class DiffState: public ArithmeticState
{
public:
    void execute(csignal<double>& sigA, csignal<double>& sigB) override;
};

class MultiState: public ArithmeticState
{
public:
    void execute(csignal<double>& sigA, csignal<double>& sigB) override;
};

class DivState: public ArithmeticState
{
public:
    void execute(csignal<double>& sigA, csignal<double>& sigB) override;
};

#endif // ARITHMETICSTATE_H
