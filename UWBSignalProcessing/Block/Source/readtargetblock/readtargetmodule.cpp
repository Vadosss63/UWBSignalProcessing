#include "readtargetmodule.h"

ReadTargetModule::ReadTargetModule(): SourceModule(1){ }

ReadTargetModule::~ReadTargetModule()
{
    if (m_readFile.is_open())
        m_readFile.close();
}

void ReadTargetModule::SetPathFile(const std::string& pathFile)
{
    if (m_readFile.is_open())
        m_readFile.close();

    m_pathFile = pathFile;

    if (ReadServiceParametersInBin())
        m_flagReadFile = true;
}

void ReadTargetModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    //для проверки на окончание файла
    m_readFile.peek();
    if(!m_readFile.eof())
    {
        // Заполняем заголовок
        AutoDatagram::Header header;
        m_readFile.read(reinterpret_cast<char*>(&header), sizeof(header));
        size_t maxSpeed;
        m_readFile >> maxSpeed;

        OutBuffer targets;
        targets.reserve(header.countTarget);
        for (uint32_t i = 0; i < header.countTarget; ++i) {
            Target target;
            m_readFile >> target;
            targets.push_back(target);
        }
        Target::SetMaxSpeed(maxSpeed);
        PushToOutput(std::move(targets));

    }
    else
    {
        m_flagReadFile = false;
        StopedModule();
    }


}
//    std::lock_guard<std::mutex> lock(operateLock);
//    //для проверки на окончание файла
//    m_readFile.peek();
//    if(!m_readFile.eof())
//    {
//        AutoDatagram::Header* header = new AutoDatagram::Header;
//        m_readFile.read(reinterpret_cast<char*>(header), sizeof (AutoDatagram::Header));
//        OutBuffer outBuffer(header->countTarget);
//        AutoDatagram::DataTarget* dataTarget = new AutoDatagram::DataTarget;
//        for (size_t i = 0; i < header->countTarget; ++i) {
//            m_readFile.read(reinterpret_cast<char*>(dataTarget), sizeof (AutoDatagram::DataTarget));
//            outBuffer[i].amp = static_cast<double>(dataTarget->amp);

//            outBuffer[i].distence = static_cast<int>(dataTarget->distence / outBuffer[i].resulution);
//            outBuffer[i].direction = static_cast<double>(dataTarget->direction);
//            outBuffer[i].startDistence = static_cast<int>(dataTarget->startDistence / outBuffer[i].resulution);
//            outBuffer[i].stopDistence = static_cast<int>(dataTarget->stopDistence / outBuffer[i].resulution);

//            int countSpeed = 128;
//            float speed = dataTarget->speed;
//            int speedInt = 0;

//            if(speed < 0)
//                speedInt = -countSpeed * speed /(3.6 * outBuffer[i].maxSpeed) + 2 * countSpeed;
//            else
//                speedInt = speed * countSpeed/ (-3.6 * outBuffer[i].maxSpeed);

//            outBuffer[i].speed = static_cast<int>(speedInt);
//        }
//        delete  header;
//        delete  dataTarget;
//        PushToOutput(std::move(outBuffer));

//    }



void ReadTargetModule::StartEvent()
{
    SetPathFile(m_pathFile);
}

void ReadTargetModule::StopEvent()
{
    std::lock_guard<std::mutex> lock(operateLock);
    if (m_readFile.is_open())
        m_readFile.close();

    SourceModule::StopEvent();
}

bool ReadTargetModule::ReadServiceParametersInBin()
{
    m_readFile.open(m_pathFile, std::ios_base::binary | std::ios_base::in);
    return static_cast<bool>(m_readFile);
}

void ReadTargetModule::TimerTimout()
{
    if(m_flagReadFile)
        QueueOperation();
}
