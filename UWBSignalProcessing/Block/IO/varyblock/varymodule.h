#ifndef VARYMODULE_H
#define VARYMODULE_H

#include "iomodule.h"
#include "csignal.h"

class VARYModule: public IOModule< csignal<double>,  csignal<double>>
{

public:

    VARYModule();
    ~VARYModule() override = default;

    void Operate() override;
    void SetVARYCoeff(const std::vector<double>& coefficients);

private:

    std::vector<double> m_VARYCoeff;
};

#endif // VARYMODULE_H
