#ifndef TRADEMODULE_H
#define TRADEMODULE_H

#include "iomodule.h"
#include "csignal.h"

class TradeModule : public IOModule< csignal<double>,  csignal<double>>
{

public:

    TradeModule();
    ~TradeModule() override = default;

    void Operate() override;
    void SetShiftBoard(std::vector<double> shiftConstantComponentBoard);

private:

    std::vector<double> m_shiftConstantComponentBoard;

};

#endif // TRADEMODULE_H
