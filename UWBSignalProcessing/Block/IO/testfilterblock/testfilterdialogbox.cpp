#include "testfilterdialogbox.h"
#include "ui_testfilterdialogbox.h"

TestFilterDialogBox::TestFilterDialogBox()
    : ui(new Ui::TestFilterDialogBox)

{
  m_layout = new QWidget;
  ui->setupUi(m_layout);
  m_gridLayout = new QGridLayout;
  m_gridLayout->addWidget(m_layout, 0, 0);
  SetTopLayout(m_gridLayout);

  ui->FreqCut_2_SpinBox->hide();
  ui->Fc2_label_3->hide();

  //Настройки вида диалогового окна для каждого типа фильтров
  connect(ui->FilterType_comboBox, SIGNAL(activated(int)), this,
          SLOT(FilterDialogSet(int)));

  //Изменение количества выходов
  connect(ui->Port_spinBox, SIGNAL(valueChanged(int)), this,
          SLOT(NumberPorts()));

  // фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

TestFilterDialogBox::~TestFilterDialogBox() { delete ui; }

uint8_t TestFilterDialogBox::GetInputPortsNumber() {
  port_state = false;
  return static_cast<uint8_t>(ui->Port_spinBox->value());
}

uint8_t TestFilterDialogBox::GetOutputPortsNumber() {
  return static_cast<uint8_t>(ui->Port_spinBox->value());
}

void TestFilterDialogBox::FilterDialogSet(const int &type) { DialogSet(type); }

void TestFilterDialogBox::NumberPorts() { port_state = true; }

double TestFilterDialogBox::GetSampleFreq() {
  switch (ui->Sample_Freq_comboBox->currentIndex()) {
  case 0:
    return 500 * 1E+6;
  case 1:
    return 1E+9;
  case 2:
    return 2E+9;
  }
  return 0;
}

double TestFilterDialogBox::GetCutFreq1() {
  return ui->FreqCut_1_SpinBox->value() * 1E+6;
}

double TestFilterDialogBox::GetCutFreq2() {
  return (ui->FreqCut_2_SpinBox->value()) * 1E+6;
}

int TestFilterDialogBox::GetOrder() { return ui->OrderspinBox->value(); }

bool TestFilterDialogBox::GetPortState() const { return port_state; }

int TestFilterDialogBox::GetFilterType() {
  return ui->FilterType_comboBox->currentIndex();
}

void TestFilterDialogBox::DialogSet(int type) {
  switch (type) {
  case 0:
    LowTypeSet();
    break;
  case 1:
    HightTypeSet();
    break;
  case 2:
    BandPassTypeSet();
    break;
  case 3:
    BandStopTypeSet();
    break;
  case 4:
    HilbertTypeSet();
    break;
  }
}

void TestFilterDialogBox::LowTypeSet() {
  ui->CutFreq_groupBox->setVisible(true);
  ui->FreqCut_2_SpinBox->setVisible(false);
  ui->Fc2_label_3->setVisible(false);
  ui->Fc1_label->setText("Fc, МГц");
}

void TestFilterDialogBox::HightTypeSet() {
  ui->CutFreq_groupBox->setVisible(true);
  ui->FreqCut_2_SpinBox->setVisible(false);
  ui->Fc2_label_3->setVisible(false);
  ui->Fc1_label->setText("Fc, МГц");
}

void TestFilterDialogBox::BandPassTypeSet() {
  ui->CutFreq_groupBox->setVisible(true);
  ui->Fc1_label->setText("Fc1, МГц");
  ui->Fc2_label_3->setText("Fc2, МГц");
  ui->FreqCut_2_SpinBox->setVisible(true);
  ui->Fc2_label_3->setVisible(true);
}

void TestFilterDialogBox::BandStopTypeSet() {

  ui->CutFreq_groupBox->setVisible(true);
  ui->Fc1_label->setText("Fc1, МГц");
  ui->Fc2_label_3->setText("Fc2, МГц");
  ui->FreqCut_2_SpinBox->setVisible(true);
  ui->Fc2_label_3->setVisible(true);
}

void TestFilterDialogBox::HilbertTypeSet() {
  ui->CutFreq_groupBox->setVisible(false);
  ui->Fc1_label->setText("Fc1, МГц");
  ui->Fc2_label_3->setText("Fc2, МГц");
  ui->FreqCut_2_SpinBox->setVisible(false);
  ui->Fc2_label_3->setVisible(false);
}

QList<std::pair<QString, QVariant>> TestFilterDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;

  listSetting.append(std::make_pair("FilterType_comboBox",
                                    ui->FilterType_comboBox->currentIndex()));
  listSetting.append(std::make_pair("Port_spinBox", ui->Port_spinBox->value()));
  listSetting.append(std::make_pair("OrderspinBox", ui->OrderspinBox->value()));
  listSetting.append(std::make_pair("Sample_Freq_comboBox",
                                    ui->Sample_Freq_comboBox->currentIndex()));
  listSetting.append(
      std::make_pair("FreqCut_1_SpinBox", ui->FreqCut_1_SpinBox->value()));
  listSetting.append(
      std::make_pair("FreqCut_2_SpinBox", ui->FreqCut_2_SpinBox->value()));
  return listSetting;
}

bool TestFilterDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 6)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "FilterType_comboBox")
      ui->FilterType_comboBox->setCurrentIndex(parmSetting.second.toInt());
    if (parmSetting.first == "Port_spinBox")
      ui->Port_spinBox->setValue(parmSetting.second.toInt());
    if (parmSetting.first == "OrderspinBox")
      ui->OrderspinBox->setValue(parmSetting.second.toInt());
    if (parmSetting.first == "Sample_Freq_comboBox")
      ui->Sample_Freq_comboBox->setCurrentIndex(parmSetting.second.toInt());
    if (parmSetting.first == "FreqCut_1_SpinBox")
      ui->FreqCut_1_SpinBox->setValue(parmSetting.second.toDouble());
    if (parmSetting.first == "FreqCut_2_SpinBox")
      ui->FreqCut_2_SpinBox->setValue(parmSetting.second.toDouble());
  }
  FilterDialogSet(ui->FilterType_comboBox->currentIndex());
  return true;
}
