#include "writemodule.h"
#include <iostream>

WriteModule::WriteModule():SinkModule(1) {}

uint16_t WriteModule::GetSizeofType()
{
    return static_cast<uint16_t>(m_typeWriteSignal);
}

void WriteModule::AddServiceParametersInBin()
{
    std::ofstream wreadFile(m_pathFile, std::ios_base::binary);
    if(wreadFile)
    {
        unsigned short barker = 0xE240;
        wreadFile.write(reinterpret_cast<char const*>(&barker), sizeof barker);
        wreadFile.write(reinterpret_cast<char const*>(&m_lengthCsignal), 2);
        unsigned short sizeofType = GetSizeofType();
        wreadFile.write(reinterpret_cast<char const*>(&sizeofType), sizeof sizeofType);
        unsigned short doubleHash = typeid(double).hash_code();
        wreadFile.write(reinterpret_cast<char const*>(&doubleHash), sizeof(doubleHash));
        wreadFile.close();
        m_writeFile.open(m_pathFile, std::ios_base::binary | std::ios_base::out | std::ios_base::app);
    }
}

WriteModule::~WriteModule()
{    
    m_writeFile.close();
}

void WriteModule::SetPathFile(const std::string& pathFile)
{
    if(m_writeFile)
        m_writeFile.close();
    m_pathFile = pathFile;
    m_flagAddServiceParameters = true;
}

void WriteModule::SetTypeWrite(TypeSignal type)
{
    m_typeWriteSignal = type;
}

void WriteModule::Operate()
{
    if(m_pathFile.empty())
    {
        // Вычитываем буффер
        csignal<double> temp;
        PopFromInput(temp);
        return;
    }

    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;

    if(!PopFromInput(temp))
        return;

    if(m_flagAddServiceParameters)
    {
        m_lengthCsignal = temp.size();
        AddServiceParametersInBin();
        m_flagAddServiceParameters = false;
    }

    switch (m_typeWriteSignal)
    {
    case TypeSignal::doubleType:
        m_writeFile << temp;
        break;

    case TypeSignal::int_8_Type:
        m_writeFile << static_cast<csignal<int8_t>>(temp);
        break;

    case TypeSignal::uint_8_Type:
        m_writeFile << static_cast<csignal<uint8_t>>(temp);
        break;

    case TypeSignal::int_16_Type:
        m_writeFile << static_cast<csignal<int16_t>>(temp);
        break;

    case TypeSignal::uint_16_Type:
        m_writeFile << static_cast<csignal<uint16_t>>(temp);
        break;
    }


}
