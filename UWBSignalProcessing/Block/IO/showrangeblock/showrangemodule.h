#ifndef SQUAREMODULE_H
#define SQUAREMODULE_H

#include "iomodule.h"
#include "csignal.h"

class ShowRangeModule : public IOModule<std::vector<csignal<double>>, csignal<double>>
{
public:

    ShowRangeModule();
    ~ShowRangeModule() override = default;

    void Operate() override;
    void SetRange(size_t range);

private:

    size_t m_range = 0; // количество углов в скане
};

#endif // SQUAREMODULE_H
