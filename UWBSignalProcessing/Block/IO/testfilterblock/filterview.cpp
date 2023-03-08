#include "filterview.h"

FilterDialogBox::FilterDialogBox()
{
    layout = new QGridLayout;
    // установка имени блока
    m_selectState = new QComboBox;
    layout->addWidget(m_selectState);
    AddState(new Lowpass);
    AddState(new Hightpass);
    AddState(new Bandpass);
    AddState(new Bandstop);
    AddState(new Hlbrt);
    m_selectState->setCurrentIndex(m_filterStates.size() - 1);
    SetTopLayout(layout);
    connect(m_selectState, SIGNAL(currentIndexChanged(int)),this, SLOT(SelectState(int)));
    m_selectState->setCurrentIndex(0);
}

FilterDialogBox::~FilterDialogBox()
{
    foreach(auto state, m_filterStates)
        delete state;
}

QList<std::pair<QString, QVariant> > FilterDialogBox::GetSetting()
{
    if(m_currentState)
    {
        QList<std::pair<QString, QVariant>> settings = m_currentState->SaveSettings();
        settings.push_front(std::pair<QString, QVariant>("TypeOfFilter", QString( m_selectState->currentText())));
        return settings;
    }

        return QList<std::pair<QString, QVariant>>();
}

bool FilterDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    for(auto & i : listSetting)
    {
        if(i.first == "TypeOfFilter") {
            m_selectState->setCurrentText(i.second.toString());
        }
    }
    m_currentState->SetParameter(listSetting);
    return true;
}

uint8_t FilterDialogBox::GetInputPortsNumber()
{
    return m_currentState->GetSettingSignal()->GetInputPortsNumber();
}

uint8_t FilterDialogBox::GetOutputPortsNumber()
{
    return m_currentState->GetSettingSignal()->GetOutputPortsNumber();
}

bool FilterDialogBox::GetPortState() const
{
    return m_currentState->GetSettingSignal()->GetPortState();
}

double FilterDialogBox::GetSampleFreq()
{
    return m_currentState->GetSettingSignal()->GetSamp();
}

double FilterDialogBox::GetCutFreq1()
{
    return m_currentState->GetCutFreq1() * 1E+6;
}

double FilterDialogBox::GetCutFreq2()
{
    return m_currentState->GetCutFreq2() * 1E+6;
}

int FilterDialogBox::GetOrder()
{
    return m_currentState->GetSettingSignal()->GetOrderFilter();
}

int FilterDialogBox::GetFilterType()
{
    return m_currentState->GetFilterType();
}

void FilterDialogBox::AddState(FilterState *state)
{
    m_filterStates.push_back(state);
    ChangeState(state);
    m_selectState->addItem(m_filterStates.back()->Name());
    layout->addWidget(state);
    m_currentState->hide();
}

void FilterDialogBox::SelectState(int index)
{
    m_currentState->hide();
    m_currentState->ChangeState(this, m_filterStates[index]);
    m_currentState->show();
    setFixedSize(minimumSize());
}

void FilterDialogBox::ChangeState(FilterState *state)
{
    state->setStyleSheet(styleSheet());
    m_currentState = state;
}

///=================SettingFilter========================///

SettingSignal::SettingSignal()
{
    m_mainLayout = new QGridLayout;
    setLayout(m_mainLayout);
    CreateOrderFilter();
    CreateSamp();
    CreateCountPorts();
    setWindowTitle("Настройки фильтра");
}

int SettingSignal::GetOrderFilter()
{
    return m_orderFilter->value();
}

double SettingSignal::GetSamp()
{
    int index = m_sampUnit->currentIndex();
    double coef = (m_sampUnit->itemData(index)).toDouble();
    return coef;
}

uint8_t SettingSignal::GetInputPortsNumber()
{
    m_currentCount = m_countPorts->value();
    return static_cast<uint8_t>(m_countPorts->value());
}

uint8_t SettingSignal::GetOutputPortsNumber()
{
    return static_cast<uint8_t>(m_countPorts->value());
}

bool SettingSignal::GetPortState()
{
    return m_currentCount != m_countPorts->value();
}

QList<std::pair<QString, QVariant> > SettingSignal::SaveSettings()
{
    QList<std::pair<QString, QVariant>> settings;
    settings.push_back(std::make_pair("FilterOrder", m_orderFilter->value()));
    settings.push_back(std::make_pair("SamplingPeriod", m_sampUnit->currentText()));
    settings.push_back(std::make_pair("CountPorts", m_countPorts->value()));
    return settings;
}

void SettingSignal::SetParameter(const QList<std::pair<QString, QVariant>>& param)
{
    for(const auto & i : param)
    {
        if(i.first == "FilterOrder")
            m_orderFilter->setValue(i.second.toInt());

        if(i.first == "SamplingPeriod")
            m_sampUnit->setCurrentText(i.second.toString());

        if(i.first == "CountPorts")
            m_countPorts->setValue(i.second.toInt());
    }
}

void SettingSignal::AddWidget(const QString& name, QWidget* widget )
{
    int currentRow = m_mainLayout->rowCount();
    m_mainLayout->addWidget(new QLabel(name), currentRow, 0, Qt::AlignLeft);
    m_mainLayout->addWidget(widget, currentRow, 1, Qt::AlignRight);
}

void SettingSignal::AddLayout(const QString& name, QLayout *layout)
{
    int currentRow = m_mainLayout->rowCount();
    m_mainLayout->addWidget(new QLabel(name), currentRow, 0, Qt::AlignLeft);
    m_mainLayout->addLayout(layout, currentRow, 1, Qt::AlignRight);
}

void SettingSignal::CreateOrderFilter()
{
    m_orderFilter = new QSpinBox;
    m_orderFilter->setRange(0, 8192);
    m_orderFilter->setValue(10);
    AddWidget("Порядок фильтра:", m_orderFilter);
}

void SettingSignal::CreateSamp()
{
    m_sampUnit = new QComboBox;
    m_sampUnit->addItem("500 МГц" , 5e+8);
    m_sampUnit->addItem("1 ГГц" , 1e+9);
    m_sampUnit->addItem("2 ГГц" , 2e+9);
    m_sampUnit->setCurrentIndex(0);
    auto* layout = new QHBoxLayout;
    layout->addWidget(m_sampUnit, 0, Qt::AlignLeft);
    AddLayout("Частота дискретизации", layout);
}

void SettingSignal::CreateCountPorts()
{
    m_countPorts = new QSpinBox;
    m_countPorts->setRange(1, 2);
    AddWidget("Количество портов:", m_countPorts);
}

///=============FilterState===============================///

FilterState::FilterState(QWidget *wparent):QWidget (wparent)
{
    m_buttonSettingSignal = new QPushButton("Настройки сигнала");
    m_settingSignal = new SettingSignal;
    connect(m_buttonSettingSignal, SIGNAL(clicked(bool)), this, SLOT(ShowSettingFilter()));
}

FilterState::~FilterState()
{
    delete m_settingSignal;
}

QList<std::pair<QString, QVariant> > FilterState::SaveSettings() const
{
    return QList<std::pair<QString, QVariant>>();
}

void FilterState::SetParameter(const QList<std::pair<QString, QVariant> > &)
{
    //не нужны параметры
}

QString FilterState::Name()
{
    return QString();
}

void FilterState::ChangeState(FilterDialogBox* dialog, FilterState* state)
{
    dialog->ChangeState(state);
}

SettingSignal* FilterState::GetSettingSignal() const
{
    return m_settingSignal;
}

double FilterState::GetCutFreq1()
{
    return -1;
}

double FilterState::GetCutFreq2()
{
    return -1;
}

int FilterState::GetFilterType()
{
    return -1;
}

void FilterState::ShowSettingFilter()
{
    m_settingSignal->show();
    m_settingSignal->setStyleSheet(styleSheet());
}

QPushButton *FilterState::buttonSettingSignal() const
{
    return m_buttonSettingSignal;
}

///=============Lowpass===============================///

Lowpass::Lowpass(QWidget *wparent):
    FilterState(wparent)
{
    CreateWidget();
}

void Lowpass::CreateWidget()
{
    m_freqCut = new CustomDialWidget("Частота среза Fc");
    m_freqCut->setMaxVal(1000);
    m_freqCut->setMinVal(1);
    m_gridLayoout = new QGridLayout;

    CreateSettingWidget();
    m_gridLayoout->addWidget(m_freqCut);
    setLayout(m_gridLayoout);
    setMaximumSize(sizeHint());
}

void Lowpass::CreateSettingWidget()
{
    m_gridLayoout->addWidget(buttonSettingSignal());
}

QList<std::pair<QString, QVariant>> Lowpass::SaveSettings() const
{
    QList<std::pair<QString, QVariant>> settings;
    settings.push_back(std::make_pair("CutoffFrequency", QString::number( m_freqCut->getDisplayIntVal())));
    QList<std::pair<QString, QVariant>> settingsInWindow;
    settingsInWindow = GetSettingSignal()->SaveSettings();
    settings = settings + settingsInWindow;
    return settings;
}

void Lowpass::SetParameter(const QList<std::pair<QString, QVariant>>& param)
{
    for(const auto & i : param)
    {
        if(i.first == "CutoffFrequency") {
            m_freqCut->setValue(i.second.toInt());;
        }
    }
    GetSettingSignal()->SetParameter(param);
}

QString Lowpass::Name()
{
    return QString("ФНЧ");
}

int Lowpass::GetFilterType()
{
    return 0;
}

double Lowpass::GetCutFreq1()
{
    return m_freqCut->getDisplayIntVal();
}

///=============Hightpass===============================///

Hightpass::Hightpass(QWidget *wparent):
    FilterState(wparent)
{
    CreateWidget();
}

QList<std::pair<QString, QVariant>> Hightpass::SaveSettings() const
{
    QList<std::pair<QString, QVariant>> settings;
    settings.push_back(std::make_pair("CutoffFrequency", QString::number( m_freqCut->getDisplayIntVal())));
    QList<std::pair<QString, QVariant>> settingsInWindow;
    settingsInWindow = GetSettingSignal()->SaveSettings();
    settings = settings + settingsInWindow;
    return settings;
}

void Hightpass::SetParameter(const QList<std::pair<QString, QVariant>>& param)
{
    for(const auto & i : param)
    {
        if(i.first == "CutoffFrequency") {
            m_freqCut->setValue(i.second.toInt());;
        }
    }
    GetSettingSignal()->SetParameter(param);
}

QString Hightpass::Name()
{
    return QString("ФВЧ");
}

int Hightpass::GetFilterType()
{
    return 1;
}

double Hightpass::GetCutFreq1()
{
    return m_freqCut->getDisplayIntVal();
}

void Hightpass::CreateSettingWidget()
{
    m_gridLayoout->addWidget(buttonSettingSignal());
}

void Hightpass::CreateWidget()
{

    m_freqCut = new CustomDialWidget("Частота среза Fc");
    m_freqCut->setMaxVal(1000);
    m_freqCut->setMinVal(1);
    m_gridLayoout = new QGridLayout;

    CreateSettingWidget();
    m_gridLayoout->addWidget(m_freqCut);
    setLayout(m_gridLayoout);
    setMaximumSize(sizeHint());

}

///================Bandpass========================///

Bandpass::Bandpass(QWidget *wparent):
    FilterState(wparent)
{
    CreateWidget();
}

QList<std::pair<QString, QVariant>> Bandpass::SaveSettings() const
{
    QList<std::pair<QString, QVariant>> settings;
    settings.push_back(std::make_pair("CutoffFrequency1", QString::number( m_freqCutFirst->getDisplayIntVal())));
    settings.push_back(std::make_pair("CutoffFrequency2", QString::number( m_freqCutSecond->getDisplayIntVal())));
    QList<std::pair<QString, QVariant>> settingsInWindow;
    settingsInWindow = GetSettingSignal()->SaveSettings();
    settings = settings + settingsInWindow;
    return settings;
}

void Bandpass::SetParameter(const QList<std::pair<QString, QVariant>> &param)
{
    for(const auto & i : param)
    {
        if(i.first == "CutoffFrequency1") {
            m_freqCutFirst->setValue(i.second.toInt());;
        }
        else
        {
            if(i.first == "CutoffFrequency2") {
                m_freqCutSecond->setValue(i.second.toInt());;
            }
        }
    }
    GetSettingSignal()->SetParameter(param);
}

QString Bandpass::Name()
{
    return QString("Полосовой фильтр");
}

double Bandpass::GetCutFreq1()
{
    return m_freqCutFirst->getDisplayIntVal();
}

double Bandpass::GetCutFreq2()
{
    return m_freqCutSecond->getDisplayIntVal();
}

int Bandpass::GetFilterType()
{
    return 2;
}

void Bandpass::CreateWidget()
{
    m_freqCutFirst = new CustomDialWidget("Частота среза Fc1");
    m_freqCutFirst->setMaxVal(1000);
    m_freqCutFirst->setMinVal(1);

    m_freqCutSecond = new CustomDialWidget("Частота среза Fc2");
    m_freqCutSecond->setMaxVal(1000);
    m_freqCutSecond->setMinVal(1);

    m_gridLayoout = new QGridLayout;

    CreateSettingWidget();

    m_gridLayoout->addWidget(m_freqCutFirst);
    m_gridLayoout->addWidget(m_freqCutSecond);

    setLayout(m_gridLayoout);
    setMaximumSize(sizeHint());
}

void Bandpass::CreateSettingWidget()
{
    m_gridLayoout->addWidget(buttonSettingSignal());
}

///=================Bandstop========================///

void Bandstop::CreateSettingWidget()
{
    m_gridLayoout->addWidget(buttonSettingSignal());
}

Bandstop::Bandstop(QWidget *wparent):
    FilterState(wparent)
{    
    CreateWidget();
}

QList<std::pair<QString, QVariant>> Bandstop::SaveSettings() const
{ 
    QList<std::pair<QString, QVariant>> settings;
    settings.push_back(std::make_pair("CutoffFrequency1", QString::number( m_freqCutFirst->getDisplayIntVal())));
    settings.push_back(std::make_pair("CutoffFrequency2", QString::number( m_freqCutSecond->getDisplayIntVal())));
    QList<std::pair<QString, QVariant>> settingsInWindow;
    settingsInWindow = GetSettingSignal()->SaveSettings();
    settings = settings + settingsInWindow;
    return settings;
}

void Bandstop::SetParameter(const QList<std::pair<QString, QVariant>>& param)
{
    for(const auto & i : param)
    {
        if(i.first == "CutoffFrequency1") {
            m_freqCutFirst->setValue(i.second.toInt());;
        }
        else
        {
            if(i.first == "CutoffFrequency2") {
                m_freqCutSecond->setValue(i.second.toInt());;
            }
        }
    }
    GetSettingSignal()->SetParameter(param);
}

QString Bandstop::Name()
{
    return QString("Режекторный фильтр");
}

int Bandstop::GetFilterType()
{
    return 3;
}

double Bandstop::GetCutFreq1()
{
    return m_freqCutFirst->getDisplayIntVal();
}

double Bandstop::GetCutFreq2()
{
    return m_freqCutSecond->getDisplayIntVal();
}

void Bandstop::CreateWidget()
{
    m_freqCutFirst = new CustomDialWidget("Частота среза Fc1");
    m_freqCutFirst->setMaxVal(1000);
    m_freqCutFirst->setMinVal(1);

    m_freqCutSecond = new CustomDialWidget("Частота среза Fc2");
    m_freqCutSecond->setMaxVal(1000);
    m_freqCutSecond->setMinVal(1);

    m_gridLayoout = new QGridLayout;

    CreateSettingWidget();

    m_gridLayoout->addWidget(m_freqCutFirst);
    m_gridLayoout->addWidget(m_freqCutSecond);

    setLayout(m_gridLayoout);
    setMaximumSize(sizeHint());
}

///=================Bandstop========================///

void Hlbrt::CreateSettingWidget()
{
    m_gridLayoout->addWidget(buttonSettingSignal());
}

Hlbrt::Hlbrt(QWidget *wparent):
    FilterState(wparent)
{
    CreateWidget();
}

QList<std::pair<QString, QVariant>> Hlbrt::SaveSettings() const
{
    QList<std::pair<QString, QVariant>> settings;
    settings = GetSettingSignal()->SaveSettings();
    return settings;
}

void Hlbrt::SetParameter(const QList<std::pair<QString, QVariant>> &param)
{
    GetSettingSignal()->SetParameter(param);
}

QString Hlbrt::Name()
{
    return QString("фильтр Гильберта");
}

void Hlbrt::CreateWidget()
{
    m_gridLayoout = new QGridLayout;
    CreateSettingWidget();

    setLayout(m_gridLayoout);
    setMaximumSize(sizeHint());
}

int Hlbrt::GetFilterType()
{
    return 4;
}
