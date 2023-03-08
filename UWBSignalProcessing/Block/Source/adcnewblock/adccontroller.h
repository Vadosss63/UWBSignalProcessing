#ifndef ADCCONTROLLER_H
#define ADCCONTROLLER_H
#include <iostream>
#include "adcprotocol.h"
#include "networkuwb.h"
#include "connectprotocol.h"
#include "boostlog.h"
#include "adcprotocol.h"

class AdcController
{
public:
    AdcController();
    ~AdcController();

    // Запуск АЦП
    bool AdcStart();
    // Остановка АЦП
    bool AdcStop();
    // Установка режима работы
    bool AdcSetMode(ADC_SYNC sync_mode, ADC_MODE adc_mode, SAMPLE_NUM samp_num, AVER_NUM aver_num, AVER_MODE aver_mode);
    // Задание параметров для цели в имитационном режиме
    void SetImitationMode(int x, int y, int v, bool isOnenRun = true);
    // Установка нормального режима работы
    void SetNormalMode();
    // Получение информации о состоянии оборудования
    ConnectProtocol::STATUS_HARDWARE GetStatusHardware();
    // Запуск синхронизатора
    bool StartSync();
private:

    // Остановка синхронизатора
    bool StopSync();
    // Зуск контроллера
    void StartThread();
    // Остановка контроллера
    void StopThread();
    // Запуск имитатора
    void StartImCMD();
    // IP адрес
    std::string m_ip = "192.168.1.100";
    // порт
    const ushort m_port = 60000;
    // Канал соединения с СПО
    NetworkUWB::SenderUDP* m_server = nullptr;
};
// функциия вычисления CRC для команд синхронизатора
uint8_t GetCRC_CMD(uint8_t* data, uint8_t size);

#endif // ADCCONTROLLER_H
