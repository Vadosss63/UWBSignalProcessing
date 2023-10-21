#include "receiverudpmodule.h"
#include <cstdio>

ReceiverUDPModule::ReceiverUDPModule()
    : SourceModule<OutBuffer>(1),
      m_client("192.168.0.10", 12244) /*m_client("127.0.0.1", 12244)*/
{
  m_client.AddCallback([&]() { Operate(); });

  m_client.CreateBuffer(20000);
}

void ReceiverUDPModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  boost::asio::streambuf buf;
  std::pair<uint8_t *, size_t> dataUint8 = m_client.GetData_uint8();
  buf.sputn(reinterpret_cast<char const *>(dataUint8.first), dataUint8.second);
  std::istream m_readFile(&buf);

  size_t size;
  m_readFile.read(reinterpret_cast<char *>(&size), sizeof(size));
  OutBuffer data(size, size);
  csignal<int16_t> sig1(size);
  csignal<int16_t> sig2(size);
  m_readFile >> sig1;
  m_readFile >> sig2;
  free(dataUint8.first);
  PushToOutput(std::make_pair(static_cast<csignal<double>>(sig1),
                              static_cast<csignal<double>>(sig2)));
}

void ReceiverUDPModule::StartThread() {
  if (m_client.IsStart())
    m_client.Stop();
  m_client.Start();
}

void ReceiverUDPModule::StopThread() {
  if (m_client.IsStart())
    m_client.Stop();
}

void ReceiverUDPModule::StartEvent() { StartThread(); }

void ReceiverUDPModule::StopEvent() {
  SourceModule::StopEvent();
  StopThread();
}
