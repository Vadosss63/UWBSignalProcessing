#ifndef MATHMODULE_H
#define MATHMODULE_H

#include <map>
#include "iomodule.h"
#include "mathstate.h"

class MathModule: public IOModule<csignal<double>, csignal<double>>
{

public:

    MathModule();
    ~MathModule() override;

    void SetMathValue(const std::string& math_value);
    void Operate() override;

private:

    void Init();

    std::map<std::string, StateMath* > m_mapState;
    StateMath* m_mathVal = nullptr; // выбранное пользователем состояние

};

#endif // MATHMODULE_H
