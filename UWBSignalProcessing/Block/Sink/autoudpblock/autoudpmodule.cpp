#include "autoudpmodule.h"

AutoUDPModule::AutoUDPModule() : SinkModule(1){}

AutoUDPModule::~AutoUDPModule()
{
    StopServer();
}

void AutoUDPModule::StopServer()
{
    delete m_server;
    m_server = nullptr;
}

void AutoUDPModule::StartServer()
{
    StopServer();
    m_server = new NetworkUWB::SenderUDP(m_ip, 12213);
}

void AutoUDPModule::CreateProtocol(std::vector<Target>& target)
{
    boost::asio::streambuf buf;
    std::ostream requestStream(&buf);
    size_t countTarget = target.size();
    // Заполняем заголовок
    AutoDatagram::Header header;
    header.type = 0x2323;
    header.countTarget = static_cast<uint32_t>(countTarget);
    requestStream.write(reinterpret_cast<char*>(&header), sizeof(header));
    requestStream << Target::GetMaxSpeed();

    for (auto& tar : target)
        requestStream << tar;

    requestStream.flush();
    //    std::cout << "size = " << buf.size() << std::endl;
    m_server->SendData(buf);
}

void AutoUDPModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    InBuffer target;

    if(!PopFromInput(target))
        return;

    if(m_server)
        CreateProtocol(target);
}

void AutoUDPModule::StartEvent()
{
    StartServer();
    SinkModule::StartEvent();
}

void AutoUDPModule::StopEvent()
{
    StopServer();
    SinkModule::StopEvent();
}
