#include "sqrtdialogbox.h"

SqrtDialogBox::SqrtDialogBox()
{
    auto m_gridLayoout = new QGridLayout;
    misSqrtCB = new QCheckBox("Извлекать корень");
    misSqrtCB->setChecked(true);
    m_gridLayoout->addWidget(misSqrtCB);

    SetTopLayout(m_gridLayoout);

    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

bool SqrtDialogBox::GetIsSqrt() const
{
    return misSqrtCB->isChecked();
}

QList<std::pair<QString, QVariant> > SqrtDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_checkBox",  misSqrtCB->isChecked()));
    return listSetting;
}

bool SqrtDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_checkBox")
            misSqrtCB->setChecked(parmSetting.second.toBool());
    }
    return true;
}
