#include "adcnewdialogbox.h"
#include <QSettings>

ADCNewDialogBox::ADCNewDialogBox()
{
    CreateWidget();
    SetTopLayout(m_gridLayout);
}

ADCNewDialogBox::~ADCNewDialogBox(){}

QList<std::pair<QString, QVariant> > ADCNewDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_syncComboBox", m_syncComboBox->currentIndex()));
    listSetting.append(std::make_pair("m_adcModeComboBox", m_adcModeComboBox->currentIndex()));
    listSetting.append(std::make_pair("m_sampleNumComboBox", m_sampleNumComboBox->currentIndex()));
    listSetting.append(std::make_pair("m_averSigComboBox", m_averSigComboBox->currentIndex()));
    listSetting.append(std::make_pair("m_averSigGroupBox", m_averSigGroupBox->isChecked()));
    listSetting.append(std::make_pair("m_agelFormatComboBox", m_agelFormatComboBox->currentIndex()));
    listSetting.append(std::make_pair("m_shiftQSpinBox", m_shiftQSpinBox->value()));
    listSetting.append(std::make_pair("m_shiftLevelISpinBox", m_shiftLevelISpinBox->value()));
    listSetting.append(std::make_pair("m_shiftLevelQSpinBox", m_shiftLevelQSpinBox->value()));
    listSetting.append(std::make_pair("isImitationMode", m_imitatorWidget->isImitationMode()));

    return listSetting;
}

bool ADCNewDialogBox::SetSetting(QList<std::pair<QString, QVariant>> listSetting)
{   
    if(listSetting.size() != 10)
        return false;

    for(auto parmSetting : listSetting)
    {
        if(parmSetting.first == "m_syncComboBox")
            m_syncComboBox->setCurrentIndex(parmSetting.second.toInt());

        if(parmSetting.first == "m_adcModeComboBox")
            m_adcModeComboBox->setCurrentIndex(parmSetting.second.toInt());

        if(parmSetting.first == "m_sampleNumComboBox")
            m_sampleNumComboBox->setCurrentIndex(parmSetting.second.toInt());

        if(parmSetting.first == "m_averSigComboBox")
            m_averSigComboBox->setCurrentIndex(parmSetting.second.toInt());

        if(parmSetting.first == "m_averSigGroupBox")
            m_averSigGroupBox->setChecked(parmSetting.second.toBool());

        if(parmSetting.first == "m_agelFormatComboBox")
            m_agelFormatComboBox->setCurrentIndex(parmSetting.second.toInt());

        if(parmSetting.first == "m_shiftQSpinBox")
            m_shiftQSpinBox->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "m_shiftLevelISpinBox")
            m_shiftLevelISpinBox->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "m_shiftLevelQSpinBox")
            m_shiftLevelQSpinBox->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "isImitationMode")
            imitationMode(parmSetting.second.toBool());

    }

    return true;
}

SAMPLE_NUM ADCNewDialogBox::GetSampleNum() const
{
    return static_cast<SAMPLE_NUM>(m_sampleNumComboBox->currentData().toInt());
}

AVER_MODE ADCNewDialogBox::GetAverMode() const
{
    return static_cast<AVER_MODE>(m_averSigGroupBox->isChecked());
}

AVER_NUM ADCNewDialogBox::GetAverNum() const
{
    return static_cast<AVER_NUM>(m_averSigComboBox->currentData().toInt());
}

ADC_MODE ADCNewDialogBox::GetAdcMode() const
{
    return static_cast<ADC_MODE>(m_adcModeComboBox->currentData().toInt());
}

ADC_SYNC ADCNewDialogBox::GetAdcSync() const
{
    return static_cast<ADC_SYNC>(m_syncComboBox->currentData().toInt());
}

int8_t ADCNewDialogBox::GetShiftQ() const
{
    return static_cast<int8_t>(m_shiftQSpinBox->value());
}

int ADCNewDialogBox::GetShiftLevelI() const
{
    return m_shiftLevelISpinBox->value();
}

int ADCNewDialogBox::GetShiftLevelQ() const
{
    return m_shiftLevelQSpinBox->value();
}

int ADCNewDialogBox::GetX() const
{
    return m_imitatorWidget->GetX();
}

int ADCNewDialogBox::GetY() const
{
    return m_imitatorWidget->GetY();
}

int ADCNewDialogBox::GetV() const
{
    return m_imitatorWidget->GetV();
}

bool ADCNewDialogBox::isImitationMode() const
{
    return m_imitatorWidget->isImitationMode();
}

void ADCNewDialogBox::imitationMode(bool enable)
{
    m_imitatorWidget->setEnabled(enable);
}

bool ADCNewDialogBox::isOneRun() const
{
    return m_imitatorWidget->isOneRun();
}

int ADCNewDialogBox::GetFormatAngel()  const
{
    return m_agelFormatComboBox->currentData().toInt();
}

void ADCNewDialogBox::CreateWidget()
{
    m_gridLayout = new QGridLayout;
    SetupSyncMode(0, 0);
    SetupAdcMode(0, 1);
    SetupSampleNum(1, 0);
    SetupAverSig(1, 1);
    SetupAngelFormat(2, 0);
    SetupShiftQ(2, 1);
    SetupShiftLevelI(3, 0);
    SetupShiftLevelQ(3, 1);
    m_imitatorWidget = new ImitatorWidget;
    m_gridLayout->addLayout(m_imitatorWidget, 0, 2, 4, 1);
}

void ADCNewDialogBox::SetupSampleNum(int row, int column)
{
    QGroupBox* sampleNumGroupBox = new QGroupBox("Количество отсчетов");
    m_sampleNumComboBox = new QComboBox;
    QHBoxLayout* sampleNumLayout = new QHBoxLayout;
    sampleNumLayout->addWidget(m_sampleNumComboBox);
    sampleNumGroupBox->setLayout(sampleNumLayout);

    m_sampleNumComboBox->addItem("512", static_cast<int>(SAMPLE_NUM::SAMP_512));
    m_sampleNumComboBox->addItem("1024", static_cast<int>(SAMPLE_NUM::SAMP_1024));
    m_sampleNumComboBox->addItem("2048", static_cast<int>(SAMPLE_NUM::SAMP_2048));
    m_sampleNumComboBox->addItem("3072", static_cast<int>(SAMPLE_NUM::SAMP_3072));
    m_sampleNumComboBox->addItem("4096", static_cast<int>(SAMPLE_NUM::SAMP_4096));
    m_sampleNumComboBox->addItem("5120", static_cast<int>(SAMPLE_NUM::SAMP_5120));
    m_sampleNumComboBox->addItem("6144", static_cast<int>(SAMPLE_NUM::SAMP_6144));
    m_sampleNumComboBox->addItem("7168", static_cast<int>(SAMPLE_NUM::SAMP_7168));
    m_sampleNumComboBox->setCurrentText("512");
    m_gridLayout->addWidget(sampleNumGroupBox, row, column);

}

void ADCNewDialogBox::SetupAverSig(int row, int column)
{
    m_averSigGroupBox = new QGroupBox("Накопление");
    m_averSigGroupBox->setCheckable(true);
    m_averSigComboBox = new QComboBox;
    QHBoxLayout* averSigLayout = new QHBoxLayout;
    averSigLayout->addWidget(m_averSigComboBox);
    m_averSigGroupBox->setLayout(averSigLayout);

    m_averSigComboBox->addItem("2", static_cast<int>(AVER_NUM::AVER_2));
    m_averSigComboBox->addItem("4", static_cast<int>(AVER_NUM::AVER_4));
    m_averSigComboBox->addItem("8", static_cast<int>(AVER_NUM::AVER_8));
    m_averSigComboBox->addItem("16", static_cast<int>(AVER_NUM::AVER_16));
    m_averSigComboBox->addItem("32", static_cast<int>(AVER_NUM::AVER_32));
    m_averSigComboBox->addItem("64", static_cast<int>(AVER_NUM::AVER_64));
    m_averSigComboBox->addItem("128", static_cast<int>(AVER_NUM::AVER_128));
    m_averSigComboBox->addItem("256", static_cast<int>(AVER_NUM::AVER_256));

    m_averSigComboBox->setCurrentText("2");
    m_averSigGroupBox->setChecked(false);

    m_gridLayout->addWidget(m_averSigGroupBox, row, column);
}

void ADCNewDialogBox::SetupAdcMode(int row, int column)
{
    QGroupBox* adcModeGroupBox = new QGroupBox("Режим работы");
    m_adcModeComboBox = new QComboBox;

    QHBoxLayout* adcModeLayout = new QHBoxLayout;
    adcModeLayout->addWidget(m_adcModeComboBox);
    adcModeGroupBox->setLayout(adcModeLayout);

    m_adcModeComboBox->addItem("Нормальный", static_cast<int>(ADC_MODE::ADC_NORM_MODE));
    m_adcModeComboBox->addItem("Тестовые данные", static_cast<int>(ADC_MODE::ADC_TPTN_MODE));
    m_adcModeComboBox->addItem("Sine режим", static_cast<int>(ADC_MODE::ADC_SINE_MODE));
    m_adcModeComboBox->addItem("Powerdown", static_cast<int>(ADC_MODE::ADC_PD_MODE));

    m_adcModeComboBox->setCurrentText("Нормальный");

    m_gridLayout->addWidget(adcModeGroupBox, row, column);

}

void ADCNewDialogBox::SetupSyncMode(int row, int column)
{
    QGroupBox* syncGroupBox = new QGroupBox("Синхронизация");
    m_syncComboBox = new QComboBox;

    QHBoxLayout* syncLayout = new QHBoxLayout;
    syncLayout->addWidget(m_syncComboBox);
    syncGroupBox->setLayout(syncLayout);

    m_syncComboBox->addItem("Внешняя", static_cast<int>(ADC_SYNC::SYNC_MODE_EXT));
    m_syncComboBox->addItem("Внутренняя", static_cast<int>(ADC_SYNC::SYNC_MODE_INT));
    m_syncComboBox->addItem("Тестовая", static_cast<int>(ADC_SYNC::SYNC_MODE_TST));

    m_syncComboBox->setCurrentText("Внешняя");

    m_gridLayout->addWidget(syncGroupBox, row, column);
}

void ADCNewDialogBox::SetupAngelFormat(int row, int column)
{
    QGroupBox* agelFormatGroupBox = new QGroupBox("Новый угол через");

    m_agelFormatComboBox = new QComboBox;
    QHBoxLayout* agelFormat = new QHBoxLayout;
    agelFormat->addWidget(m_agelFormatComboBox);
    agelFormatGroupBox->setLayout(agelFormat);
    m_agelFormatComboBox->addItem("1", 1);
    m_agelFormatComboBox->addItem("2", 2);
    m_agelFormatComboBox->addItem("4", 4);
    m_agelFormatComboBox->addItem("8", 8);
    m_agelFormatComboBox->addItem("16", 16);
    m_agelFormatComboBox->addItem("32", 32);
    m_agelFormatComboBox->addItem("64", 64);
    m_agelFormatComboBox->addItem("128", 128);
    m_agelFormatComboBox->addItem("256", 256);

    m_agelFormatComboBox->setCurrentText("1");
    m_gridLayout->addWidget(agelFormatGroupBox, row, column);
}

void ADCNewDialogBox::SetupShiftQ(int row, int column)
{
    QGroupBox* shiftQGroupBox = new QGroupBox("Сдвиг отсчетов Q");

    m_shiftQSpinBox = new QSpinBox;
    QHBoxLayout* shiftLH = new QHBoxLayout;
    shiftLH->addWidget(m_shiftQSpinBox);
    shiftQGroupBox->setLayout(shiftLH);
    m_shiftQSpinBox->setRange(-6, 6);
    m_gridLayout->addWidget(shiftQGroupBox, row, column);
}

void ADCNewDialogBox::SetupShiftLevelQ(int row, int column)
{
    QGroupBox* shiftQGroupBox = new QGroupBox("Смещение уровня Q");

    m_shiftLevelQSpinBox = new QSpinBox;
    QHBoxLayout* shiftLH = new QHBoxLayout;
    shiftLH->addWidget(m_shiftLevelQSpinBox);
    shiftQGroupBox->setLayout(shiftLH);
    m_shiftLevelQSpinBox->setRange(-300, 300);
    m_gridLayout->addWidget(shiftQGroupBox, row, column);
}

void ADCNewDialogBox::SetupShiftLevelI(int row, int column)
{
    QGroupBox* shiftIGroupBox = new QGroupBox("Смещение уровня I");

    m_shiftLevelISpinBox = new QSpinBox;
    QHBoxLayout* shiftLH = new QHBoxLayout;
    shiftLH->addWidget(m_shiftLevelISpinBox);
    shiftIGroupBox->setLayout(shiftLH);
    m_shiftLevelISpinBox->setRange(-300, 300);
    m_gridLayout->addWidget(shiftIGroupBox, row, column);
}


ImitatorWidget::ImitatorWidget()
{
    CreateWidget();
}

ImitatorWidget::~ImitatorWidget(){}

int ImitatorWidget::GetX() const
{
    return m_x->value();
}

int ImitatorWidget::GetY() const
{
    return m_y->value();
}

int ImitatorWidget::GetV() const
{
    return m_v->value()/* / 3.6*/;
}

bool ImitatorWidget::isImitationMode() const
{
    return m_imitatorParametr->isChecked();
}

bool ImitatorWidget::isOneRun() const
{
    return m_isOneRun->isChecked();
}

void ImitatorWidget::imitationMode(bool enable)
{
    m_imitatorParametr->setChecked(enable);
}

void ImitatorWidget::CreateWidget()
{
    QGridLayout* layoutImitator = new QGridLayout;
    m_x = new QSpinBox;
    m_x->setRange(-10, 10);
    layoutImitator->addWidget(new QLabel("x, м :"), 0, 0);
    layoutImitator->addWidget(m_x, 0, 1);
    m_y = new QSpinBox;
    m_y->setRange(0, 150);
    layoutImitator->addWidget(new QLabel("y, м:"), 1, 0);
    layoutImitator->addWidget(m_y, 1, 1);

    m_v = new QSpinBox;
    m_v->setRange(-90, 90);

    layoutImitator->addWidget(new QLabel("v, км/ч:"), 2, 0);
    layoutImitator->addWidget(m_v, 2, 1);

    m_isOneRun = new QCheckBox("Однократный запуск");
    layoutImitator->addWidget(m_isOneRun, 3, 0, 1, 2);

    m_imitatorParametr = new QGroupBox("Имитатор");
    m_imitatorParametr->setLayout(layoutImitator);
    m_imitatorParametr->setCheckable(true);
    m_imitatorParametr->setChecked(false);
    addWidget(m_imitatorParametr);
}











