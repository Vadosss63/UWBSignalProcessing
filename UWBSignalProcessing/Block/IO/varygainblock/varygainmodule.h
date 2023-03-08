#ifndef GAINMODULE_H
#define GAINMODULE_H

#include "iomodule.h"
#include "csignal.h"

class VaryGainModule : public IOModule< csignal<double>,  csignal<double>>
{

public:

    VaryGainModule();
    ~VaryGainModule() override = default;

    void Operate() override;
    void SetIndexConst(size_t index);

private:    

    size_t m_index_R2 = 3;
    size_t m_index_const = 333;

};

#endif // GAINMODULE_H
