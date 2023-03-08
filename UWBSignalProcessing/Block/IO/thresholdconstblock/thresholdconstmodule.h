#ifndef THRESHOLDCONSTMODULE_H
#define THRESHOLDCONSTMODULE_H

#include "iomodule.h"
#include "csignal.h"


class ThresholdModule : public IOModule< csignal<double>,  csignal<double>>
{

public:

    ThresholdModule();
    ~ThresholdModule() override = default;

    ThresholdModule(const ThresholdModule& rhs) = delete;
    ThresholdModule& operator=(const ThresholdModule& rhs) = delete;

    void Operate() override;
    void SetParameters(double threshold);

private:    
    double m_threshold = 0;
};


#endif // THRESHOLDCONSTMODULE_H
