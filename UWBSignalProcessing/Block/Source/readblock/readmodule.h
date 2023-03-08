#ifndef READMODULE_H
#define READMODULE_H

#include <string>
#include "sourcemodule.h"
#include "csignal.h"
#include "typesignal.h"

class ReadModule: public SourceModule<csignal<double>>
{

public:

    ReadModule();
    ~ReadModule() override;

    void SetPathFile(const std::string& pathFile);

    void StartEvent() override;

    void StopEvent() override;

    void TimerTimout() override;

    void Operate() override;

private:
    // Чтение сигнала из файла
    csignal<double> ReadSignal();
    //читает сервисную информацию из файла. и если она верна,
    //то устанавливает m_flagReadFile = true
    bool ReadServiceParametersInBin();
    // флаг чтения файла (если false, то файл или имеет не верный формат, или закончился)
    bool m_flagReadFile = false;
    // путь к файлу csignal
    std::string m_pathFile = "";
    // длина csignal
    size_t m_lengthCsignal = 0;
    //поток чтения из файла в csignal
    std::ifstream m_readFile;
    // тип сохраненного сигнала
    TypeSignal m_typeSignal = TypeSignal::doubleType;
};

#endif // READMODULE_H
