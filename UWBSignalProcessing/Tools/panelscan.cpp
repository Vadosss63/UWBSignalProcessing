#include "panelscan.h"
#include "ui_panelscan.h"

PanelScan::PanelScan(QWidget *parent) : QWidget(parent), ui(new Ui::PanelScan) {
  ui->setupUi(this);
  connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(Aplay()));
}

PanelScan::~PanelScan() { delete ui; }

Ui::PanelScan *PanelScan::getUi() const { return ui; }

float PanelScan::GetCoefStratch() { return m_coef; }

float PanelScan::GetResolution() {
  if (getUi()->distanceShow->currentIndex() == 0)
    return 2.4f;
  if (getUi()->distanceShow->currentIndex() == 1)
    return 1.2f;
  if (getUi()->distanceShow->currentIndex() == 2)
    return 0.6f;
  if (getUi()->distanceShow->currentIndex() == 3)
    return 0.3f;
  return 0;
}

void PanelScan::SetRange(uint32_t maxVal) {

  if (614 > maxVal) {
    if (ui->distanceShow->currentIndex() != 3) {
      ui->distanceShow->setCurrentIndex(3);
    }
    return;
  }
  if (1229 > maxVal) {
    if (ui->distanceShow->currentIndex() != 2) {
      ui->distanceShow->setCurrentIndex(2);
    }

    return;
  }

  if (2458 > maxVal) {
    if (ui->distanceShow->currentIndex() != 1) {
      ui->distanceShow->setCurrentIndex(1);
    }

    return;
  }
  if (4915 > maxVal) {
    if (ui->distanceShow->currentIndex() != 0) {
      ui->distanceShow->setCurrentIndex(0);
    }

    return;
  }
}

void PanelScan::Aplay() {
  m_coef = std::pow(2, getUi()->distance->currentIndex());
}
