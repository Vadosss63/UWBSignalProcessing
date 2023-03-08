#include "transmitterudpmodule.h"
TransmitterUDPModule::TransmitterUDPModule() : SinkModule(1){}

TransmitterUDPModule::~TransmitterUDPModule()
{
    StopServer();
}

void TransmitterUDPModule::StopServer()
{
    delete m_server;
    m_server = nullptr;
}

void TransmitterUDPModule::StartServer()
{
    StopServer();
    m_server = new NetworkUWB::SenderUDP(m_ip, 12244);
}

void TransmitterUDPModule::CreateProtocol(InBuffer& data)
{
    boost::asio::streambuf buf;
    std::ostream requestStream(&buf);
    size_t size = data.first.size();
    requestStream.write(reinterpret_cast<char const*>(&size), sizeof(size));
    requestStream << static_cast<csignal<int16_t>>(data.first);
    requestStream << static_cast<csignal<int16_t>>(data.second);
    requestStream.flush();
    m_server->SendData(buf);

}

void TransmitterUDPModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    InBuffer target;

    if(!PopFromInput(target))
        return;

    if(m_server)
        CreateProtocol(target);
}

void TransmitterUDPModule::StartEvent()
{
    StartServer();
    SinkModule::StartEvent();
}

void TransmitterUDPModule::StopEvent()
{
    StopServer();
    SinkModule::StopEvent();
}

