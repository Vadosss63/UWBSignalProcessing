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

  void StartThread();
  void StopThread();

private:
  NetworkUWB::ReciverUDP m_client;

  static constexpr const char* m_ipAddress = "127.0.0.1";
  ///TODO: add dialogWindow to change parameters
  static constexpr uint16_t m_port = 12244;
};

#endif // RECEIVERUDPMODULE_H
