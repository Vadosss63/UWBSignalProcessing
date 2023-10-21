#include "adcnewmodule.h"
#include <chrono>
#include <iostream>

ADCNewModule::ADCNewModule()
    : SourceModule(1)
{
    m_reciver = new NetworkUWB::ReciverUDP(m_ip, m_port);
    m_reciver->AddCallback([&](){ProcessDataStrobePacket();});
}

ADCNewModule::~ADCNewModule()
{
    m_adcController.AdcStop();
    // Остановка отправки от АЦП
    delete m_reciver;
}

bool ADCNewModule::StartADC()
{    
    if(!m_reciver->Start())
    {
        StopedModule();
        std::cout << m_reciver->GetErrors() << std::endl;
        return false;
    }
    // Запуск передачи данных от АЦП
    return m_adcController.AdcStart();
}

void ADCNewModule::StopADC()
{
    // Остановка передачи данных
    m_adcController.AdcStop();
    m_reciver->Stop();
}

bool ADCNewModule::SendSetMod()
{
    return m_adcController.AdcSetMode(m_sync_mode, m_adc_mode, m_samp_num, m_aver_num, m_aver_mode);
}

void ADCNewModule::SetAdcMode(ADC_SYNC sync_mode, ADC_MODE adc_mode, SAMPLE_NUM samp_num, AVER_NUM aver_num, AVER_MODE aver_mode)
{
    m_sync_mode = sync_mode;
    m_adc_mode = adc_mode;
    m_samp_num = samp_num;
    m_aver_num = aver_num;
    m_aver_mode = aver_mode;
    //    SendSetMod();
}

void ADCNewModule::SetImitationMode(int x, int y, int v)
{
    m_isNormalMod = false;
    m_imitationTarrget.x = x;
    m_imitationTarrget.y = y;
    m_imitationTarrget.v = v;
}

void ADCNewModule::ResetParametrsSignal()
{
    m_az = 0;
    m_frame = 0;
    firstStbNum = 0;
    m_receivedPacketsBuffer.clear();
}

void ADCNewModule::ProcessDataStrobePacket()
{
    DataADC data = m_reciver->GetData<DataADC>();
    m_receivedPacketsBuffer[data.NFrame] = data;

    if(m_receivedPacketsBuffer.size() == static_cast<size_t>(data.TFrame))
    {
        ProcessDataStrobe();
        m_receivedPacketsBuffer.clear();
    }
}

void ADCNewModule::SetSyncMode()
{
    if(m_isNormalMod)
    {
        m_adcController.SetNormalMode();
    }
    else
    {
        m_adcController.SetImitationMode(m_imitationTarrget.x,
                                         m_imitationTarrget.y,
                                         m_imitationTarrget.v, m_isOneRun);
    }
}

void ADCNewModule::SetIsOneRun(bool isOneRun)
{
    m_isOneRun = isOneRun;
}

void ADCNewModule::ProcessDataStrobe()
{
    csignal<double> outI(m_sizeData);
    csignal<double> outQ(m_sizeData);

    if(m_shiftQ >= 0){
        for(int i = 0; i < 512; i++)
            outI[i] = m_receivedPacketsBuffer[0].data[i] + m_shiftLevelI;

        for(int i = 0; i < 512 - m_shiftQ; i++)
            outQ[i] = m_receivedPacketsBuffer[1].data[i + m_shiftQ] + m_shiftLevelQ;

    }else{

        for(int i = 0; i < 512 + m_shiftQ; i++)
            outI[i] = m_receivedPacketsBuffer[0].data[i - m_shiftQ] + m_shiftLevelI;

        for(int i = 0; i < 512; i++)
            outQ[i] = m_receivedPacketsBuffer[1].data[i] + m_shiftLevelQ;
    }

    m_az = m_receivedPacketsBuffer[0].AngelNum;
    outI.setFrameNumber(m_frame);
    outI.setAzimuth(m_az);
    outQ.setFrameNumber(m_frame);
    outQ.setAzimuth(m_az);
    ++m_frame;
    PushToOutput(std::make_pair(std::move(outI), std::move(outQ)));
}

void ADCNewModule::SetFormatAngel(int stepAngel)
{
    m_stepAngel = stepAngel;
}

void ADCNewModule::SetNormalMode()
{
    m_isNormalMod = true;
}

ConnectProtocol::STATUS_HARDWARE ADCNewModule::GetStatusHardware()
{
    return m_adcController.GetStatusHardware();
}

void ADCNewModule::SetShiftQ(int8_t shift)
{
    m_shiftQ = shift;
}

void ADCNewModule::SetShiftLevelI(int16_t shiftLevelI)
{
    m_shiftLevelI = shiftLevelI;
}

void ADCNewModule::SetShiftLevelQ(int16_t shiftLevelQ)
{
    m_shiftLevelQ = shiftLevelQ;
}

void ADCNewModule::Operate(){}

void ADCNewModule::StopEvent()
{
    StopADC();
    SourceModule::StopEvent();
}

void ADCNewModule::StartEvent()
{
    ResetParametrsSignal();
    if(!SendSetMod()) return;
    //    SetSyncMode();
    //    StartADC();

    if(!StartADC()) return;
    SetSyncMode();
    m_adcController.StartSync();
}
