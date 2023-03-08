#ifndef CONTROLANTENNSMODULE_H
#define CONTROLANTENNSMODULE_H

#include <algorithm>
#include "iomodule.h"
#include "csignal.h"


using SigBuffer = std::pair<csignal<double>, csignal<double>>;

class ControlAntennsModule : public IOModule<SigBuffer, SigBuffer>
{

public:
    ControlAntennsModule();    
    ~ControlAntennsModule() override = default;

    void Operate() override;

};

#endif // CONTROLANTENNSMODULE_H
