#include "accumulationdualdialogbox.h"

AccumulationDialogBoxDual::AccumulationDialogBoxDual(AccumulationDualModule *parent, QWidget *wparent):
    AbstractDialogCommand(wparent), m_module(parent)
{
    m_gridLayoout = new QGridLayout;
    m_widget = new CustomDialWidget("Число накоплений сигнала");
    m_widget->setMaxVal(1024);
    m_widget->setMinVal(2);
    m_gridLayoout->addWidget(m_widget, 1, 1, 1, 1);
    m_widget2 = new CustomDialWidget("Число первых и последних накоплений сигнала (не задавать больше чем (число накоплений сигнала)/2)");
    m_widget2->setMaxVal(512);
    m_widget2->setMinVal(1);
    m_gridLayoout->addWidget(m_widget2, 3, 1, 1, 1);

    connect(m_widget, SIGNAL(ChangeVal()), this, SLOT(ChangeValCountAcc()));

    SetTopLayout(m_gridLayoout);

    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
    m_widget->setValue(256);
    m_widget2->setValue(2);
    SendCommandAbstractModule();
}

void AccumulationDialogBoxDual::SendCommandAbstractModule()
{
    m_module->SetSizeBuf(m_widget->getDisplayIntVal(), m_widget2->getDisplayIntVal());
}

void AccumulationDialogBoxDual::ChangeValCountAcc()
{
    m_widget2->setMaxVal(m_widget->getDisplayIntVal()/2);
}

QList<std::pair<QString, QVariant> > AccumulationDialogBoxDual::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_widget", m_widget->getDisplayIntVal()));
    listSetting.append(std::make_pair("m_widget2", m_widget2->getDisplayIntVal()));
    return listSetting;
}

bool AccumulationDialogBoxDual::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 2)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_widget")
            m_widget->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "m_widget2")
            m_widget2->setValue(parmSetting.second.toInt());
    }
    return true;
}








