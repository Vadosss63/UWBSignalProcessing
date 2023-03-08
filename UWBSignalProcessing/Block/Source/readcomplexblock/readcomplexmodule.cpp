#include "readcomplexmodule.h"
#include <cstdio>

ReadComplexModule::ReadComplexModule(): SourceModule(1){}

ReadComplexModule::~ReadComplexModule()
{
    CloseStream();
}

void ReadComplexModule::SetPathFile(const std::string& pathFileSigI, const std::string& pathFileSigQ)
{
    CloseStream();
    m_pathFileSigI = pathFileSigI;
    m_pathFileSigQ = pathFileSigQ;
    m_flagReadFile = ReadServiceParametersInBinSigI() && ReadServiceParametersInBinSigQ();
}

void ReadComplexModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    //для проверки на окончание файла
    for (int i = 0; i < 256; ++i) {

        if(!m_flagReadFile) return;
        m_readFileSigI.peek();
        m_readFileSigQ.peek();
        if(!m_readFileSigI.eof() && !m_readFileSigQ.eof())
        {
            PushToOutput(std::make_pair(ReadSignalI(), ReadSignalQ()));
        }
        else
        {
            m_flagReadFile = false;
            StopedModule();
            return;
        }
    }

}

void ReadComplexModule::StartEvent()
{
    SetPathFile(m_pathFileSigI, m_pathFileSigQ);
}

void ReadComplexModule::StopEvent()
{
    CloseStream();
    SourceModule::StopEvent();
}

bool ReadComplexModule::ReadServiceParametersInBinSigI()
{
    m_readFileSigI.open(m_pathFileSigI, std::ios_base::binary | std::ios_base::in);
    if (m_readFileSigI)
    {
        unsigned short barker;
        m_readFileSigI.read(reinterpret_cast<char*>(&barker), 2);
        if(barker == 0xE240)
        {
            m_readFileSigI.read(reinterpret_cast<char*>(&m_lengthSigI), 2 );

            unsigned short sizeofType;
            m_readFileSigI.read(reinterpret_cast<char*>(&sizeofType), sizeof sizeofType);
            m_typeSignalI = static_cast<TypeSignal>(sizeofType);

            unsigned short doubleHash;
            m_readFileSigI.read(reinterpret_cast<char*>(&doubleHash), sizeof(doubleHash));

            if( doubleHash == static_cast<unsigned short>(typeid(double).hash_code()))
            {
                return true;
            }
        }
    }

    return false;
}

bool ReadComplexModule::ReadServiceParametersInBinSigQ()
{
    m_readFileSigQ.open(m_pathFileSigQ, std::ios_base::binary | std::ios_base::in);
    if (m_readFileSigQ)
    {
        unsigned short barker;
        m_readFileSigQ.read(reinterpret_cast<char*>(&barker), 2);
        if(barker == 0xE240)
        {
            m_readFileSigQ.read(reinterpret_cast<char*>(&m_lengthSigQ), 2 );

            unsigned short sizeofType;
            m_readFileSigQ.read(reinterpret_cast<char*>(&sizeofType), sizeof sizeofType);
            m_typeSignalQ = static_cast<TypeSignal>(sizeofType);

            unsigned short doubleHash;
            m_readFileSigQ.read(reinterpret_cast<char*>(&doubleHash), sizeof(doubleHash));

            if(doubleHash == static_cast<unsigned short>(typeid(double).hash_code()))
            {
                return true;
            }
        }
    }

    return false;
}

csignal<double> ReadComplexModule::ReadSignalI()
{
    switch (m_typeSignalI)
    {
    case TypeSignal::doubleType:
    {
        csignal<double> sig(m_lengthSigI);
        return  m_readFileSigI >> sig;
    }
    case TypeSignal::int_8_Type:
    {
        csignal<int8_t> sig(m_lengthSigI);
        m_readFileSigI >> sig;
        return static_cast<csignal<double>>(sig);
    }

    case TypeSignal::uint_8_Type:
    {
        csignal<uint8_t> sig(m_lengthSigI);
        m_readFileSigI >> sig;
        return static_cast<csignal<double>>(sig);
    }
    case TypeSignal::int_16_Type:
    {
        csignal<int16_t> sig(m_lengthSigI);
        m_readFileSigI >> sig;
        return static_cast<csignal<double>>(sig);
    }
    case TypeSignal::uint_16_Type:
    {
        csignal<uint16_t> sig(m_lengthSigI);
        m_readFileSigI >> sig;
        return static_cast<csignal<double>>(sig);
    }
    }
    return csignal<double>();
}

csignal<double> ReadComplexModule::ReadSignalQ()
{
    switch (m_typeSignalQ)
    {
    case TypeSignal::doubleType:
    {
        csignal<double> sig(m_lengthSigQ);
        return m_readFileSigQ >> sig;
    }
    case TypeSignal::int_8_Type:
    {
        csignal<int8_t> sig(m_lengthSigQ);
        m_readFileSigQ >> sig;
        return static_cast<csignal<double>>(sig);
    }

    case TypeSignal::uint_8_Type:
    {
        csignal<uint8_t> sig(m_lengthSigQ);
        m_readFileSigQ >> sig;
        return static_cast<csignal<double>>(sig);
    }
    case TypeSignal::int_16_Type:
    {
        csignal<int16_t> sig(m_lengthSigQ);
        m_readFileSigQ >> sig;
        return static_cast<csignal<double>>(sig);
    }
    case TypeSignal::uint_16_Type:
    {
        csignal<uint16_t> sig(m_lengthSigQ);
        m_readFileSigQ >> sig;
        return static_cast<csignal<double>>(sig);
    }
    }
    return csignal<double>();
}

void ReadComplexModule::TimerTimout()
{
    if(m_flagReadFile)
        QueueOperation();
}

void ReadComplexModule::CloseStream()
{
    m_flagReadFile = false;
    std::lock_guard<std::mutex> lock(operateLock);
    if (m_readFileSigI.is_open())
        m_readFileSigI.close();

    if (m_readFileSigQ.is_open())
        m_readFileSigQ.close();
}
