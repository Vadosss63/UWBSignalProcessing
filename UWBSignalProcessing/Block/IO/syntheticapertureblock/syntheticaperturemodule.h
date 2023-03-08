#ifndef SYNTHETICAPERTUREMODULE_H
#define SYNTHETICAPERTUREMODULE_H

#include "iomodule.h"
#include "csignal.h"

class SyntheticApertureModule : public IOModule<csignal<double>,  csignal<double>>
{

public:

    SyntheticApertureModule();
    ~SyntheticApertureModule() override = default;

    void Operate() override;
    void SetIndexStop(std::vector<size_t> indexStop);

private:    

    void PushBuffer(csignal<double>& signal);
    void SyntheticAperture();
    void AccumSignal(csignal<double>& signal, size_t start, size_t stop, size_t countAntennes);
    void ClearBuffer();
    // размер буфера
    size_t m_sizeBuffer = 16;
    std::vector<csignal<double>> m_bufferAperture;
    std::vector<size_t> m_indexStop;

};

#endif // SYNTHETICAPERTUREMODULE_H
