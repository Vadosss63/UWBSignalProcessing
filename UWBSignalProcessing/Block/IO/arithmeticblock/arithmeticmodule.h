#ifndef ARITHMETICMODULE_H
#define ARITHMETICMODULE_H

#include <map>
#include "iomodule.h"
#include "arithmeticstate.h"

class ArithmeticModule: public IOModule<csignal<double>, csignal<double>>
{

public:

    ArithmeticModule();
    ~ArithmeticModule() override;

    void SetMathValue(const std::string& math_value);
    void Operate() override;

private:
    void Init();

    std::map<std::string, ArithmeticState* > m_mapState;
    ArithmeticState* m_mathVal = nullptr;
};

#endif // ARITHMETICMODULE_H
