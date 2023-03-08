#ifndef WRITEMODULE_H
#define WRITEMODULE_H

#include <string>
#include "sinkmodule.h"
#include "csignal.h"
#include "typesignal.h"

class WriteModule : public SinkModule<csignal<double>>
{    

public:

    WriteModule();
    ~WriteModule() override;

    void Operate() override;

    //устанавливает путь файла с csignal и устанавливает  m_flagAddServiceParameters = true
    void SetPathFile(const std::string& pathFile);
    // Установка типа записи
    void SetTypeWrite(TypeSignal type);

private:
    //добавляет сервисную информацию в файл.
    void AddServiceParametersInBin();
    uint16_t GetSizeofType();
    //флаг сообщает Operate() что указан новый путь к файлу bin
    bool m_flagAddServiceParameters = false;
    //путь к файлу csignal
    std::string m_pathFile = "";
    //длина csignal
    size_t m_lengthCsignal = 0;
    //поток записи из csignal в файл
    std::ofstream m_writeFile;
    TypeSignal m_typeWriteSignal = TypeSignal::doubleType;

};

#endif // WRITEMODULE_H
