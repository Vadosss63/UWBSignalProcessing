#ifndef READTARGETMODULE_H
#define READTARGETMODULE_H

#include <string>
#include <cstdio>
#include "sourcemodule.h"
#include "target.h"
#include "csignal.h"
#include "autodatagram.h"

using OutBuffer = std::vector<Target>;
class ReadTargetModule: public SourceModule<OutBuffer>
{

public:
    ReadTargetModule();
    ~ReadTargetModule() override;

    void SetPathFile(const std::string& pathFile);
    void Operate() override;
    void StartEvent() override;
    void StopEvent() override;
    void TimerTimout() override;

private:
    //читает сервисную информацию из файла. и если она верна,
    //то устанавливает m_flagReadFile = true
    bool ReadServiceParametersInBin();
    //флаг чтения файла (если false, то файл или имеет не верный формат, или закончился)
    bool m_flagReadFile = false;
    //путь к файлу csignal
    std::string m_pathFile = "";
    //поток чтения из файла в csignal
    std::ifstream m_readFile;
};

#endif // READTARGETMODULE_H
