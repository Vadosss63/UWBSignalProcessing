#include "controlantennsmodule.h"

ControlAntennsModule::ControlAntennsModule(): IOModule(1, 2){}

void ControlAntennsModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    SigBuffer tempSig;

    if (!PopFromInput(tempSig))
        return;

    PushToOutput(tempSig, tempSig.first.frameNumber() % 2);
}
