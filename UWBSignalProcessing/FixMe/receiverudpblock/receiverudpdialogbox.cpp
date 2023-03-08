#include "receiverudpdialogbox.h"
#include <math.h>

ReceiverUDPDialogBox::ReceiverUDPDialogBox(dspm::ReceiverUDPModule*parent, QWidget *wparent):
    AbstractDialogCommand(wparent),
    m_module(parent)
{
    m_gridLayoout = new QGridLayout;
    m_label = new QLabel("Коэфициент сжатия сигнала");
    m_QLCDNumber = new QLCDNumber;
    m_QDial = new QDial;
    m_QDial->setNotchesVisible(true);
    m_QDial->setRange(0, 3);
    m_gridLayoout->addWidget(m_label, 0, 0);
    m_gridLayoout->addWidget(m_QLCDNumber, 1, 0);
    m_gridLayoout->addWidget(m_QDial, 2, 0);
    SetTopLayout(m_gridLayoout);
    connect(m_QDial, SIGNAL(valueChanged(int)), this, SLOT(ChangedQDial(int)));
    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
    m_QDial->setValue(0);
    ChangedQDial(0);
}

ReceiverUDPDialogBox::~ReceiverUDPDialogBox()
{
}

void ReceiverUDPDialogBox::ChangedQDial(int val)
{
    m_QLCDNumber->display(std::pow(2, val));
}

void ReceiverUDPDialogBox::SendCommandAbstractModule()
{

    dspm::AbstractModule::PushCommand(
                new dspm::ReceiverUDPModule::ControlCommand(
                    m_module, m_QLCDNumber->value()));
    dspm::AbstractModule::ExecutePendingCommands();
}


QList<std::pair<QString, QVariant> > ReceiverUDPDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::pair<QString, QVariant>("m_QDial", m_QDial->value()));
    return listSetting;
}

bool ReceiverUDPDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
       if(parmSetting.first == "m_QDial")
       {

           m_QDial->setValue(parmSetting.second.toInt());

             ChangedQDial(parmSetting.second.toInt());
       }

    }
    return true;

}
