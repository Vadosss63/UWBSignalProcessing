#include "bindialogbox.h"

BinDialogBox::BinDialogBox()
{
    auto m_gridLayoout = new QGridLayout;
    m_gridLayoout->addWidget(new QLabel("Shift I:"), 0, 0);
    m_shiftI = new QSpinBox;
    m_shiftI->setRange(-100, 100);
    m_gridLayoout->addWidget(m_shiftI, 0, 1);

    m_gridLayoout->addWidget(new QLabel("Shift Q:"), 1, 0);
    m_shiftQ = new QSpinBox;
    m_shiftQ->setRange(-100, 100);
    m_gridLayoout->addWidget(m_shiftQ, 1, 1);

    m_gridLayoout->addWidget(new QLabel("Range Up:"), 2, 0);
    m_rangeUp = new QSpinBox;
    m_rangeUp->setRange(-128, 127);
    m_gridLayoout->addWidget(m_rangeUp, 2, 1);

    m_gridLayoout->addWidget(new QLabel("Range Bottom:"), 3, 0);
    m_rangeBottom = new QSpinBox;
    m_rangeBottom->setRange(-128, 127);
    m_gridLayoout->addWidget(m_rangeBottom, 3, 1);

    SetTopLayout(m_gridLayoout);

    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

QList<std::pair<QString, QVariant> > BinDialogBox::GetSetting()
{    
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_shiftI",  m_shiftI->value()));
    listSetting.append(std::make_pair("m_shiftQ",  m_shiftQ->value()));
    listSetting.append(std::make_pair("m_rangeUp",  m_rangeUp->value()));
    listSetting.append(std::make_pair("m_rangeBottom",  m_rangeBottom->value()));
    return listSetting;
}

bool BinDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 4)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_shiftI")
            m_shiftI->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "m_shiftQ")
            m_shiftQ->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "m_rangeUp")
            m_rangeUp->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "m_rangeBottom")
            m_rangeBottom->setValue(parmSetting.second.toInt());

    }
    return true;
}

int BinDialogBox::GetShiftI() const
{
    return m_shiftI->value();
}

int BinDialogBox::GetShiftQ() const
{
    return m_shiftQ->value();
}

int BinDialogBox::GetRangeUp() const
{
    return m_rangeUp->value();
}

int BinDialogBox::GetRangeBottom() const
{
    return m_rangeBottom->value();
}
