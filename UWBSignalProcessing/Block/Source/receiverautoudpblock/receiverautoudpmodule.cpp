#include "receiverautoudpmodule.h"
#include <cstdio>

ReceiverAutoUDPModule::ReceiverAutoUDPModule()
    : SourceModule<OutBuffer>(1), m_client("192.168.0.10", 12213) {
  m_client.AddCallback([&]() { Operate(); });

  m_client.CreateBuffer(20000);
}

void ReceiverAutoUDPModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  boost::asio::streambuf buf;
  std::pair<uint8_t *, size_t> dataUint8 = m_client.GetData_uint8();
  buf.sputn(reinterpret_cast<char const *>(dataUint8.first), dataUint8.second);
  std::istream m_readFile(&buf);
  // Заполняем заголовок
  AutoDatagram::Header header;
  m_readFile.read(reinterpret_cast<char *>(&header), sizeof(header));
  size_t maxSpeed;
  m_readFile.read(reinterpret_cast<char *>(&maxSpeed), sizeof(maxSpeed));
  OutBuffer targets;
  targets.reserve(header.countTarget);
  for (uint32_t i = 0; i < header.countTarget; ++i) {
    Target target;
    m_readFile >> target;
    targets.push_back(target);
  }
  Target::SetMaxSpeed(maxSpeed);
  free(dataUint8.first);
  PushToOutput(std::move(targets));
}

void ReceiverAutoUDPModule::StartThread() {
  if (m_client.IsStart())
    m_client.Stop();
  m_client.Start();
}

void ReceiverAutoUDPModule::StopThread() {
  if (m_client.IsStart())
    m_client.Stop();
}

void ReceiverAutoUDPModule::StartEvent() { StartThread(); }

void ReceiverAutoUDPModule::StopEvent() {
  SourceModule::StopEvent();
  StopThread();
}
