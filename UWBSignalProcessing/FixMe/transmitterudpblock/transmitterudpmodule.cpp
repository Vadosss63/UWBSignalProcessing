#include "transmitterudpmodule.h"
#include "crc16.h"
#include <iostream>

namespace dspm
{
SenderUDPModule::SenderUDPModule(QObject* parent) :
    EndPointModule(parent), m_server(nullptr)
{ }


SenderUDPModule::~SenderUDPModule()
{
    StopServer();
}

void SenderUDPModule::StopServer()
{
    if(m_server){
        delete m_server;
        m_server = nullptr;
    }
}

void SenderUDPModule::StartServer()
{
    StopServer();
   //m_server = new Network::ServerUDP(std::string("224.1.1.30"), 22251);
     m_server = new Network::ServerUDP(std::string("127.0.0.1"), 10000);
}

void SenderUDPModule::CreateProtocol(csignal<double>& sig)
{
    //    количество пакетов
    size_t size = sig.size()/(Datagram::DatagramPRLI::maxSize);
    // значение дискрета 30 см это 3*0.1 м
    uint16_t discret = 3;
    if(sig.size() % (Datagram::DatagramPRLI::maxSize))
        size ++;
    // выполняем создание массива с данными
    for (uint8_t i = 0; i < size; ++i)
    {
        dspm::csignal<double> sigData;
        int64_t sizeSig = sig.size();
        // структура данных для отправки
        if((sizeSig - (Datagram::DatagramPRLI::maxSize * (i+1))) >= 0)
            sigData.reset(Datagram::DatagramPRLI::maxSize);
        else
            sigData.reset(sizeSig - Datagram::DatagramPRLI::maxSize * i);

        std::copy(sig.begin() + Datagram::DatagramPRLI::maxSize * i,sig.begin() +
                  (Datagram::DatagramPRLI::maxSize * i + sigData.size()), sigData.begin());
        sigData.set_azimuth(sig.azimuth());
        Datagram::DatagramPRLI::DataSend data;

        /// ЗАПОЛНЕНИЕ ДАННЫХ
        // Тип источника информации
        data.dataByte.prli.typeOfInformationSource = 0;
        // установка резерва
        data.dataByte.prli.reserve = 0;
        // Количество значимых дискретов
        data.dataByte.prli.significantNumberOfDiscrete = sigData.size();
        // Пелинг
        data.dataByte.prli.bearing = qRound(sigData.azimuth() * std::pow(2,16) / 360.0 * 90 / 64);
        // Угол места
        data.dataByte.prli.angleOfElevation = 0;
        // Значение дискрета
        data.dataByte.prli.valueIncrement = discret;
        // Дальность первого элемента массива
        data.dataByte.prli.rangeOfFirstElement = sigData.size() * i  * discret;

        dspm::csignal<unsigned char> ucSig = static_cast<dspm::csignal<unsigned char>>(sigData);
        for (size_t j = 0; j < ucSig.size(); ++j)
            data.dataByte.prli.data[j] = ucSig[j];
        /// ЗАПОЛНЕНИЕ ЗАГОЛОВКА
        // Поток первичной информации
        data.dataByte.header.type = 2;
        // Количество информационных пакетов
        data.dataByte.header.numberOfInformationPackets = size;
        // Идентификатор фрагмента сообщения.
        data.dataByte.header.fragmentIndentifierMessage = i + 1;
        // !!! индефикатор отправителя сделать последный байт адреса получателя
        data.dataByte.header.senderID = 25;
        // !!! индефикатор получателя
        data.dataByte.header.userID = 25;
        data.dataByte.header.countWorld = sizeof(data) / 4;
    }
}

void SenderUDPModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);
    csignal<double> temp;
    if(!PopFromInput(temp))
        return;
    CreateProtocol(temp);
}

void SenderUDPModule::StartOperation()
{
    StartServer();
}

}


