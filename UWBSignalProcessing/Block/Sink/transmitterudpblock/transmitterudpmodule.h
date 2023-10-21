#ifndef TRANSMITTERUDPMODULE_H
#define TRANSMITTERUDPMODULE_H

#include "csignal.h"
#include "networkuwb.h"
#include "sinkmodule.h"
#include <array>
#include <string>
#include <thread>

using InBuffer = std::pair<csignal<double>, csignal<double>>;
class TransmitterUDPModule : public SinkModule<InBuffer> {

public:
  TransmitterUDPModule();
  ~TransmitterUDPModule() override;

  void Operate() override;
  void StartEvent() override;
  void StopEvent() override;

private:
  // слот запуска сервера
  void StartServer();
  void StopServer();
  // создание протокола передачи данных
  void CreateProtocol(InBuffer &data);
  NetworkUWB::SenderUDP *m_server = nullptr;
  // std::string m_ip = "192.168.0.10";
  std::string m_ip = "127.0.0.1";
};

#endif // TRANSMITTERUDPMODULE_H
