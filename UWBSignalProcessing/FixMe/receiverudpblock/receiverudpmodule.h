#ifndef RECEIVERUDPMODULE_H
#define RECEIVERUDPMODULE_H

#include <boost/asio.hpp>
#include <boost/crc.hpp>
#include <map>
#include <string>
#include <thread>

#include "abstractmodule.h"
#include "dataparser.h"
#include "network.h"

namespace dspm {
// Парусов В.А.
//Класс ClientUDP создает отдельный поток для прослушки сокета
//Функция PushCollection выполняет сборку сигнала в csignal
//По готовности данных вызывается сигнал PendingData
class PRLIClient : public Network::ClientUDP {
  Q_OBJECT
public:
  PRLIClient(QObject *parent);
  ~PRLIClient();
  // получение csignal из очереди
  bool PopCSignal(csignal<uint8_t> &sig);
public slots:
  void SetCoef(size_t coef);

protected:
  void CreateData();

private:
  void PushCollection(DataParser::Parser<Datagram::DatagramPRLI::PRLI> &parser);
  // потоко безопастная очередь сырых данных
  tbb::concurrent_queue<csignal<uint8_t>> m_dataCsignal;
  // колекция для сложного сигнала номер позиции
  std::map<uint8_t, csignal<uint8_t>> m_collection;
  size_t m_coef = 1;
};

//Класс ReceiverUDPModule  выполяет обработку  сигнала, принятого по протоколу
//UDP
// Предназначен для приема сырых данных из UDP
class ReceiverUDPModule : public SourceModule {
  Q_OBJECT
public:
  class ControlCommand : public std::function<void(void)> {
  public:
    ControlCommand(ReceiverUDPModule *parent, size_t coef)
        : std::function<void(void)>(
              std::bind(&ReceiverUDPModule::SetCoefRecive, parent, coef)) {}
  };

  explicit ReceiverUDPModule(QObject *parent = nullptr);
  ReceiverUDPModule(const ReceiverUDPModule &rhs) = delete;
  ~ReceiverUDPModule() override;
  ReceiverUDPModule &operator=(const ReceiverUDPModule &rhs) = delete;

  void Operate() override;
signals:
  void SetCoef(size_t val);
public slots:

  // создание потока и запуск прослушки порта
  void StartThread();
  // остановка потока
  void StopThread();

protected slots:
  // слот остановки схемы
  void StartOperation() override;
  void StopOperation() override;

private:
  void SetCoefRecive(size_t coef);

  // приемник данных из UDP
  PRLIClient m_client;
  // номер кадра
  unsigned short m_frame;
};
} // namespace dspm

#endif // RECEIVERUDPMODULE_H
