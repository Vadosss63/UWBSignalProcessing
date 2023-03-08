#ifndef BINMODULE_H
#define BINMODULE_H

#include "iomodule.h"
#include "csignal.h"

using buffer = std::pair<csignal<double>, csignal<double>>;
class BinModule : public IOModule<buffer,  buffer>
{

public:
    BinModule();
    ~BinModule() override = default;

    void SetSettings(int shiftI, int shiftQ, int rangeUp, int rangeBottom);

    void Operate() override;

private:
    int m_shiftI = 0;
    int m_shiftQ = 0;
    int m_rangeUp = 0;
    int m_rangeBottom = 0;
};

#endif // BINMODULE_H
