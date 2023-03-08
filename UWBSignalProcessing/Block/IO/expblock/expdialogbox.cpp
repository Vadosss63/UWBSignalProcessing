#include "expdialogbox.h"
#include <qmath.h>

ExpDialogBox::ExpDialogBox()
{
    CreateWidget();
}

double ExpDialogBox::GetThreshold()
{
    return m_threshold->value();
}

QList<std::pair<QString, QVariant> > ExpDialogBox::GetSetting()
{    
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_threshold", m_threshold->value()));
    return listSetting;
}

bool ExpDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 1)
        return false;

    if(listSetting.first().first == "m_threshold")
        m_threshold->setValue(listSetting.first().second.toDouble());

    return true;
}

void ExpDialogBox::CreateWidget()
{
    auto mainLayout = new QHBoxLayout;
    m_threshold = new QDoubleSpinBox;
    m_threshold->setRange(-10000, 10000);
    m_threshold->setDecimals(4);
    mainLayout->addWidget(new QLabel("Порог - "));
    mainLayout->addWidget(m_threshold);
    SetTopLayout(mainLayout);
}
