#ifndef KAMAZUDPMODULE_H
#define KAMAZUDPMODULE_H

#include "csignal.h"
#include "sinkmodule.h"
#include <array>
#include <string>
#include <thread>

#include "kamazdatagramprli.h"
#include "networkuwb.h"

class KamazUDPModule : public SinkModule<csignal<double>> {

public:
  KamazUDPModule();
  ~KamazUDPModule() override;

  void Operate() override;
  void StartEvent() override;

  uint32_t getCyclicNum();

private:
  // слот запуска сервера
  void StartServer();
  void StopServer();

  // создание протокола передачи данных
  void CreateProtocol(csignal<double> &sig);

  NetworkUWB::SenderUDP *m_server;
  uint32_t m_cyclicNum = 0;
  size_t m_dataSizeToSend;
  uint8_t *m_dataSend;
};

#endif // KAMAZUDPMODULE_H
