#include "twogaindialogbox.h"
#include <qmath.h>

TwoGainDialogBox::TwoGainDialogBox()
{
    CreateWidget();
    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

int TwoGainDialogBox::GetIndexOtherGainSignal() const
{
    return m_indexOtherGain->value();
}

void TwoGainDialogBox::CreateWidget()
{
    auto mainLayout = new QHBoxLayout;
    m_indexOtherGain = new QSpinBox;
    m_indexOtherGain->setRange(0, 8192);
    mainLayout->addWidget(new QLabel("Индекс второго усиления - "));
    mainLayout->addWidget(m_indexOtherGain);
    SetTopLayout(mainLayout);
}

QList<std::pair<QString, QVariant> > TwoGainDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_indexOtherGain", m_indexOtherGain->value()));
    return listSetting;
}

bool TwoGainDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_indexOtherGain")
            m_indexOtherGain->setValue(parmSetting.second.toInt());

    }
    return true;
}
