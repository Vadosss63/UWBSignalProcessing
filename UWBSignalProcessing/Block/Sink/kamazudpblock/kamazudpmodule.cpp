#include "kamazudpmodule.h"

KamazUDPModule::KamazUDPModule() : SinkModule(1), m_server(nullptr) {
  m_dataSizeToSend =
      sizeof(KamazDatagram::HeaderPRLI) + sizeof(KamazDatagram::DataPRLI);
  m_dataSend = new uint8_t[m_dataSizeToSend];
}

KamazUDPModule::~KamazUDPModule() {
  delete[] m_dataSend;
  StopServer();
}

void KamazUDPModule::StopServer() {
  if (m_server) {
    delete m_server;
    m_server = nullptr;
  }
}

void KamazUDPModule::StartServer() {
  StopServer();
  m_server = new NetworkUWB::SenderUDP("127.0.0.1", 10000);
}

void KamazUDPModule::CreateProtocol(csignal<double> &sig) {
  //    количество пакетов
  size_t maxSizeData = KamazDatagram::maxSize / sizeof(uint16_t);
  uint8_t size = static_cast<uint8_t>(sig.size() / (maxSizeData));
  double discret = 0.3;

  // выполняем создание массива с данными
  for (uint8_t i = 0; i < size; ++i) {
    csignal<double> sigData;

    // структура данных для отправки
    sigData.reset(maxSizeData);

    std::copy(sig.begin() + maxSizeData * i,
              sig.begin() + (maxSizeData * i + sigData.size()),
              sigData.begin());

    sigData.setAzimuth(sig.azimuth());

    KamazDatagram::HeaderPRLI *headerPRLI =
        reinterpret_cast<KamazDatagram::HeaderPRLI *>(m_dataSend);
    /// ЗАПОЛНЕНИЕ ЗАГОЛОВКА
    headerPRLI->countPackage = size; // Количество информационных пакетов
    headerPRLI->idPackage = i + 1; // Идентификатор фрагмента сообщения
    headerPRLI->significantNumberOfDiscrete =
        static_cast<uint32_t>(sigData.size()); // Количество значемых дискретов
    headerPRLI->cyclicMessageNumber = getCyclicNum();
    headerPRLI->bearing = sigData.azimuth(); // угол места
    headerPRLI->rangeOfFirstElement = static_cast<uint32_t>(
        sigData.size() * i * discret); // Дальность первого элемента массива

    const size_t sheftHeader = sizeof(KamazDatagram::HeaderPRLI);
    KamazDatagram::DataPRLI *dataPRLI =
        reinterpret_cast<KamazDatagram::DataPRLI *>(&m_dataSend[sheftHeader]);

    /// ЗАПОЛНЕНИЕ ДАННЫХ
    for (size_t j = 0; j < sigData.size(); ++j)
      dataPRLI->data[j] = static_cast<uint16_t>(sigData[j]);

    m_server->SendData(m_dataSend, m_dataSizeToSend);
  }
}

uint32_t KamazUDPModule::getCyclicNum() {
  if (m_cyclicNum) {
    return m_cyclicNum++;
  }
  return (m_cyclicNum = 1);
}

void KamazUDPModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> temp;
  if (!PopFromInput(temp))
    return;
  if (m_server)
    CreateProtocol(temp);
}

void KamazUDPModule::StartEvent() {
  StartServer();
  SinkModule::StopEvent();
}
