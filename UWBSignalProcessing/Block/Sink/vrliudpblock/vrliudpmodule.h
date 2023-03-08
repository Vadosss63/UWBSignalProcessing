#ifndef TRANSMITTERUDPMODULE_H
#define TRANSMITTERUDPMODULE_H

#include <string>
#include <thread>
#include <array>
#include "sinkmodule.h"
#include "csignal.h"
#include "networkuwb.h"
#include "datavrli.h"

class VRLIUDPModule : public SinkModule<std::vector<DataVRLI>>
{

public:

    VRLIUDPModule();
    ~VRLIUDPModule() override;

    void Operate() override;
    void StartEvent() override;

private:

    void StopServer();
    // слот запуска сервера
    void StartServer();
    // создание протокола передачи данных
    void CreateProtocol();
    // дата для отправки
    std::vector<DataVRLI> m_dataTarget;
    // класс для отправки данных
    NetworkUWB::ReciverUDP m_server;
    // Количество количество углов в диапазоне
    uint32_t m_countAngel = 64;
    // Начало диапазона сканирования углов
    float m_angelBegin = -45;
    // Значние одного угла
    float m_valueAngel = (90.f/64.f);
    // Значение дискрета дальности
    float m_valueDiscret = 0.3f;

};

#endif // TRANSMITTERUDPMODULE_H
