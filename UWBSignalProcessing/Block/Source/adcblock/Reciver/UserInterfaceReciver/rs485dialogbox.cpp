#include <QGridLayout>
#include <cmath>
#include <QPushButton>
#include "rs485dialogbox.h"
#include "ui_rs485dialogbox.h"
#include "controlword.h"

ReciverUWBDialog::ReciverUWBDialog(QWidget *wparent):
    QDialog(wparent), m_ui(new Ui::RS485DialogBox)
{
    m_ptreeCommandsWriter = new PTreeCommandsWriter;
    QWidget* m_layout = new QWidget;
    m_ui->setupUi(m_layout);
    //    создание сетки
    QVBoxLayout* vl = new QVBoxLayout;
    vl->addWidget(m_layout);
    setLayout(vl);

    MakeConnections();
    MakeConnectionsSlot();

    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
    m_timerSender = new QTimer(this);
    connect(m_timerSender, SIGNAL(timeout()), this, SLOT(ExecuteCommand()));
    InitValueData();
    ///Reboot
    WriteCommandReboot();
    WriteCommandRebootSynchronition();
}

ReciverUWBDialog::~ReciverUWBDialog()
{
    delete m_ptreeCommandsWriter;
    delete m_ui;
}

void ReciverUWBDialog::WriteCommandSyncronizationTune()
{
    m_ptreeCommandsWriter->AddName("SyncronizationTune");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.syncronizationTune);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandSyncronizationTune();
}

void ReciverUWBDialog::WriteCommandBandWidthTune()
{
    m_ptreeCommandsWriter->AddName("BandWidthTune");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.bandWidthTune);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandBandWidthTune();
}

void ReciverUWBDialog::WriteCommandDMAmplifierTune()
{
    m_ptreeCommandsWriter->AddName("DMAmplifierTune");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.dm_amplifierTune);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandDMAmplifierTune();
}

void ReciverUWBDialog::WriteCommandUSP1AmplifierTune()
{
    m_ptreeCommandsWriter->AddName("USP1AmplifierTune");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.USP1AmplifierTune);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandUSP1AmplifierTune();
}

void ReciverUWBDialog::WriteCommandUSP2AmplifierTune()
{
    m_ptreeCommandsWriter->AddName("USP2AmplifierTune");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.USP2AmplifierTune);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandUSP2AmplifierTune();
}

void ReciverUWBDialog::WriteCommandDMAmplifierTune_2()
{
    m_ptreeCommandsWriter->AddName("DMAmplifierTune_2");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.dm_amplifierTune_2);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandDMAmplifierTune_2();
}

void ReciverUWBDialog::WriteCommandUSP1AmplifierTune_2()
{
    m_ptreeCommandsWriter->AddName("USP1AmplifierTune_2");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.USP1AmplifierTune_2);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandUSP1AmplifierTune_2();
}

void ReciverUWBDialog::WriteCommandUSP2AmplifierTune_2()
{
    m_ptreeCommandsWriter->AddName("USP2AmplifierTune_2");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.USP2AmplifierTune_2);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandUSP2AmplifierTune_2();
}

void ReciverUWBDialog::WriteCommandDMSynthesizerTuneFast()
{
    m_ptreeCommandsWriter->AddName("DMSynthesizerTune");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(m_stateValueData.DMSynthesizerTune);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandDMSynthesizerTuneFast();
}

void ReciverUWBDialog::WriteCommandReboot()
{
    m_ptreeCommandsWriter->AddName("RebootCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandReboot();
}

void ReciverUWBDialog::WriteCommandSave()
{
    m_ptreeCommandsWriter->AddName("SaveCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandSave();
}

void ReciverUWBDialog::TimerSenderStop()
{
    m_timerSender->stop();
}

void ReciverUWBDialog::ExecuteCommand()
{
    m_ptreeCommandsWriter->Clear();

    InitValueData();
    (this->*m_slotExecute)();
    TimerSenderStop();

    LoudValueDataToUI();
}

void ReciverUWBDialog::ReadCommandSyncronizationTune()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.syncronizationTune = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "Синхронизация: " + QString::number(m_stateValueData.syncronizationTune, 16)
                    + " (1 байта) ";

            WriteStatus(statusStr);
        }
    }

}

void ReciverUWBDialog::ReadCommandBandWidthTune()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.bandWidthTune = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "Полоса: " + QString::number(m_stateValueData.bandWidthTune, 16)
                    + " (1 байта) ";

            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandDMAmplifierTune()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.dm_amplifierTune = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "DM: " + QString::number(m_stateValueData.dm_amplifierTune, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandUSP1AmplifierTune()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.USP1AmplifierTune = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "USP1: " + QString::number(m_stateValueData.USP1AmplifierTune, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandUSP2AmplifierTune()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.USP2AmplifierTune = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "USP2: " + QString::number(m_stateValueData.USP2AmplifierTune, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandDMAmplifierTune_2()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.dm_amplifierTune_2 = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "DM_2: " + QString::number(m_stateValueData.dm_amplifierTune_2, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandUSP1AmplifierTune_2()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.USP1AmplifierTune_2 = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "USP1_2: " + QString::number(m_stateValueData.USP1AmplifierTune_2, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandUSP2AmplifierTune_2()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.USP2AmplifierTune_2 = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "USP2_2: " + QString::number(m_stateValueData.USP2AmplifierTune_2, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandDMSynthesizerTuneFast()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint32_t")
        {
            m_stateValueData.DMSynthesizerTune = m_ptreeCommandsWriter->ReadParameterValueUINT32_T(data);
            QString statusStr = "DMSynt: " + QString::number(m_stateValueData.DMSynthesizerTune, 16)
                    + " (4 байта) ";
            WriteStatus(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandReboot()
{
    if(!ReadAnswer())
        return;
    WriteStatus("Reboot: выполнена");
}

void ReciverUWBDialog::ReadCommandSave()
{
    if(!ReadAnswer())
        return;
    WriteStatus("Save: выполнена");
}

void ReciverUWBDialog::ReadCommandStatusReciver()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.reciverStatus = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "Status: " + QString::number(m_stateValueData.reciverStatus, 16)
                    + " (1 байта) ";
            WriteStatus(statusStr);
        }
    }
}

uint32_t ReciverUWBDialog::ReadCommandGetSettingReciver()
{
    m_ptreeCommandsWriter->Clear();
    uint32_t dataVal = 0xffffffff;
    if(!ReadAnswer())
        return dataVal;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint32_t")
            dataVal = m_ptreeCommandsWriter->ReadParameterValueUINT32_T(data);

    return dataVal;
}

void ReciverUWBDialog::ReadCommandStatusSynchronition()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint8_t")
        {
            m_stateValueData.reciverStatusSynchronition = m_ptreeCommandsWriter->ReadParameterValueUINT8_T(data);
            QString statusStr = "Status: " + QString::number(m_stateValueData.reciverStatusSynchronition, 16)
                    + " (1 байта) ";
            WriteStatusSynchronition(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandAccumSynchronition()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint32_t")
        {
            uint32_t val = m_ptreeCommandsWriter->ReadParameterValueUINT32_T(data);
            m_stateValueData.accumSynchronition = *reinterpret_cast<ValueData::Accum*>(&val);
            QString statusStr = "AccumSync: " + QString::number(val, 16)
                    + " (4 байта) ";
            WriteStatusSynchronition(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandControlScanSynchronition()
{
    if(!ReadAnswer())
        return;

    std::list<std::string> answer = m_ptreeCommandsWriter->GetParametersAnswer();
    for(auto& data : answer)
    {
        if(m_ptreeCommandsWriter->ReadTypeParameter(data) == "uint32_t")
        {
            uint32_t val = m_ptreeCommandsWriter->ReadParameterValueUINT32_T(data);
            m_stateValueData.controlScan = *reinterpret_cast<ValueData::ControlScan*>(&val);
            QString statusStr = "ControlScan: " + QString::number(val, 16)
                    + " (4 байта) ";
            WriteStatusSynchronition(statusStr);
        }
    }
}

void ReciverUWBDialog::ReadCommandRebootSynchronition()
{
    if(!ReadAnswer())
        return;
    WriteStatusSynchronition("Reboot: выполнена");
}

void ReciverUWBDialog::ReadCommandSaveSynchronition()
{
    if(!ReadAnswer())
        return;
    WriteStatusSynchronition("Save: выполнена");
}

void ReciverUWBDialog::InitValueData()
{
    m_stateValueData.bandWidthTune = m_ui->radioButton_500MHz->isChecked();
    m_stateValueData.syncronizationTune = m_ui->radioButton_InSync->isChecked();
    m_stateValueData.dm_amplifierTune = static_cast<uint8_t>(m_ui->dialAmpDm->value());
    m_stateValueData.USP1AmplifierTune = static_cast<uint8_t>(m_ui->dialAmpUsp1->value());
    m_stateValueData.USP2AmplifierTune =  static_cast<uint8_t>(m_ui->dialAmpUsp2->value());

    m_stateValueData.dm_amplifierTune_2 = static_cast<uint8_t>(m_ui->dialAmpDm_2->value());
    m_stateValueData.USP1AmplifierTune_2 = static_cast<uint8_t>(m_ui->dialAmpUsp1_2->value());
    m_stateValueData.USP2AmplifierTune_2 =  static_cast<uint8_t>(m_ui->dialAmpUsp2_2->value());

    m_stateValueData.DMSynthesizerTune = (static_cast<uint32_t>(m_ui->dialSintDmFast->value()) << 24) +
            (static_cast<uint32_t>(m_ui->dialSintDmSlow->value()) << 16);

    m_stateValueData.controlScan.isHandScan = m_ui->isHandScane->isChecked();
    m_stateValueData.controlScan.azimut = static_cast<uint8_t>(m_ui->azimutScane->value());

    m_stateValueData.accumSynchronition.isTwoGain = static_cast<uint8_t>(m_ui->twoGain->isChecked());
    m_stateValueData.accumSynchronition.countAccum = static_cast<uint8_t>(m_ui->accumOneElement->currentIndex());
    m_stateValueData.accumSynchronition.countReciverAntenn = static_cast<uint8_t>(m_ui->countReciversAntenn->currentIndex());
    m_stateValueData.accumSynchronition.countTransmiter= static_cast<uint8_t>(m_ui->countTransmiters->currentIndex());
}

void ReciverUWBDialog::SetupStatus()
{
    Status reciverStatus = *reinterpret_cast<Status*>(&m_stateValueData.reciverStatus);
    QString color;
    if(reciverStatus.statusReciver == 1)
        color = "red";
    if(reciverStatus.statusReciver == 2)
        color = "yellow";
    if(reciverStatus.statusReciver == 3)
        color = "gray";
    if(reciverStatus.statusReciver == 0)
        color = "green";

    m_ui->StatusReciver->setStyleSheet(QString("background-color: %1;").arg(color));
}

void ReciverUWBDialog::SetupStatusSynchronition()
{
    uint8_t status = m_stateValueData.reciverStatusSynchronition & 0x03;
    QString color;
    if(status == 0)
        color = "red";
    else if(status == 1)
        color = "yellow";
    else if(status == 2)
        color = "gray";
    else if(status == 3)
        color = "green";

    m_ui->statusSynchronition->setStyleSheet(QString("background-color: %1;").arg(color));
}

void ReciverUWBDialog::LoudValueDataToUI()
{
    m_ui->radioButton_500MHz->setChecked(m_stateValueData.bandWidthTune);
    TimerSenderStop();
    m_ui->radioButton_InSync->setChecked(m_stateValueData.syncronizationTune);
    TimerSenderStop();
    m_ui->dialAmpDm->setValue(m_stateValueData.dm_amplifierTune);
    TimerSenderStop();
    m_ui->dialAmpUsp1->setValue(m_stateValueData.USP1AmplifierTune);
    TimerSenderStop();
    m_ui->dialAmpUsp2->setValue(m_stateValueData.USP2AmplifierTune);
    TimerSenderStop();

    m_ui->dialAmpDm_2->setValue(m_stateValueData.dm_amplifierTune_2);
    TimerSenderStop();
    m_ui->dialAmpUsp1_2->setValue(m_stateValueData.USP1AmplifierTune_2);
    TimerSenderStop();
    m_ui->dialAmpUsp2_2->setValue(m_stateValueData.USP2AmplifierTune_2);
    TimerSenderStop();

    m_ui->dialSintDmFast->setValue(static_cast<uint8_t>(m_stateValueData.DMSynthesizerTune >> 24));
    TimerSenderStop();
    m_ui->dialSintDmSlow->setValue(static_cast<uint8_t>(m_stateValueData.DMSynthesizerTune >> 16));
    TimerSenderStop();

    m_ui->isHandScane->setChecked(m_stateValueData.controlScan.isHandScan);
    TimerSenderStop();
    m_ui->azimutScane->setValue(m_stateValueData.controlScan.azimut);
    TimerSenderStop();
    m_ui->twoGain->setChecked(m_stateValueData.accumSynchronition.isTwoGain);
    TimerSenderStop();
    m_ui->accumOneElement->setCurrentIndex(m_stateValueData.accumSynchronition.countAccum);
    TimerSenderStop();
    m_ui->countReciversAntenn->setCurrentIndex(m_stateValueData.accumSynchronition.countReciverAntenn);
    TimerSenderStop();
    m_ui->countTransmiters->setCurrentIndex(m_stateValueData.accumSynchronition.countTransmiter);
    TimerSenderStop();

    SetupStatus();
    SetupStatusSynchronition();
}

bool ReciverUWBDialog::CheckNoError()
{
    if(m_ptreeCommandsWriter->IsNoError())
        return true;
    QString error = "Error send: " + QString::fromStdString(m_ptreeCommandsWriter->GetError());
    WriteStatus(error);
    return false;
}

bool ReciverUWBDialog::ReadAnswer()
{   
    m_ptreeCommandsWriter->GetAnswer();
    return CheckNoError();
}

void ReciverUWBDialog::WriteCommandStatusReciver()
{
    m_ptreeCommandsWriter->AddName("GetStatusCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandStatusReciver();
}

uint32_t ReciverUWBDialog::CreateRequestGetSetting(uint32_t data)
{
    m_ptreeCommandsWriter->Clear();
    m_ptreeCommandsWriter->AddName("GetSettingCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddress());
    m_ptreeCommandsWriter->AddParameterCommand(data);
    m_ptreeCommandsWriter->WriteCommand();
    return ReadCommandGetSettingReciver();
}

void ReciverUWBDialog::WriteCommandGetSettingReciver()
{
    m_stateValueData.dm_amplifierTune = CreateRequestGetSetting(COMMANDS::DM_AMP);
    m_stateValueData.USP1AmplifierTune = CreateRequestGetSetting(COMMANDS::USP1Amp);
    m_stateValueData.USP2AmplifierTune = CreateRequestGetSetting(COMMANDS::USP2Amp);

    m_stateValueData.dm_amplifierTune_2 = CreateRequestGetSetting(COMMANDS::DM_AMP_2);
    m_stateValueData.USP1AmplifierTune_2 = CreateRequestGetSetting(COMMANDS::USP1Amp_2);
    m_stateValueData.USP2AmplifierTune_2 = CreateRequestGetSetting(COMMANDS::USP2Amp_2);

    m_stateValueData.DMSynthesizerTune = CreateRequestGetSetting(COMMANDS::DMSynt);
    m_stateValueData.bandWidthTune = CreateRequestGetSetting(COMMANDS::BAND);
    m_stateValueData.syncronizationTune = CreateRequestGetSetting(COMMANDS::SYNCRONIZATION);
}

void ReciverUWBDialog::WriteCommandStatusSynchronition()
{
    m_ptreeCommandsWriter->AddName("GetStatusSynchronitionCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddressSynchronition());
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandStatusSynchronition();
}

void ReciverUWBDialog::WriteCommandAccumSynchronition()
{
    m_ptreeCommandsWriter->AddName("AccumSynchronition");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddressSynchronition());
    uint32_t val = *(reinterpret_cast<uint32_t*>(&m_stateValueData.accumSynchronition));
    m_ptreeCommandsWriter->AddParameterCommand(val);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandAccumSynchronition();
}

void ReciverUWBDialog::WriteCommandControlScanSynchronition()
{
    m_ptreeCommandsWriter->AddName("ControlScanSynchronition");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddressSynchronition());
    uint32_t val = *(reinterpret_cast<uint32_t*>(&m_stateValueData.controlScan));
    m_ptreeCommandsWriter->AddParameterCommand(val);
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandControlScanSynchronition();
}

void ReciverUWBDialog::WriteCommandRebootSynchronition()
{
    m_ptreeCommandsWriter->AddName("RebootSynchronitionCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddressSynchronition());
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandRebootSynchronition();
}

void ReciverUWBDialog::WriteCommandSaveSynchronition()
{
    m_ptreeCommandsWriter->AddName("SaveSynchronitionCommand");
    m_ptreeCommandsWriter->AddParameterCommand("address", GetAddressSynchronition());
    m_ptreeCommandsWriter->WriteCommand();
    ReadCommandSaveSynchronition();
}

uint8_t ReciverUWBDialog::GetAddress() const
{
    bool ok;
    uint8_t address = m_ui->deviceAdressReciver->text().toInt(&ok, 16);
    if(ok)
        return address;

    return 0;
}

uint8_t ReciverUWBDialog::GetAddressSynchronition() const
{
    bool ok;
    uint8_t address = m_ui->deviceAdressSynchronition->text().toInt(&ok, 16);
    if(ok)
        return address;

    return 0;
}

void ReciverUWBDialog::MakeConnectionsSlot()
{
    m_slots[m_ui->dialAmpDm] = &ReciverUWBDialog::WriteCommandDMAmplifierTune;
    m_slots[m_ui->dialAmpUsp2] = &ReciverUWBDialog::WriteCommandUSP2AmplifierTune;
    m_slots[m_ui->dialAmpUsp1] = &ReciverUWBDialog::WriteCommandUSP1AmplifierTune;

    m_slots[m_ui->dialAmpDm_2] = &ReciverUWBDialog::WriteCommandDMAmplifierTune_2;
    m_slots[m_ui->dialAmpUsp2_2] = &ReciverUWBDialog::WriteCommandUSP2AmplifierTune_2;
    m_slots[m_ui->dialAmpUsp1_2] = &ReciverUWBDialog::WriteCommandUSP1AmplifierTune_2;

    m_slots[m_ui->radioButton_500MHz] = &ReciverUWBDialog::WriteCommandBandWidthTune;
    m_slots[m_ui->radioButton_250MHz] = &ReciverUWBDialog::WriteCommandBandWidthTune;
    m_slots[m_ui->radioButton_InSync] = &ReciverUWBDialog::WriteCommandSyncronizationTune;
    m_slots[m_ui->radioButton_OutSync] = &ReciverUWBDialog::WriteCommandSyncronizationTune;
    m_slots[m_ui->dialSintDmFast] = &ReciverUWBDialog::WriteCommandDMSynthesizerTuneFast;
    m_slots[m_ui->dialSintDmSlow] = &ReciverUWBDialog::WriteCommandDMSynthesizerTuneFast;
    m_slots[m_ui->ResetReciver] = &ReciverUWBDialog::WriteCommandReboot;
    m_slots[m_ui->SaveReciver] = &ReciverUWBDialog::WriteCommandSave;
    m_slots[m_ui->StatusReciver] = &ReciverUWBDialog::WriteCommandStatusReciver;
    m_slots[m_ui->GetSettingReciver] = &ReciverUWBDialog::WriteCommandGetSettingReciver;

    m_slots[m_ui->statusSynchronition] = &ReciverUWBDialog::WriteCommandStatusSynchronition;

    m_slots[m_ui->accumOneElement] = &ReciverUWBDialog::WriteCommandAccumSynchronition;
    m_slots[m_ui->countTransmiters] = &ReciverUWBDialog::WriteCommandAccumSynchronition;
    m_slots[m_ui->countReciversAntenn] = &ReciverUWBDialog::WriteCommandAccumSynchronition;
    m_slots[m_ui->oneGain] = &ReciverUWBDialog::WriteCommandAccumSynchronition;
    m_slots[m_ui->twoGain] = &ReciverUWBDialog::WriteCommandAccumSynchronition;

    m_slots[m_ui->isHandScane] = &ReciverUWBDialog::WriteCommandControlScanSynchronition;
    m_slots[m_ui->azimutScane] = &ReciverUWBDialog::WriteCommandControlScanSynchronition;

    m_slots[m_ui->ResetSynchronition] = &ReciverUWBDialog::WriteCommandRebootSynchronition;
    m_slots[m_ui->SaveSynchronition] = &ReciverUWBDialog::WriteCommandSaveSynchronition;

    connect(m_ui->dialAmpDm, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->dialAmpUsp1, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->dialAmpUsp2, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));

    connect(m_ui->dialAmpDm_2, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->dialAmpUsp1_2, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->dialAmpUsp2_2, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));

    connect(m_ui->radioButton_500MHz, SIGNAL(clicked()), this, SLOT(SendCommand()));
    connect(m_ui->radioButton_250MHz, SIGNAL(clicked()), this, SLOT(SendCommand()));
    connect(m_ui->radioButton_InSync, SIGNAL(clicked()), this, SLOT(SendCommand()));
    connect(m_ui->radioButton_OutSync, SIGNAL(clicked()), this, SLOT(SendCommand()));
    connect(m_ui->dialSintDmFast, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->dialSintDmSlow, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->ResetReciver, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));
    connect(m_ui->SaveReciver, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));
    connect(m_ui->StatusReciver, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));
    connect(m_ui->GetSettingReciver, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));

    connect(m_ui->statusSynchronition, SIGNAL(clicked()), this, SLOT(SendCommand()));

    connect(m_ui->accumOneElement, SIGNAL(currentIndexChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->countTransmiters, SIGNAL(currentIndexChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->countReciversAntenn, SIGNAL(currentIndexChanged(int)), this, SLOT(SendCommand()));
    connect(m_ui->oneGain, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));
    connect(m_ui->twoGain, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));

    connect(m_ui->isHandScane, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));
    connect(m_ui->azimutScane, SIGNAL(valueChanged(int)), this, SLOT(SendCommand()));

    connect(m_ui->ResetSynchronition, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));
    connect(m_ui->SaveSynchronition, SIGNAL(clicked(bool)), this, SLOT(SendCommand()));


}

bool ReciverUWBDialog::GetAnswer()
{
    return m_ptreeCommandsWriter->GetAnswer();
}

void ReciverUWBDialog::SendCommand()
{
    if(m_slots.contains(sender()))
    {
        m_slotExecute = m_slots[sender()];
        m_timerSender->start(300);
    }
}

double ReciverUWBDialog::ChangeAmp(int val)
{
    double doubleVal = ((double)(val - 27)) / 2;
    return doubleVal;
}

void ReciverUWBDialog::ChangeDm()
{
    double fast = m_ui->dialSintDmFast->value();
    double slow = m_ui->dialSintDmSlow->value();
    double valdouble = (fast  + slow / 256.0) * 20;
    m_ui->lcdSintDm->display(valdouble);
}

void ReciverUWBDialog::ChangeAmpDm(int val)
{
    m_ui->lcdAmpDm->display(ChangeAmp(val));
}

void ReciverUWBDialog::ChangeAmpUsp1(int val)
{
    m_ui->lcdAmpUsp1->display(ChangeAmp(val));
}

void ReciverUWBDialog::ChangeAmpUsp2(int val)
{
    m_ui->lcdAmpUsp2->display(ChangeAmp(val));
}

void ReciverUWBDialog::ChangeAmpDm_2(int val)
{
    m_ui->lcdAmpDm_2->display(ChangeAmp(val));
}

void ReciverUWBDialog::ChangeAmpUsp1_2(int val)
{
    m_ui->lcdAmpUsp1_2->display(ChangeAmp(val));
}

void ReciverUWBDialog::ChangeAmpUsp2_2(int val)
{
    m_ui->lcdAmpUsp2_2->display(ChangeAmp(val));
}

void ReciverUWBDialog::MakeConnections()
{
    connect(m_ui->dialAmpDm, SIGNAL(valueChanged(int)), this, SLOT(ChangeAmpDm(int)));
    connect(m_ui->dialAmpUsp1, SIGNAL(valueChanged(int)), this, SLOT(ChangeAmpUsp1(int)));
    connect(m_ui->dialAmpUsp2, SIGNAL(valueChanged(int)), this, SLOT(ChangeAmpUsp2(int)));

    connect(m_ui->dialAmpDm_2, SIGNAL(valueChanged(int)), this, SLOT(ChangeAmpDm_2(int)));
    connect(m_ui->dialAmpUsp1_2, SIGNAL(valueChanged(int)), this, SLOT(ChangeAmpUsp1_2(int)));
    connect(m_ui->dialAmpUsp2_2, SIGNAL(valueChanged(int)), this, SLOT(ChangeAmpUsp2_2(int)));

    connect(m_ui->dialSintDmFast, SIGNAL(valueChanged(int)), this, SLOT(ChangeDm()));
    connect(m_ui->dialSintDmSlow, SIGNAL(valueChanged(int)), this, SLOT(ChangeDm()));
}

void ReciverUWBDialog::WriteStatus(const QString& status)
{
    m_ui->lableStatus->setText(status);
}

void ReciverUWBDialog::WriteStatusSynchronition(const QString& status)
{
    m_ui->lableStatusSynchronition->setText(status);
}
