#include "adcdialogbox.h"
#include "ui_adcdialogbox.h"
#include <QSettings>
#include "rs485dialogbox.h"

ADCServerDialogBox::ADCServerDialogBox(ADCModule* parent, QWidget* wparent)
    : AbstractDialogCommand(wparent),
      ui(new Ui::UDPServerDialogBox), m_module(parent), m_senderToUDP(m_IpIndex.toStdString(), 0x4060)
{
    ///TODO проверить
    m_senderToUDP.Bind(0x4020);
    InitOldValue();
    // для добавления виждета
    QWidget* m_layout = new QWidget;
    ui->setupUi(m_layout);
    ReadDelayADC();
    m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(m_layout, 0, 0);
    SetTopLayout(m_gridLayout);

    MakeConnections();
    // параметры по умолчанию
    ui->signalFrequency->setValue(1);
    ui->signalLengthDial->setValue(14);
    ui->countAzimut->setVisible(false);

    // блокировка кнопок Применить и ОК
    SetEnabledApplyButton(false);
    SetEnabledOkButton(false);

}

ADCServerDialogBox::~ADCServerDialogBox()
{
    WriteDelayADC();
}

void ADCServerDialogBox::WriteDelayADC()
{
    QSettings settings("Radar MMS", "UWBSignalProcessing");
    settings.beginGroup("ADCdelay");
    settings.setValue("delay_1", ui->delayADC_1->text());
    settings.setValue("delay_2", ui->delayADC_2->text());
    settings.setValue("delay_3", ui->delayADC_3->text());
    settings.setValue("delay_4", ui->delayADC_4->text());
    settings.setValue("delay_5", ui->delayADC_5->text());
    settings.setValue("CountBoards", ui->CountBoards->currentIndex());
    settings.endGroup();
}

void ADCServerDialogBox::ReadDelayADC()
{
    QSettings settings("Radar MMS", "UWBSignalProcessing");
    settings.beginGroup("ADCdelay");
    if(!settings.allKeys().isEmpty())
    {
        ui->delayADC_1->setText(settings.value("delay_1").toString());
        ui->delayADC_2->setText(settings.value("delay_2").toString());
        ui->delayADC_3->setText(settings.value("delay_3").toString());
        ui->delayADC_4->setText(settings.value("delay_4").toString());
        ui->delayADC_5->setText(settings.value("delay_5").toString());
        ui->CountBoards->setCurrentIndex(settings.value("CountBoards").toInt());

        m_delayADCBoard.clear();
        bool ok;
        m_delayADCBoard.push_back(settings.value("delay_1").toString().toInt(&ok, 16));
        m_delayADCBoard.push_back(settings.value("delay_2").toString().toInt(&ok, 16));
        m_delayADCBoard.push_back(settings.value("delay_3").toString().toInt(&ok, 16));
        m_delayADCBoard.push_back(settings.value("delay_4").toString().toInt(&ok, 16));
        m_delayADCBoard.push_back(settings.value("delay_5").toString().toInt(&ok, 16));
    }
    settings.endGroup();
}

void ADCServerDialogBox::MakeConnections()
{
    connect(ui->workMode2RadioButton, SIGNAL(toggled(bool)), ui->adcNumberComboBox, SLOT(setEnabled(bool)));
    connect(ui->signalLengthDial, SIGNAL(valueChanged(int)), this, SLOT(ChangedSignalLength(int)));
    connect(ui->adcDelayDial, SIGNAL(valueChanged(int)), this, SLOT(ChangedADCDelay(int)));

    connect(ui->signalFrequency, SIGNAL(valueChanged(double)), this, SLOT(ChangedSignalFreaq(double)));

    connect(ui->ipSelector, SIGNAL(activated(QString)), this, SLOT(ShowDialogMassage(QString)));
    connect(ui->workMode2RadioButton, SIGNAL(toggled(bool)), this, SLOT(ChangedWorkMode(bool)));
    connect(ui->findADCPushButton, SIGNAL(clicked()), this, SLOT(CheckIPs()));

    connect(ui->delayADC_1, SIGNAL(editingFinished()), this, SLOT(SetupDelayADC()));
    ui->delayADC_1->setProperty("index", 0);
    connect(ui->delayADC_2, SIGNAL(editingFinished()), this, SLOT(SetupDelayADC()));
    ui->delayADC_2->setProperty("index", 1);
    connect(ui->delayADC_3, SIGNAL(editingFinished()), this, SLOT(SetupDelayADC()));
    ui->delayADC_3->setProperty("index", 2);
    connect(ui->delayADC_4, SIGNAL(editingFinished()), this, SLOT(SetupDelayADC()));
    ui->delayADC_4->setProperty("index", 3);
    connect(ui->delayADC_5, SIGNAL(editingFinished()), this, SLOT(SetupDelayADC()));
    ui->delayADC_5->setProperty("index", 4);

    ReciverUWBDialog* reciverUWBDialog = new ReciverUWBDialog(this);
    connect(ui->ModuleControl, SIGNAL(clicked(bool)), reciverUWBDialog, SLOT(show()));

    connect(m_module, SIGNAL(StartADC()), this, SLOT(SwitchADCOn()));
    connect(m_module, SIGNAL(StopADC()), this, SLOT(SwitchADCOff()));

}

void ADCServerDialogBox::WriteSettingToADC()
{
    if(ui->countAzimut->isVisible())
        SetCountAzimut(ui->countAzimut->value());

    m_module->SetAutoTrend(ui->autoTrend->isChecked());
    SetADCControlReg();
    SetUFOSAllControlReg();

    SetSignalPeriod();
    SetADCDelay();
    SetSignalLength();
    SetUFOSAllRegimeReg();
    SetUFOSAllADCNumber();
    SetRegAF();
    SetAccumMode();

    if(!CheckStatus())
        ShowMassageResetADC();
}

bool ADCServerDialogBox::SetAccumMode()
{
    ADC_ALL_ACCUM adc_ALL_ACCUM;
    adc_ALL_ACCUM.RX_MODE = m_currentValues.registerAF.RX_MODE;
    adc_ALL_ACCUM.DATA_X = 0;

    WriteRegisterADC((uint8_t)ADC_ALL_ACCUM::ADDR::ADDRESS, adc_ALL_ACCUM);
    GetStatus();
    return true;
}

void ADCServerDialogBox::SetValuesUI()
{
    double signalFrequency = (m_currentValues.signalPeriod_L.TX_PERIOD + (m_currentValues.signalPeriod_H.TX_PERIOD << 8) + 1) * 0.08;
    ui->signalFrequency->setValue(signalFrequency);
    ui->adcDelayDial->setValue(m_currentValues.adcDelay.RX_DELAY);
    ui->signalLengthDial->setValue(m_currentValues.signalLength.LengthTX);

    ui->workMode0RadioButton->setChecked(!(m_currentValues.ufosAllRegimeReg.WORK_MODE));
    ui->workMode2RadioButton->setChecked(m_currentValues.ufosAllRegimeReg.WORK_MODE & 0x2);
    ui->continuousRadioButton->setChecked(m_currentValues.ufosAllRegimeReg.SDRAM_MODE);
    ui->strobRadioButton->setChecked(!(m_currentValues.ufosAllRegimeReg.SDRAM_MODE));

    ui->outerClkRadioButton->setChecked(m_currentValues.ufosAllControlReg.EXT_CLK_EN);
    ui->innerClkRadioButton->setChecked(!(m_currentValues.ufosAllControlReg.EXT_CLK_EN));
    ui->nsModeCheckBox->setChecked(m_currentValues.ufosAllControlReg.NS1_EN);

    ui->lvdsCheckBox->setChecked(m_currentValues.adcControlReg.OUTS_EN);
    ui->GHzCheckBox->setChecked(m_currentValues.adcControlReg.G1_EN);
    ui->testDataCheckBox->setChecked(m_currentValues.adcControlReg.TEST_DATA);
    ui->innerAzimuthRadioButton->setChecked(m_currentValues.adcControlReg.TEST_BEAR);
    ui->outerAzimuthRadioButton->setChecked(!(m_currentValues.adcControlReg.TEST_BEAR));

    ui->adcNumberComboBox->setCurrentIndex(m_currentValues.ufosAllADCNumberPosition.CountRX);
    ui->accumMode->setCurrentIndex(m_currentValues.registerAF.RX_MODE);
}

bool ADCServerDialogBox::SetSignalPeriod()
{
    uint16_t val = (ui->signalPeriodLcd->value() / 0.08) - 1;
    m_currentValues.signalPeriod_H.TX_PERIOD = val >> 8;
    m_senderToUDP.SendData(m_currentValues.signalPeriod_H);

    m_currentValues.signalPeriod_L.TX_PERIOD = val;
    m_senderToUDP.SendData(m_currentValues.signalPeriod_L);
    GetStatus();
    return true;
}

bool ADCServerDialogBox::SetADCDelay()
{
    m_currentValues.adcDelay.RX_DELAY = ui->adcDelayDial->value();
    // отпрвака сдвига отсчетов АЦП
    WriteRegisterADC((uint8_t)ADC_ALL_RegisterRX_DELAY::ADDR::ADDRESS, m_currentValues.adcDelay);
    GetStatus();
    return true;
}

bool ADCServerDialogBox::SetSignalLength()
{
    m_currentValues.signalLength.LengthTX = ui->signalLengthDial->value();
    m_senderToUDP.SendData(m_currentValues.signalLength);
    GetStatus();
    return true;
}

bool ADCServerDialogBox::SetUFOSAllRegimeReg()
{
    m_currentValues.ufosAllRegimeReg.ADC_MODE = 0;
    m_currentValues.ufosAllRegimeReg.DAC_MODE = 0;
    m_currentValues.ufosAllRegimeReg.DATA_X = 0;
    m_currentValues.ufosAllRegimeReg.WORK_MODE = ui->workMode2RadioButton->isChecked();
    m_currentValues.ufosAllRegimeReg.SDRAM_MODE = ui->continuousRadioButton->isChecked();
    WriteRegisterADC((uint8_t)ADC_ALL_RegisterWorkMode::ADDR::ADDRESS, m_currentValues.ufosAllRegimeReg);
    GetStatus();
    return true;
}

bool ADCServerDialogBox::SetUFOSAllControlReg()
{
    m_currentValues.ufosAllControlReg.TX_EN = m_switchUFOSOnOff;
    m_currentValues.ufosAllControlReg.EXT_CLK_EN = ui->outerClkRadioButton->isChecked();
    m_currentValues.ufosAllControlReg.NS1_EN = ui->nsModeCheckBox->isChecked();
    m_currentValues.ufosAllControlReg.DATA_X = 0;
    WriteRegisterADC((uint8_t)ADC_ALL_RegisterControl::ADDR::ADDRESS, m_currentValues.ufosAllControlReg);
    GetStatus();
    return true;
}

void ADCServerDialogBox::GetStatus()
{
    m_statusADC.Reset();
    if(m_senderToUDP.ReadWithTimeout(boost::posix_time::millisec(10)))
    {
        m_statusADC =  m_senderToUDP.GetData<StatusADC>();
    }
}

void ADCServerDialogBox::SetCountAzimut(int azimut)
{
    m_module->SetCountAzimut(azimut);
}

bool ADCServerDialogBox::SetADCControlReg()
{
    m_currentValues.adcControlReg.TX_EN = m_switchADCOnOff;
    m_currentValues.adcControlReg.OUTS_EN = ui->lvdsCheckBox->isChecked();
    m_currentValues.adcControlReg.G1_EN = ui->GHzCheckBox->isChecked();
    m_currentValues.adcControlReg.G1_ADDR_SCLR = 0x00;
    m_currentValues.adcControlReg.TEST_DATA = ui->testDataCheckBox->isChecked();
    m_currentValues.adcControlReg.TEST_BEAR = ui->innerAzimuthRadioButton->isChecked();
    m_currentValues.adcControlReg.DATA_X = 0;
    m_senderToUDP.SendData(m_currentValues.adcControlReg);

    GetStatus();
    return true;
}

bool ADCServerDialogBox::SetUFOSAllADCNumber()
{
    m_currentValues.ufosAllADCNumberPosition.CountRX = (uint8_t)(ui->adcNumberComboBox->currentText().toUInt() / 32 - 1);
    WriteRegisterADC((uint8_t)ADC_ALL_RegisterCountRX::ADDR::ADDRESS, m_currentValues.ufosAllADCNumberPosition);
    GetStatus();
    return true;
}

bool ADCServerDialogBox::SetRegAF()
{
    uint8_t countBoard = std::pow(2, ui->CountBoards->currentIndex());
    m_module->SetCountBoards(countBoard);
    m_currentValues.registerAF.BRD_COUNT = ui->CountBoards->currentIndex();
    m_currentValues.registerAF.LED0 = 1;
    m_currentValues.registerAF.LED1 = 1;
    ///TODO для накопления при старте
    m_currentValues.registerAF.RX_MODE = ui->accumMode->currentIndex();
    m_senderToUDP.SendData(m_currentValues.registerAF);
    GetStatus();
    return true;
}

void ADCServerDialogBox::SendCommandAbstractModule()
{
    WriteSettingToADC();
}

void ADCServerDialogBox::ShowMassageResetADC()
{
    if(!m_dialogErrors.isVisible())
    {
        m_dialogErrors.setText("Предупреждение");
        m_dialogErrors.setInformativeText(QString("Перегрузите АЦП!!!"));
        m_dialogErrors.setStandardButtons(QMessageBox::Ok);
        m_dialogErrors.setDefaultButton(QMessageBox::Ok);
        m_dialogErrors.setIcon(QMessageBox::Warning);
        m_dialogErrors.exec();
    }
}

void ADCServerDialogBox::ChangedSignalLength(int value)
{
    ui->signalLengthLcdNumber->display((value + 1) * 20);
}

void ADCServerDialogBox::ChangedADCDelay(int value)
{
    ui->adcDelayLcdNumber->display(static_cast<double>(value * 128 + 4) * 0.15 * 4);
}

void ADCServerDialogBox::ChangedSignalFreaq(double value)
{
    double resPeriod = 1000.0 / (value);
    int resDiv = resPeriod / 0.08;
    resPeriod = 0.08 * resDiv;
    ui->signalPeriodLcd->display(resPeriod);
    ui->signalFrequencyLcd->display(1000.0 / resPeriod);
}

void ADCServerDialogBox::ChangedWorkMode(bool checked)
{
    if (!checked)
        ui->adcNumberComboBox->setCurrentIndex(0);
}

void ADCServerDialogBox::ShowDialogMassage(const QString& index)
{
    if(SetSignalPeriod() | SetADCDelay() | SetSignalLength() | SetUFOSAllRegimeReg() |
            SetUFOSAllControlReg() | SetADCControlReg() | SetUFOSAllADCNumber())
    {
        ///TODO посмотреть что это
        QMessageBox dialog;
        dialog.about(this, "Предупреждение!",
                     QString("Изменение настроек для IP-%1 было добавлено в очередь команд")
                     .arg(m_IpIndex));
    }

    m_IpIndex = index;

    SetValuesUI();
}

void ADCServerDialogBox::SetClockTune(uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0)
{
    RegisterCLOCKS_TUNE rcclocks_tune0;
    rcclocks_tune0.adress = static_cast<uint8_t>(RegisterCLOCKS_TUNE::ADDR::ADDRESS_TUNE0);
    rcclocks_tune0.CLOCKS_TUNE = byte0;
    m_senderToUDP.SendData(rcclocks_tune0);
    RegisterCLOCKS_TUNE rcclocks_tune1;
    rcclocks_tune1.adress = static_cast<uint8_t>(RegisterCLOCKS_TUNE::ADDR::ADDRESS_TUNE1);
    rcclocks_tune1.CLOCKS_TUNE = byte1;
    m_senderToUDP.SendData(rcclocks_tune1);
    RegisterCLOCKS_TUNE rcclocks_tune2;
    rcclocks_tune2.adress = static_cast<uint8_t>(RegisterCLOCKS_TUNE::ADDR::ADDRESS_TUNE2);
    rcclocks_tune2.CLOCKS_TUNE = byte2;
    m_senderToUDP.SendData(rcclocks_tune2);

    RegisterCLOCKS_TUNE rcclocks_tune3;
    rcclocks_tune3.adress = static_cast<uint8_t>(RegisterCLOCKS_TUNE::ADDR::ADDRESS_TUNE3);
    rcclocks_tune3.CLOCKS_TUNE = byte3;
    m_senderToUDP.SendData(rcclocks_tune3);
}

void ADCServerDialogBox::SetClockTune(uint32_t delay)
{
    SetClockTune((delay >> 24) & 0xFF, (delay >> 16) & 0xFF, (delay >> 8) & 0xFF, delay & 0xFF);
    GetStatus();
}

void ADCServerDialogBox::CheckIPs()
{
    ui->ipSelector->clear();
    ResetIPNameList();
    m_switchADCOnOff = false;
    m_switchUFOSOnOff = false;

    // перезагрузка
    SetClockTune(0x80000000);
    // проверка АЦП
    if(m_statusADC.clocksTune[3] == 0x80)
    {
        // рабочий режим
        SetClockTune(0x00000000);
        // установка задержек для всех плат
        SetupAllDelayADC();
        // установка внешней синхронизации
        SetClockTune(0x6800000E);

        ui->ipSelector->addItem(m_IpIndex);
        QHostAddress ip(m_IpIndex);
        InitADCiP((ip.toIPv4Address() - 0xC0A80065));

        m_IpIndex = ui->ipSelector->itemText(0);
        m_module->SetOutputPortsNumber(ui->ipSelector->count());
    }

    SetEnabledApplyButton(ui->ipSelector->count());
    SetEnabledOkButton(ui->ipSelector->count());
}

void ADCServerDialogBox::SwitchADCOn()
{
    if (!ui->ipSelector->count())
    {
        m_module->Stop();
        QMessageBox msgBox;
        msgBox.setText("Предупреждение!");
        msgBox.setInformativeText(QString("Список найденных АЦП пуст. "
                                          "Обновите список или проверьте соединения"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        // останвливаем схему
        return;
    }

    SendCommandAbstractModule();

    m_switchADCOnOff = true;
    m_switchUFOSOnOff = true;

    m_module->DataSocketOpen();
    SetUFOSAllControlReg();
    SetADCControlReg();

}

void ADCServerDialogBox::SwitchADCOff()
{
    if(ui->ipSelector->count())
    {
        m_switchADCOnOff = false;
        m_switchUFOSOnOff = false;
        m_module->DataSocketClose();

        SetADCControlReg();
        SetUFOSAllControlReg();
    }
}

void ADCServerDialogBox::SetupDelayADC()
{
    QLineEdit * le = qobject_cast<QLineEdit*>(sender());
    int index = le->property("index").toInt();
    bool ok;
    uint32_t val = le->text().toUInt(&ok, 16);
    if(ok)
    {
        m_delayADCBoard[index] = val;
        SetClockTune(val);
    }
}

void ADCServerDialogBox::InitOldValue()
{
    m_currentValues.signalPeriod_H.adress = (uint8_t)RegisterTXPERIOD::ADDR::ADDRESS_H;
    m_currentValues.signalPeriod_L.adress = (uint8_t)RegisterTXPERIOD::ADDR::ADDRESS_L;
}

bool ADCServerDialogBox::CheckStatus()
{
    bool resultCheck = true;
    resultCheck = resultCheck && CheckData(m_statusADC.control, m_currentValues.adcControlReg);
    resultCheck = resultCheck && CheckData(m_statusADC.txPeriod[0], m_currentValues.signalPeriod_L);
    resultCheck = resultCheck && CheckData(m_statusADC.txPeriod[1], m_currentValues.signalPeriod_H);
    resultCheck = resultCheck && CheckData(m_statusADC.lengthTX, m_currentValues.signalLength);
    resultCheck = resultCheck && CheckData(m_statusADC.registerAF, m_currentValues.registerAF);
    return resultCheck;
}

void ADCServerDialogBox::SetupAllDelayADC()
{
    ///TODO Написать валидатор
    bool ok;
    SetClockTune(ui->delayADC_1->text().toUInt(&ok, 16));
    SetClockTune(ui->delayADC_2->text().toUInt(&ok, 16));
    SetClockTune(ui->delayADC_3->text().toUInt(&ok, 16));
    SetClockTune(ui->delayADC_4->text().toUInt(&ok, 16));
    SetClockTune(ui->delayADC_5->text().toUInt(&ok, 16));
}

QList<std::pair<QString, QVariant> > ADCServerDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;

    listSetting.append(std::make_pair("signalPeriod_H", m_currentValues.signalPeriod_H.TX_PERIOD));
    listSetting.append(std::make_pair("signalPeriod_L", m_currentValues.signalPeriod_L.TX_PERIOD));
    listSetting.append(std::make_pair("RX_MODE", (uint8_t)m_currentValues.registerAF.RX_MODE));
    listSetting.append(std::make_pair("TEST_BEAR", (uint8_t)m_currentValues.adcControlReg.TEST_BEAR));
    listSetting.append(std::make_pair("CountAzimut", ui->countAzimut->value()));
    listSetting.append(std::make_pair("LengthTX", ui->signalLengthDial->value()));

    return listSetting;
}

bool ADCServerDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 6)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "signalPeriod_H")
            m_currentValues.signalPeriod_H.TX_PERIOD = parmSetting.second.toUInt();

        if(parmSetting.first == "signalPeriod_L")
            m_currentValues.signalPeriod_L.TX_PERIOD = parmSetting.second.toUInt();

        if(parmSetting.first == "RX_MODE")
            m_currentValues.registerAF.RX_MODE = parmSetting.second.toUInt();

        if(parmSetting.first == "TEST_BEAR")
            m_currentValues.adcControlReg.TEST_BEAR = parmSetting.second.toUInt();

        if(parmSetting.first == "LengthTX")
            m_currentValues.signalLength.LengthTX = parmSetting.second.toInt();

        if(parmSetting.first == "CountAzimut")
            SetCountAzimut(parmSetting.second.toInt());

    }
    double signalFrequency = 1000.0 / ((m_currentValues.signalPeriod_L.TX_PERIOD + (m_currentValues.signalPeriod_H.TX_PERIOD << 8) + 1) * 0.08);
    ui->signalLengthDial->setValue(m_currentValues.signalLength.LengthTX);

    ui->signalFrequency->setValue(signalFrequency);
    ui->accumMode->setCurrentIndex(m_currentValues.registerAF.RX_MODE);
    ui->innerAzimuthRadioButton->setChecked(m_currentValues.adcControlReg.TEST_BEAR);
    return true;
}

