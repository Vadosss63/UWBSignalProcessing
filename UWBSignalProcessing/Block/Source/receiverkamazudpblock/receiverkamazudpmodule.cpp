#include "receiverkamazudpmodule.h"
#include <cstdio>

ReceiverKamazUDPModule::ReceiverKamazUDPModule()
    : SourceModule<csignal<double>>(static_cast<uint8_t>(1)), m_frame(0),
      m_client(10000) {
  m_client.AddCallback([&]() { CreateData(); });
}

void ReceiverKamazUDPModule::Operate() {
  int countPack = m_data[1].headerPRLI.countPackage;
  size_t sizeDataPack = m_data[1].headerPRLI.significantNumberOfDiscrete;
  csignal<double> temp(countPack * sizeDataPack);

  temp.setAzimuth(m_data[1].headerPRLI.bearing);
  temp.setFrameNumber(m_frame++);

  for (int j = 1; j < countPack + 1; ++j) {

    for (size_t i = 0; i < sizeDataPack; ++i) {
      temp[i + (j - 1) * sizeDataPack] = m_data[j].dataPRLI.data[i];
    }
  }
  PushToOutput(std::move(temp));
  m_data.clear();
}

void ReceiverKamazUDPModule::StartThread() {
  if (m_client.IsStart())
    m_client.Stop();
  m_client.Start();
}

void ReceiverKamazUDPModule::StopThread() {
  if (m_client.IsStart())
    m_client.Stop();
}

void ReceiverKamazUDPModule::StartEvent() { StartThread(); }

void ReceiverKamazUDPModule::StopEvent() {
  m_frame = 0;
  SourceModule::StopEvent();
  StopThread();
}

void ReceiverKamazUDPModule::CreateData() {
  KamazDatagram::PackageKamaz dataRec =
      m_client.GetData<KamazDatagram::PackageKamaz>();
  int id = dataRec.headerPRLI.idPackage;
  size_t count = dataRec.headerPRLI.countPackage;
  m_data[id] = dataRec;
  if (m_data.size() == count) {
    Operate();
  }
}
