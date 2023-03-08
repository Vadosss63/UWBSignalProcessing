#include "readmodule.h"
#include <cstdio>

ReadModule::ReadModule(): SourceModule(1){ }

ReadModule::~ReadModule()
{
    if (m_readFile.is_open())
        m_readFile.close();
}

void ReadModule::SetPathFile(const std::string& pathFile)
{
    if (m_readFile.is_open())
        m_readFile.close();

    m_pathFile = pathFile;

    if (ReadServiceParametersInBin())
        m_flagReadFile = true;
}

void ReadModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    //для проверки на окончание файла
    m_readFile.peek();
    if(!m_readFile.eof())
    {
        PushToOutput(ReadSignal());
    }
    else
    {
        if(m_flagReadFile){
            m_flagReadFile = false;
            StopedModule();
        }
    }

}

void ReadModule::StartEvent()
{
    SetPathFile(m_pathFile);
}

void ReadModule::StopEvent()
{
    if (m_readFile.is_open())
        m_readFile.close();

    SourceModule::StopEvent();
}

bool ReadModule::ReadServiceParametersInBin()
{
    m_readFile.open(m_pathFile, std::ios_base::binary | std::ios_base::in);
    if (m_readFile)
    {
        unsigned short barker;
        m_readFile.read(reinterpret_cast<char*>(&barker), 2);
        if(barker == 0xE240)
        {
            m_readFile.read(reinterpret_cast<char*>(&m_lengthCsignal), 2 );

            unsigned short sizeofType;
            m_readFile.read(reinterpret_cast<char*>(&sizeofType), sizeof sizeofType);
            m_typeSignal = static_cast<TypeSignal>(sizeofType);
            unsigned short doubleHash;
            m_readFile.read(reinterpret_cast<char*>(&doubleHash), sizeof(doubleHash));

            if(doubleHash == static_cast<unsigned short>(typeid(double).hash_code()))
            {
                return true;
            }
        }
    }

    return false;
}

void ReadModule::TimerTimout()
{
    if(m_flagReadFile)
        QueueOperation();
}

csignal<double> ReadModule::ReadSignal()
{
    switch (m_typeSignal)
    {
    case TypeSignal::doubleType:
    {
        csignal<double> sig(m_lengthCsignal);
        return  m_readFile >> sig;
    }
    case TypeSignal::int_8_Type:
    {
        csignal<int8_t> sig(m_lengthCsignal);
        m_readFile >> sig;
        return static_cast<csignal<double>>(sig);
    }

    case TypeSignal::uint_8_Type:
    {
        csignal<uint8_t> sig(m_lengthCsignal);
        m_readFile >> sig;
        return static_cast<csignal<double>>(sig);
    }
    case TypeSignal::int_16_Type:
    {
        csignal<int16_t> sig(m_lengthCsignal);
        m_readFile >> sig;
        return static_cast<csignal<double>>(sig);
    }
    case TypeSignal::uint_16_Type:
    {
        csignal<uint16_t> sig(m_lengthCsignal);
        m_readFile >> sig;
        return static_cast<csignal<double>>(sig);
    }
    }
    return csignal<double>();
}
