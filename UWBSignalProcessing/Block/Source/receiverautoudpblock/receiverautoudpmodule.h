#ifndef RECEIVERAUTOUDPMODULE_H
#define RECEIVERAUTOUDPMODULE_H

#include <string>
#include <thread>
#include <map>
#include "sourcemodule.h"
#include "networkuwb.h"
#include "autodatagram.h"
#include "target.h"

using OutBuffer = std::vector<Target>;

class ReceiverAutoUDPModule : public SourceModule<OutBuffer>
{

public:

    ReceiverAutoUDPModule();
    ~ReceiverAutoUDPModule() override = default;

    void Operate() override;
    void StopEvent() override;
    void StartEvent() override;

    // создание потока и запуск прослушки порта
    void StartThread();
    // остановка потока
    void StopThread();

private:

    NetworkUWB::ReciverUDP m_client;

};


#endif // RECEIVERAUTOUDPMODULE_H
