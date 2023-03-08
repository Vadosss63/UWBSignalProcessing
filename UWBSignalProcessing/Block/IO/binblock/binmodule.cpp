#include "binmodule.h"

BinModule::BinModule():IOModule(1, 1){}

void BinModule::SetSettings(int shiftI, int shiftQ, int rangeUp, int rangeBottom)
{
    m_shiftI = shiftI;
    m_shiftQ = shiftQ;
    m_rangeUp = rangeUp;
    m_rangeBottom = rangeBottom;
}

void BinModule::Operate()
{    
    buffer temp;
    if(!PopFromInput(temp))
        return;
    double valI = 0;
    double valQ = 0;
    for (size_t i = 0; i < temp.first.size(); ++i) {

        valI = temp.first[i];
        valQ = temp.second[i];

        valI = valI + m_shiftI;
        valQ = valQ + m_shiftQ;

        if(valI > m_rangeUp)
            valI = m_rangeUp;

        if(valQ > m_rangeUp)
            valQ = m_rangeUp;

        if(valI <= m_rangeBottom)
            valI = m_rangeBottom;

        if(valQ <= m_rangeBottom)
            valQ = m_rangeBottom;

        temp.first[i] = valI;
        temp.second[i] = valQ;
    }

    PushToOutput(std::move(temp));
}



