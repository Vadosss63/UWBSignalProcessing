#include "panelscan.h"
#include "ui_panelscan.h"

PanelScan::PanelScan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelScan)
{
    ui->setupUi(this);
}

PanelScan::~PanelScan()
{
    delete ui;
}

Ui::PanelScan *PanelScan::GetUi() const
{
    return ui;
}

void PanelScan::DumpEvent(qreal bering, qreal range)
{
    ui->labelRange->setText(QString::number(range, 3, 2));
    ui->labelAngel->setText(QString::number(bering, 3, 2));
}

uint8_t PanelScan::GetCoefStreatch() const
{
    return static_cast<uint8_t>(std::pow(2, ui->distance->currentIndex()));
}

int PanelScan::GetVerticalShift() const
{
    return ui->threshold->value();
}

int PanelScan::GetGain() const
{
    return ui->gan->value();
}

bool PanelScan::GetUpdateScan() const
{
    return ui->isScanMode->isChecked();
}

int PanelScan::GetUpdateStep() const
{
    return ui->updateStep->value();
}

bool PanelScan::GetIsColorView() const
{
    return ui->isColorScan->isChecked();
}

bool PanelScan::GetIsScanLine() const
{
    return ui->isLineOnScan->isChecked();
}

bool PanelScan::GetIsGridOnScan() const
{
    return ui->isGridOnScan->isChecked();
}

void PanelScan::SetCoefStreatch(uint8_t coef)
{
    int index = static_cast<int>(std::log2(coef));
    ui->distance->setCurrentIndex(index);
}

void PanelScan::SetVerticalShift(int shift)
{
    ui->threshold->setValue(shift);
}

void PanelScan::SetGain(int gain)
{
    ui->gan->setValue(gain);
}

void PanelScan::SetUpdateScan(bool isUpdateScan)
{
    ui->isScanMode->setChecked(isUpdateScan);
}

void PanelScan::SetUpdateStep(int updateStep)
{
    ui->updateStep->setValue(updateStep);
}

void PanelScan::SetIsGridOnScan(bool isGridOnScan)
{
    ui->isGridOnScan->setChecked(isGridOnScan);
}

void PanelScan::SetColorView(bool isColorView)
{
    ui->isColorScan->setChecked(isColorView);
}

void PanelScan::SetIsScanLine(bool isScanLine)
{
    ui->isLineOnScan->setChecked(isScanLine);
}

void PanelScan::SetRange(uint32_t maxVal)
{    
    if(614 > maxVal)
    {
        if(ui->distanceShow->currentIndex() != 3)
            ui->distanceShow->setCurrentIndex(3);

        return;
    }
    if(1229 > maxVal)
    {
        if(ui->distanceShow->currentIndex() != 2)
            ui->distanceShow->setCurrentIndex(2);


        return;
    }

    if(2458 > maxVal)
    {
        if(ui->distanceShow->currentIndex() != 1)
            ui->distanceShow->setCurrentIndex(1);

        return;
    }

    if(4915 > maxVal)
    {
        if(ui->distanceShow->currentIndex() != 0)
            ui->distanceShow->setCurrentIndex(0);
        return;
    }
}
