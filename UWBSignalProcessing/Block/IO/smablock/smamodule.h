#ifndef SMAMODULE_H
#define SMAMODULE_H

#include <iostream>
#include <iterator>
#include <algorithm>
#include <queue>
#include <vector>
#include "iomodule.h"
#include "csignal.h"

class SMAModule : public IOModule<csignal<double>, csignal<double>>
{

public:

    SMAModule();
    ~SMAModule() override = default;

    void Operate() override;
    void SetSizeWindow(size_t sizeWindow);

private:    

    size_t m_sizeWindow = 1;    // размер скользящиго окна

};

#endif // SMAMODULE_H
