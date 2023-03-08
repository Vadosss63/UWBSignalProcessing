#include "rangeaccumdialogbox.h"
#include <qmath.h>

RangeAccumDialogBox::RangeAccumDialogBox()
{
    CreateWidget();
}

QList<std::pair<QString, QVariant>> RangeAccumDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("numberRange", m_numberRange->value()));
    listSetting.append(std::make_pair("countAcumm", m_countAcumm->value()));
    listSetting.append(std::make_pair("range", m_range->value()));
    listSetting.append(std::make_pair("threshol", m_threshol->value()));
    listSetting.append(std::make_pair("windows", m_windows->currentIndex()));
    listSetting.append(std::make_pair("thresholType", m_thresholType->currentIndex()));
    listSetting.append(std::make_pair("varu", m_varu->isChecked()));
    listSetting.append(std::make_pair("m_gain", m_gain->value()));

    return listSetting;
}

bool RangeAccumDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() < 8)
        return false;

    for(auto& val : listSetting)
    {
        if(val.first == "numberRange")
            m_numberRange->setValue(val.second.toInt());

        if(val.first == "countAcumm")
            m_countAcumm->setValue(val.second.toInt());

        if(val.first == "range")
            m_range->setValue(val.second.toInt());

        if(val.first == "threshol")
            m_threshol->setValue(val.second.toInt());

        if(val.first == "windows")
            m_windows->setCurrentIndex(val.second.toInt());

        if(val.first == "thresholType")
            m_thresholType->setCurrentIndex(val.second.toInt());

        if(val.first == "varu")
            m_varu->setChecked(val.second.toBool());

        if(val.first == "m_gain")
            m_gain->setValue(val.second.toDouble());
    }

    return true;
}

size_t RangeAccumDialogBox::GetNumberRange()
{
    return static_cast<size_t>(m_numberRange->value());
}

size_t RangeAccumDialogBox::GetCountAcumm()
{
    return static_cast<size_t>(m_countAcumm->value());
}

size_t RangeAccumDialogBox::GetRangeAcumm()
{
    return static_cast<size_t>(m_range->value());
}

double RangeAccumDialogBox::GetThresholAcumm()
{
    return static_cast<double>(m_threshol->value());
}

int RangeAccumDialogBox::GetWindow()
{
    return m_windows->currentIndex();
}

int RangeAccumDialogBox::GetThresholType()
{
    return m_thresholType->currentIndex();
}

bool RangeAccumDialogBox::IsVARU()
{
    return m_varu->isChecked();
}

double RangeAccumDialogBox::GetGain()
{
    return m_gain->value();
}

void RangeAccumDialogBox::CreateWidget()
{
    auto layout = new QGridLayout;

    m_numberRange = new QSpinBox;
    m_countAcumm = new QSpinBox;
    m_range = new QSpinBox;

    m_threshol = new QSpinBox;
    m_threshol->setRange(0, 2048);
    int maxRange = 8192;

    m_numberRange->setRange(0, maxRange - 1);
    m_range->setRange(1, maxRange);
    m_range->setValue(1);

    connect(m_numberRange, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int i)
    {
        m_range->setMaximum(maxRange - i);
    });

    m_countAcumm->setRange(1, 1024);

    layout->addWidget(new QLabel("Накопление "), 0, 0);
    layout->addWidget(m_countAcumm, 0, 1);
    layout->addWidget(new QLabel("Начальная дальность "), 0, 2);
    layout->addWidget(m_numberRange, 0, 3);
    layout->addWidget(new QLabel("Количество "), 0, 4);
    layout->addWidget(m_range, 0, 5);

    m_windows = new QComboBox();
    m_windows->addItem("Нет");
    m_windows->addItem("Хемминга");
    m_windows->addItem("Ханна");
    m_windows->addItem("Гаусса");
    m_windows->addItem("Бартлетта-Хеннинга");
    m_windows->addItem("Блэкмана");
    m_windows->addItem("Бартлетта");

    layout->addWidget(new QLabel("Окно fft "), 1, 0);
    layout->addWidget(m_windows, 1, 1);

    m_thresholType = new QComboBox();

    m_thresholType->addItem("Нет");
    m_thresholType->addItem("Бинарный");
    m_thresholType->addItem("Индикаторный");
    m_thresholType->addItem("Индикаторно-адаптивный");

    layout->addWidget(new QLabel("Тип порога "), 1, 2);
    layout->addWidget(m_thresholType, 1, 3);

    layout->addWidget(new QLabel("Порог "), 1, 4);
    layout->addWidget(m_threshol, 1, 5);
    m_varu = new QCheckBox("ВАРУ");
    layout->addWidget(m_varu, 2, 0);

    m_gain = new QDoubleSpinBox;
    m_gain->setRange(-1000.0, 1000.0);
    m_gain->setValue(1.0);
    layout->addWidget(new QLabel("Усиление "), 2, 1);
    layout->addWidget(m_gain, 2, 2);

    SetTopLayout(layout);
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}
