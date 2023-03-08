#ifndef D3MODULE_H
#define D3MODULE_H

#include <algorithm>
#include <iostream>
#include "sinkmodule.h"
#include "csignal.h"
#include "interface.h"
using inBuffer = std::vector<std::vector<double>>;
class D3Module: public SinkModule<inBuffer>
{
public:

    D3Module(UpdateData* update);
    ~D3Module() override = default;

    void Operate() override;

private:
    UpdateData* m_update;
    size_t m_countData = 256;
    size_t m_currentData = 0;
};

#endif // D3MODULE_H
