#include "costransferdialogbox.h"
#include "ui_costransferdialogbox.h"

CosTransferDialogBox::CosTransferDialogBox():
    ui(new Ui::CosTransferDialogBox)
{
    m_layout = new QWidget;
    ui->setupUi(m_layout);
    m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(m_layout, 0, 0 );
    SetTopLayout(m_gridLayout);
    connect(ui->dial, SIGNAL(valueChanged(int)), ui->lcd, SLOT(display(int)));
    ui->dial->setMaximum(249);
    ui->dial->setMinimum(1);
    
    connect(ui->pushButtonInc, SIGNAL(clicked()), this, SLOT(SetInc()));
    connect(ui->pushButtonDec, SIGNAL(clicked()), this, SLOT(SetDec()));
    connect(ui->silmpleFrequency500Mhz, SIGNAL(clicked()),this, SLOT(SetMaxRangeFrequecy500MHz()));
    connect(ui->silmpleFrequency2Hhz, SIGNAL(clicked()),this, SLOT(SetMaxRangeFrequecy2GHz()));
    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

CosTransferDialogBox::~CosTransferDialogBox()
{
    delete ui;
}

void CosTransferDialogBox::SetInc()
{
    if(ui->dial->value() != 1000)
        ui->dial->setValue(ui->dial->value() + 1);
}

void CosTransferDialogBox::SetDec()
{
    if(ui->dial->value() != 1)
        ui->dial->setValue(ui->dial->value() - 1);

}

void CosTransferDialogBox::SetMaxRangeFrequecy500MHz()
{
    ui->dial->setMaximum(249);
}

void CosTransferDialogBox::SetMaxRangeFrequecy2GHz()
{
    ui->dial->setMaximum(999);
}

QList<std::pair<QString, QVariant> > CosTransferDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("silmpleFrequency500Mhz", ui->silmpleFrequency500Mhz->isChecked()));
    listSetting.append(std::make_pair("dial", ui->dial->value()));
    listSetting.append(std::make_pair("sin", ui->sin->isChecked()));
    return listSetting;
}

bool CosTransferDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 3)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "silmpleFrequency500Mhz")
        {
            if(parmSetting.second.toBool())
            {
                ui->silmpleFrequency500Mhz->setChecked(true);
                SetMaxRangeFrequecy500MHz();
            }
            else
            {
                ui->silmpleFrequency2Hhz->setChecked(true);
                SetMaxRangeFrequecy2GHz();
            }
        }

        if(parmSetting.first == "dial")
            ui->dial->setValue(parmSetting.second.toInt());

        if(parmSetting.first == "sin")
        {
            parmSetting.second.toBool() ? ui->sin->setChecked(true) : ui->cos->setChecked(true);
        }

    }
    return true;

}

double CosTransferDialogBox::GetFrequency()
{
    return ui->lcd->intValue();
}

bool CosTransferDialogBox::GetPhase()
{
    return ui->sin->isChecked();
}

bool CosTransferDialogBox::GetSimpleFrequency()
{
    return ui->silmpleFrequency500Mhz->isChecked();
}
