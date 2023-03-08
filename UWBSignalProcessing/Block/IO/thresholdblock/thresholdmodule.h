#ifndef MODMODULE_H
#define MODMODULE_H

#include <vector>
#include "iomodule.h"
#include "csignal.h"

class ThresholdModule : public IOModule < csignal<double>,  csignal<double>>
{

public:

    ThresholdModule();
    ~ThresholdModule() override = default;
    void Operate() override;

};
#endif // MODMODULE_H
