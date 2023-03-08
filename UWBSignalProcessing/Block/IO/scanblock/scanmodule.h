#ifndef SQUAREMODULE_H
#define SQUAREMODULE_H

#include "iomodule.h"
#include "csignal.h"

class ScanModule : public IOModule<csignal<double>,  std::vector<csignal<double>>>
{
public:

    ScanModule();
    ~ScanModule() override = default;
    void Operate() override;

private:

    bool IsFullScan();
    void ResetScan();

    size_t m_countAngel = 64;
    std::vector<csignal<double>> m_scan;

};

#endif // SQUAREMODULE_H
