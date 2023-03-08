#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "scopecomplexdialogbox.h"

ScopeComplexDialogBox::ScopeComplexDialogBox(CSignalComplexPlotter* parent, QWidget* wparent):
    QDialog(wparent, Qt::Window | Qt::WindowSystemMenuHint |
            Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
{
    m_memoryScope = new PlotterComplex(parent);
    QGridLayout* m_mainLayout = new QGridLayout;

    m_dialMagnitudeDimention = new QDial;

    m_dialMagnitudeDimention->setNotchesVisible(true);
    m_dialMagnitudeDimention->setRange(4, 17);
    m_dialMagnitudeDimention->setFixedWidth(130);
    QLabel* labelMagnitude = new QLabel("Амплитуда");
    labelMagnitude->setFixedHeight(15);

    m_LCDMagnitude = new QLCDNumber;
    m_LCDMagnitude->setPalette(QPalette("#1111ff"));
    QVBoxLayout* lMagnitude = new QVBoxLayout;
    lMagnitude->addStretch();
    lMagnitude->addWidget(m_LCDMagnitude, 0, Qt::AlignCenter|Qt::AlignBottom);
    lMagnitude->addWidget(m_dialMagnitudeDimention);
    lMagnitude->addWidget(labelMagnitude, 0, Qt::AlignCenter|Qt::AlignTop);

    m_dialTimeDimention = new QDial;
    m_dialTimeDimention->setNotchesVisible(true);
    m_dialTimeDimention->setRange(4, 14);
    m_dialTimeDimention->setSingleStep(1);


    QLabel* labelTime = new QLabel("Время");
    labelTime->setFixedHeight(15);

    m_LCDTime =  new QLCDNumber;
    m_LCDTime->setPalette(QPalette("#1111ff"));
    QVBoxLayout* lTime = new QVBoxLayout;
    lTime->addWidget(m_LCDTime, 0, Qt::AlignCenter|Qt::AlignBottom);
    lTime->addWidget(m_dialTimeDimention);
    lTime->addWidget(labelTime, 0, Qt::AlignCenter|Qt::AlignTop);
    lTime->addStretch();

    QPushButton* settingButton = new QPushButton("Настройки\nотображения");


    m_mainLayout->addWidget(m_memoryScope, 0, 0, 3, 1);

    m_mainLayout->addLayout(lMagnitude, 0, 1, 1, 1);
    m_mainLayout->addLayout(lTime, 1, 1, 1, 1);
    m_mainLayout->addWidget(settingButton, 2, 1, 1, 1);

    setLayout(m_mainLayout);
    setMinimumWidth(1024);
    setMinimumHeight(512);

    m_settingScopeDialog = new SettingScopeDialog(this);
    connect(settingButton, SIGNAL(clicked()), this, SLOT(ShowDialogSetting()));

    connect(m_dialTimeDimention, SIGNAL(valueChanged(int)), this, SLOT(SetTime(int)));
    connect(m_dialMagnitudeDimention, SIGNAL(valueChanged(int)), this, SLOT(SetMagnitude(int)));

    m_dialMagnitudeDimention->setValue(13);
    m_dialTimeDimention->setValue(11);
}

void ScopeComplexDialogBox::ShowDialog()
{
    if(isHidden())
        show();
    else {
        raise();
        activateWindow();
        showNormal();
    }
}

int ScopeComplexDialogBox::GetMagnitudeDimention() const
{
    return m_dialMagnitudeDimention->value();
}

void ScopeComplexDialogBox::SetMagnitudeDimention(int magnitudeDimention)
{
    m_dialMagnitudeDimention->setValue(magnitudeDimention);
}

int ScopeComplexDialogBox::GetTimeDimention() const
{
    return m_dialTimeDimention->value();
}

void ScopeComplexDialogBox::SetTimeDimention(int timeDimention)
{
    m_dialTimeDimention->setValue(timeDimention);
}

TypeX ScopeComplexDialogBox::GetTypeX() const
{
    return m_settingScopeDialog->GetTypeX();
}

void ScopeComplexDialogBox::SetTypeX(TypeX typeX)
{
    m_settingScopeDialog->SetTypeX(typeX);
}

int ScopeComplexDialogBox::GetVal() const
{
    return m_settingScopeDialog->GetVal();
}

void ScopeComplexDialogBox::SetVal(int val)
{
    m_settingScopeDialog->SetVal(val);
}

QString ScopeComplexDialogBox::GetValueName() const
{
    return m_settingScopeDialog->GetValueName();
}

void ScopeComplexDialogBox::SetValueName(const QString& valueName)
{
    m_settingScopeDialog->SetValueName(valueName);
}

void ScopeComplexDialogBox::SetMagnitude(int Ymax)
{
    double x = std::pow(2, m_dialTimeDimention->value());
    double y = std::pow(2, Ymax);
    m_LCDMagnitude->display(y);
    m_memoryScope->SetPlotSetting(PlotSetting(x, y));
}

void ScopeComplexDialogBox::SetTime(int Xmax)
{
    double x = std::pow(2, Xmax);
    double y = std::pow(2, m_dialMagnitudeDimention->value());
    m_LCDTime->display(x);
    m_memoryScope->SetPlotSetting(PlotSetting(x, y));

}

void ScopeComplexDialogBox::SetPliting()
{
    m_memoryScope->SetSettindDisplay(m_settingScopeDialog->GetTypeX(),
                                     m_settingScopeDialog->GetVal(),
                                     m_settingScopeDialog->GetValueName());
}

void ScopeComplexDialogBox::ShowDialogSetting()
{
    if(m_settingScopeDialog->exec())
        SetPliting();
}

/////////////////////

SettingScopeDialog::SettingScopeDialog(QWidget *parent): QDialog(parent)
{
    m_dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                             |QDialogButtonBox::Cancel ,Qt::Horizontal);
    connect(m_dialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_dialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QGridLayout* gl = new QGridLayout;
    m_typeComboBox = new QComboBox;
    QLabel* typeLabel = new QLabel("Тип оси х: ");
    m_typeComboBox->addItem("Отсчет");
    m_typeComboBox->addItem("Время");
    m_typeComboBox->addItem("Частота");
    m_typeComboBox->addItem("Дальность");
    gl->addWidget(typeLabel, 0, 0, 1, 1);
    gl->addWidget(m_typeComboBox, 0, 1, 1, 2);
    valueLabel = new QLabel;
    m_valueSpinBox = new QSpinBox;
    m_valueNameComboBox = new QComboBox;
    gl->addWidget(valueLabel, 1, 0);
    gl->addWidget(m_valueSpinBox, 1, 1);
    gl->addWidget(m_valueNameComboBox, 1, 2);
    valueLabel->setVisible(false);
    m_valueSpinBox->setVisible(false);
    m_valueNameComboBox->setVisible(false);

    mainLayout->addLayout(gl);
    mainLayout->addWidget(m_dialogButtonBox);
    setLayout(mainLayout);
    connect(m_typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeType(int)));
}

TypeX SettingScopeDialog::GetTypeX() const
{
    return static_cast<TypeX>(m_typeComboBox->currentIndex());
}

void SettingScopeDialog::SetTypeX(TypeX typeX)
{
    m_typeComboBox->setCurrentIndex(typeX);
}

int SettingScopeDialog::GetVal() const
{
    return m_valueSpinBox->value();
}

void SettingScopeDialog::SetVal(int val)
{
    m_valueSpinBox->setValue(val);
}

QString SettingScopeDialog::GetValueName() const
{
    return m_valueNameComboBox->currentText();
}

void SettingScopeDialog::SetValueName(const QString& valueName)
{
    m_valueNameComboBox->setCurrentText(valueName);
}

void SettingScopeDialog::ChangeType(int val)
{
    switch (val)
    {
    case typeValue:
    {
        valueLabel->setVisible(false);
        m_valueSpinBox->setVisible(false);
        m_valueNameComboBox->setVisible(false);
        m_valueSpinBox->setRange(0, 0);
        m_valueSpinBox->setValue(0);
        valueLabel->clear();
        break;
    }
    case typeTime:
    {
        valueLabel->setVisible(true);
        m_valueSpinBox->setVisible(true);
        m_valueNameComboBox->setVisible(true);
        valueLabel->setText("Значение отсчета времени:");
        m_valueSpinBox->setRange(1, 999);
        m_valueNameComboBox->clear();
        m_valueNameComboBox->addItem("с");
        m_valueNameComboBox->addItem("мкс");
        m_valueNameComboBox->addItem("нс");
        m_valueNameComboBox->setCurrentIndex(2);

        break;
    }
    case typeFrequency:
    {
        valueLabel->setVisible(true);
        m_valueSpinBox->setVisible(true);
        m_valueNameComboBox->setVisible(true);
        valueLabel->setText("Частота дискретизации:");
        m_valueSpinBox->setRange(1, 999);
        m_valueNameComboBox->clear();
        m_valueNameComboBox->addItem("кГц");
        m_valueNameComboBox->addItem("МГц");
        m_valueNameComboBox->addItem("ГГц");
        m_valueNameComboBox->setCurrentIndex(1);
        break;
    }
    case typeRange:
    {
        valueLabel->setVisible(true);
        m_valueSpinBox->setVisible(true);
        m_valueNameComboBox->setVisible(true);
        valueLabel->setText("Значение отсчета дальности:");
        m_valueSpinBox->setRange(1, 100);
        m_valueNameComboBox->clear();
        m_valueNameComboBox->addItem("см");

        break;
    }
    default:
        break;
    }
}

QList<std::pair<QString, QVariant>> ScopeComplexDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant>> setting;
    setting.push_back(std::make_pair(QString("MagnitudeDimention"), GetMagnitudeDimention()));
    setting.push_back(std::make_pair(QString("TimeDimention"), GetTimeDimention()));
    setting.push_back(std::make_pair(QString("TypeX"), GetTypeX()));
    setting.push_back(std::make_pair(QString("Val"), GetVal()));
    setting.push_back(std::make_pair(QString("ValueName"), GetValueName()));
    return setting;
}

bool ScopeComplexDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 5)
        return false;

    foreach (auto& parmSetting, listSetting)
    {
        if(parmSetting.first == "MagnitudeDimention")
            SetMagnitudeDimention(parmSetting.second.toInt());

        if(parmSetting.first == "TimeDimention")
            SetTimeDimention(parmSetting.second.toInt());

        if(parmSetting.first == "TypeX")
            SetTypeX(static_cast<TypeX>(parmSetting.second.toInt()));

        if(parmSetting.first == "Val")
            SetVal(parmSetting.second.toInt());

        if(parmSetting.first == "ValueName")
            SetValueName(parmSetting.second.toString());
    }

    return true;
}

void ScopeComplexDialogBox::SendCommand()
{
    SetPliting();
}

void ScopeComplexDialogBox::Attach(ChangeEvent* observer)
{
    m_observer = observer;
}

void ScopeComplexDialogBox::Notification()
{
    if(m_observer)
        m_observer->Change();
}
