#ifndef TRANSMITTERUDPMODULE_H
#define TRANSMITTERUDPMODULE_H

#include <array>
#include <string>
#include <thread>

#include "endpointmodule.h"
#include "network.h"

namespace dspm {

// Парусов В.А.
// Класс TransmitterUDPModule служит для предачи данных по протоколу UDP
// TransmitterUDPModule включает в себя ServerUDP, который в свою очередь
// является оберткой для boost::asio Данный клас выполняет передачу данных типа
// csignal путем преобразование его в arry массив в конструкторе ServerUDP
// выполняется иницилизация ip адреса и порта протокола UDP

class TransmitterUDPModule : public EndPointModule {
  Q_OBJECT
public:
  // в конструкторе создается обьект с нулевыми параметрами
  explicit TransmitterUDPModule(QObject *parent = nullptr);
  ~TransmitterUDPModule() override;
  TransmitterUDPModule(const TransmitterUDPModule &rhs) = delete;
  TransmitterUDPModule &operator=(const TransmitterUDPModule &rhs) = delete;

  void Operate() override;

protected slots:
  void StartOperation() override;

private:
  void StopServer();
  // слот запуска сервера
  void StartServer();
  // создание протокола передачи данных
  void CreateProtocol(csignal<double> &sig);
  Network::ServerUDP *m_server;
};
} // namespace dspm

#endif // TRANSMITTERUDPMODULE_H
