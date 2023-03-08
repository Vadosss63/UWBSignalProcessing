#include "receiverudpmodule.h"
#include <cstdio>
namespace dspm
{

///
/// \brief PRLIClient::PRLIClient
/// \param parent
///
PRLIClient::PRLIClient(QObject *parent):Network::ClientUDP("224.1.1.30", 22251, sizeof(Datagram::DatagramPRLI::PRLI) + sizeof(Datagram::Header), parent){}

PRLIClient::~PRLIClient(){}

bool PRLIClient::PopCSignal(csignal<uint8_t>& sig)
{
    return m_dataCsignal.try_pop(sig);
}

void PRLIClient::SetCoef(size_t coef)
{
    m_coef = coef;
}

void PRLIClient::CreateData()
{
    std::vector<uint8_t> data;
    PopData(data);
    DataParser::Parser<Datagram::DatagramPRLI::PRLI> parser(data, data.size(), 2);
    PushCollection(parser);
}

void PRLIClient::PushCollection(DataParser::Parser<Datagram::DatagramPRLI::PRLI> &parser)
{
    DataParser::IPackage<Datagram::DatagramPRLI::PRLI> package;
    if(!parser.GetPackage(package))
    {
        m_collection.clear();
        return;
    }
    csignal<uint8_t> temp(package.infoBody().significantNumberOfDiscrete);
    for (size_t i = 0; i < temp.size(); ++i)
        temp[i] = package.infoBody().data[i];

    temp.set_azimuth(qRound(package.infoBody().bearing * 360.0 / std::pow(2,16) * 64 / 90));
    m_collection[package.header().fragmentIndentifierMessage] = temp;

    if (m_collection.size() == package.header().numberOfInformationPackets)
    {
        size_t size = 0;
        size_t coef = m_coef;
        for (size_t i = 1; i < m_collection.size()+1; ++i)
            size += m_collection[i].size();

        temp.reset(size/coef);
        for (size_t i = 1; i < m_collection.size() + 1; ++i)
            for (size_t j = 0; j < m_collection[i].size() / coef; ++j)
                temp[j +  (i-1) * m_collection[i].size() / coef] = m_collection[i][j * coef];

        m_dataCsignal.push(std::move(temp));
        emit PendingData();
        m_collection.clear();
    }
}

///
/// \brief ReceiverUDPModule::ReceiverUDPModule
/// \param parent
///

ReceiverUDPModule::ReceiverUDPModule(QObject* parent) :
    SourceModule(static_cast<ports_quantity>(1), parent), m_client(this), m_frame(0)
{
    connect(&m_client, SIGNAL(PendingData()), this, SLOT(QueueOperation()));
    connect(this, SIGNAL(SetCoef(size_t)), &m_client, SLOT(SetCoef(size_t)));
}

ReceiverUDPModule::~ReceiverUDPModule(){}

void ReceiverUDPModule::Operate()
{    
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<uint8_t> sig;
    if (!m_client.PopCSignal(sig))
        return;

    csignal<double>temp(static_cast<csignal<double>>(sig));
    temp.set_azimuth(sig.azimuth());
    temp.set_frame_number(m_frame++);
    PushToOutput(std::move(temp));
}

void ReceiverUDPModule::StartThread()
{
    if (m_client.IsStart())
        m_client.Stop();
    m_client.Start();
}

void ReceiverUDPModule::StopThread()
{
    if (m_client.IsStart())
        m_client.Stop();
}

void ReceiverUDPModule::StartOperation()
{
    StartThread();
}

void ReceiverUDPModule::StopOperation()
{
    m_frame = 0;
    SourceModule::StopOperation();
    StopThread();
}

void ReceiverUDPModule::SetCoefRecive(size_t coef)
{
    emit SetCoef(coef);
}


}


