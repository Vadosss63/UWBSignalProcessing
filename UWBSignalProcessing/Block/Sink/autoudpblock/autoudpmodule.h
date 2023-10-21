#ifndef KAMAZUDPMODULE_H
#define KAMAZUDPMODULE_H

#include "sinkmodule.h"
#include <array>
#include <string>
#include <thread>
#include <vector>

#include "autodatagram.h"
#include "networkuwb.h"
#include "target.h"

using InBuffer = std::vector<Target>;
class AutoUDPModule : public SinkModule<InBuffer> {

public:
  AutoUDPModule();
  ~AutoUDPModule() override;

  void Operate() override;
  void StartEvent() override;
  void StopEvent() override;

private:
  // слот запуска сервера
  void StartServer();
  void StopServer();
  // создание протокола передачи данных
  void CreateProtocol(std::vector<Target> &target);
  NetworkUWB::SenderUDP *m_server = nullptr;
  std::string m_ip = "192.168.0.10";
};

#endif // KAMAZUDPMODULE_H
