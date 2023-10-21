#ifndef RECEIVERUDPMODULE_H
#define RECEIVERUDPMODULE_H

#include "csignal.h"
#include "networkuwb.h"
#include "sourcemodule.h"
#include <map>
#include <string>
#include <thread>

using OutBuffer = std::pair<csignal<double>, csignal<double>>;
class ReceiverUDPModule : public SourceModule<OutBuffer> {

public:
  ReceiverUDPModule();
  ~ReceiverUDPModule() override = default;

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

#endif // RECEIVERUDPMODULE_H
