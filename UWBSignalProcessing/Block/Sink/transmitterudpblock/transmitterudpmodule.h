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
  void StartServer();
  void StopServer();

  void CreateProtocol(InBuffer &data);
  NetworkUWB::SenderUDP *m_server = nullptr;

  ///TODO: add dialogWindow to change parameters
  static constexpr const char* m_ipAddress = "127.0.0.1";
  static constexpr uint16_t m_port = 12244;
};

#endif // TRANSMITTERUDPMODULE_H
