#include "adccontroller.h"

AdcController::AdcController()
{
    StartThread();
}

AdcController::~AdcController()
{
    StopThread();
}

bool AdcController::AdcStart()
{
    AdcStartPackage package;
    package.command = static_cast<uint8_t>(ADC_COMMANDS::ADC_CMD_START);                // Command
    package.runAdc = static_cast<uint8_t>(ADC_START_STOP_CMD::ADC_START_COMMAND);	    // Start/Stop byte
    m_server->SendData<AdcStartPackage>(package);

    if(!m_server->ReadWithTimeout(boost::posix_time::seconds(3)))
    {
        std::string mess =  "No AdcStart";
        std::cout << mess << std::endl;
        std::cout << m_server->GetErrorsMessage() << std::endl;
        //        LOG_ERROR << mess;
        return false;
    }
    std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
    delete p.first;
    std::cout << "ADC start" << std::endl;
    //    LOG_INFO << "ADC start";
    return true;
}

bool AdcController::AdcStop()
{
           if(!StopSync()) return false;/// TODO вернуть назад

    AdcStartPackage package;
    package.command = static_cast<u_int8_t>(ADC_COMMANDS::ADC_CMD_START);               // Command
    package.runAdc = static_cast<u_int8_t>(ADC_START_STOP_CMD::ADC_STOP_COMMAND);	    // Start/Stop byte
    m_server->SendData(package);

    if(m_server->ReadWithTimeout(boost::posix_time::seconds(3)))
    {
        std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
        delete p.first;
        std::cout << "AdcStop" << std::endl;
        std::cout << m_server->GetErrorsMessage() << std::endl;
        //LOG_INFO <<  mess;
        // if(!StopSync()) return false;
        return true;
    }
    std::cout << "No AdcStop" << std::endl;
    //LOG_ERROR << "Error finish ADC";
    return false;
}

bool AdcController::AdcSetMode(ADC_SYNC sync_mode, ADC_MODE adc_mode, SAMPLE_NUM samp_num, AVER_NUM aver_num, AVER_MODE aver_mode)
{
    SetAdcModePackage package;
    package.command = static_cast<uint8_t>(ADC_COMMANDS::ADC_CMD_SET_MODE);    // Command
    package.syncMode = static_cast<uint8_t>(sync_mode);                        // Sync Mode Byte
    package.adcMode = static_cast<uint8_t>(adc_mode);                          // ADC Mode Byte
    package.sampleNum = static_cast<uint8_t>(samp_num);
    package.averNum = static_cast<uint8_t>(aver_num);
    package.averMode = static_cast<uint8_t>(aver_mode);
    for (int i = 0; i < 2; ++i) {
        m_server->SendData(package);
        if(m_server->ReadWithTimeout(boost::posix_time::seconds(4)))
        {
            std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
            delete p.first;
            std::cout << "AdcSetMode" << std::endl;
            return true;
        }
        std::cout << m_server->GetErrorsMessage() << std::endl;
        std::cout << "AdcSetMode Send to ADC = " << i << std::endl;
    }
    std::cout << "No AdcSetMode" << std::endl;
    return false;
}

void AdcController::StartImCMD()
{
    UART_UWB::IM_Start im_Start;
    m_server->SendData(im_Start);
    if(m_server->ReadWithTimeout(boost::posix_time::seconds(2)))
    {
        std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
        delete p.first;
        std::cout << "Setup NORMAL" << std::endl;
        return;
    }
    std::cout << "NO NORMAL" << std::endl;
    //LOG_ERROR << "Error Imitator";
}

void AdcController::SetImitationMode(int x, int y, int v, bool isOnenRun)
{
    int D = static_cast<int>(std::round(static_cast<double>(std::sqrt(x*x + y*y)) / 0.0075) * 2);

    int tAntenna = y ?
                static_cast<int>(std::round(((0.290 * x)/static_cast<double>(y))) / 0.0075)
              : 0;

    int8_t vSend = static_cast<int8_t>(std::round(static_cast<double>(v) / 10));

    UART_UWB::IM_CMD im_CMD;
    im_CMD.d = static_cast<uint16_t>(D);
    im_CMD.t = static_cast<int8_t>(tAntenna);
    im_CMD.v = vSend;

    im_CMD.start = isOnenRun ? im_CMD.ONE_RUN : im_CMD.CIRCLE_RUN;

    uint8_t* data = reinterpret_cast<uint8_t*>(&im_CMD);
    data = data + 2;
    im_CMD.crc = GetCRC_CMD(data, im_CMD.size - 1);
    m_server->SendData(im_CMD);
    if(m_server->ReadWithTimeout(boost::posix_time::seconds(2)))
    {
        std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
        delete p.first;
        std::cout << "Imitator setup! " << std::endl;
        return;
    }
    std::cout << "Imitator no answer! " << std::endl;
}

void AdcController::SetNormalMode()
{
    StartImCMD();
}

ConnectProtocol::STATUS_HARDWARE AdcController::GetStatusHardware()
{
    UART_UWB::CMD_HARDWARE cmdHARDWARE;
    m_server->SendData(cmdHARDWARE);
    if(m_server->ReadWithTimeout(boost::posix_time::seconds(3)))
    {
        //        UART_UWB::CMD_HARDWARE_answer answer = m_server->GetData<UART_UWB::CMD_HARDWARE_answer>();
        auto ptrData = m_server->GetData_uint8();
        UART_UWB::CMD_HARDWARE_answer answer = *reinterpret_cast<UART_UWB::CMD_HARDWARE_answer*>(ptrData.first);
        ConnectProtocol::STATUS_HARDWARE sh;
        sh.adc_Status = ConnectProtocol::ADCStatus::WORK;
        sh.reciverStatus = static_cast<ConnectProtocol::ReciverStatus>(answer.rec);
        sh.synchronizationStatus = static_cast<ConnectProtocol::SynchronizationStatus>(answer.sync);
        delete ptrData.first;
        return sh;
    }
    std::cout << m_server->GetErrorsMessage() << std::endl;
    std::cout << "NOT Read status" << std::endl;
    return ConnectProtocol::STATUS_HARDWARE();
}

bool AdcController::StartSync()
{
    UART_UWB::EnableSyncCMD cmdStart;
    cmdStart.SetEnable();
    m_server->SendData(cmdStart);
    if(m_server->ReadWithTimeout(boost::posix_time::seconds(1)))
    {
        std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
        delete p.first;
        std::cout << "Start Sync" << std::endl;
        return true;
    }
    std::cout << m_server->GetErrorsMessage() << std::endl;
    std::cout << "Start no Sync" << std::endl;
    return false;
}

bool AdcController::StopSync()
{
    UART_UWB::EnableSyncCMD cmdStop;
    cmdStop.SetDisable();
    m_server->SendData(cmdStop);
    if(m_server->ReadWithTimeout(boost::posix_time::seconds(1)))
    {
        std::pair<uint8_t*, size_t> p = m_server->GetData_uint8();
        delete p.first;
        std::cout << "StopSync" << std::endl;
        return true;
    }
    std::cout << "No StopSync" << std::endl;
    std::cout << m_server->GetErrorsMessage() << std::endl;

    return false;
}

void AdcController::StartThread()
{
    StopThread();
    m_server = new NetworkUWB::SenderUDP(m_ip, m_port);
    m_server->Bind(m_port);
}

void AdcController::StopThread()
{
    if(m_server){
        delete m_server;
        m_server = nullptr;
    }
}

uint8_t GetCRC_CMD(uint8_t *data, uint8_t size)
{
    uint8_t s = 0;
    for (uint8_t i = 0; i < size; ++i)
        s += *data++;
    return s;
}
