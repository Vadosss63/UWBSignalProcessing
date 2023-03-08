#ifndef RECEIVERKAMAZUDPMODULE_H
#define RECEIVERKAMAZUDPMODULE_H

#include <string>
#include <thread>
#include <map>
#include "sourcemodule.h"
#include "networkuwb.h"
#include "kamazdatagramprli.h"
#include "csignal.h"

class ReceiverKamazUDPModule : public SourceModule< csignal<double>>
{

public:

    ReceiverKamazUDPModule();
    ~ReceiverKamazUDPModule() override = default;

    void Operate() override;
    void StopEvent() override;
    void StartEvent() override;

    // создание потока и запуск прослушки порта
    void StartThread();
    // остановка потока
    void StopThread();

private:

    void CreateData();

    unsigned short m_frame; // номер кадра
    std::map<int, KamazDatagram::PackageKamaz> m_data;
    NetworkUWB::ReciverUDP m_client;

};


#endif // RECEIVERUDPMODULE_H
