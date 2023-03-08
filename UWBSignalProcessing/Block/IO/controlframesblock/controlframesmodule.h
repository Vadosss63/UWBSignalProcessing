#ifndef CONTROLFRAMES_H
#define CONTROLFRAMES_H

#include <algorithm>
#include <iostream>

#include "iomodule.h"
#include "csignal.h"

class ControlFramesModule : public IOModule<csignal<double>, csignal<double>>
{

public:
    ControlFramesModule();
    ~ControlFramesModule() override = default;

    void Operate() override;

private:

    unsigned short m_frame;
};

#endif // CONTROLFRAMES_H
