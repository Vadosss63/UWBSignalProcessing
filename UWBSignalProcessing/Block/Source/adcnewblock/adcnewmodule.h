#ifndef ADCNEWMODULE_H
#define ADCNEWMODULE_H

#include <map>
#include "csignal.h"
#include "sourcemodule.h"
#include "networkuwb.h"

#include "adcprotocol.h"
#include "adccontroller.h"


using OutBufferADC = std::pair<csignal<double>, csignal<double>>;

class ADCNewModule : public SourceModule<OutBufferADC>
{

public:

    ADCNewModule();
    ~ADCNewModule() override;

    ADCNewModule(const ADCNewModule& rhs) = delete;
    ADCNewModule& operator=(const ADCNewModule& rhs) = delete;

    void Operate() override;
    void StopEvent() override;
    void StartEvent() override;

    void SetAdcMode(ADC_SYNC sync_mode, ADC_MODE adc_mode, SAMPLE_NUM samp_num,
                    AVER_NUM aver_num, AVER_MODE aver_mode);
    // Координаты X и Y, м
    // Скорость V, км/ч
    void SetImitationMode(int x, int y, int v);

    void SetFormatAngel(int stepAngel);

    void SetNormalMode();

    ConnectProtocol::STATUS_HARDWARE GetStatusHardware();

    void SetShiftQ(int8_t shift);
    void SetShiftLevelI(int16_t shiftLevelI);
    void SetShiftLevelQ(int16_t shiftLevelQ);
    void SetIsOneRun(bool isOneRun);

private:
    // Сброс параметвов сигнала
    void ResetParametrsSignal();
    // Установка режима работы АЦП
    bool SendSetMod();
    //инициализация сокетов
    bool StartADC();
    void StopADC();
    // процесс сборки сигналов
    void ProcessDataStrobe();
    // Процесс палучения пакетов
    void ProcessDataStrobePacket();
    // Настройка режима работы синхронизатора
    void SetSyncMode();

    UART_UWB::ImitationTarrget m_imitationTarrget;
    // IP адрес
    std::string m_ip = "192.168.1.99";
    // порт
    ushort m_port = 60001;
    // класс для отправки данных
    NetworkUWB::ReciverUDP* m_reciver;
    //приемник сырых данных АЦП
    AdcController m_adcController;
    std::map<u_int8_t, DataADC> m_receivedPacketsBuffer;
    size_t m_sizeData = 512;
    uint32_t m_az = 0;
    int m_stepAngel = 1;
    ushort m_frame = 0;
    uint8_t firstStbNum = 0;


    bool m_isOneRun = true;
    bool m_isNormalMod = true;

    int16_t m_shiftLevelI = -50;
    int16_t m_shiftLevelQ = 20;
    int8_t m_shiftQ = 0;

    ADC_SYNC m_sync_mode;
    ADC_MODE m_adc_mode;
    SAMPLE_NUM m_samp_num;
    AVER_NUM m_aver_num;
    AVER_MODE m_aver_mode;
};

#endif // ADCNEWMODULE_H
