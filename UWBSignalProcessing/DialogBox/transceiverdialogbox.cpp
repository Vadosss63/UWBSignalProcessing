#include "transceiverdialogbox.h"
#include "ui_TransceiverDialogBox.h"

TransceiverDialogBox::TransceiverDialogBox(
    dspm::TransceiverControlModule *parent, QWidget *wparent)
    : AbstractDialogCommand(wparent), ui(new Ui::TransceiverDialogBox),
      m_module(parent) {
  m_layout = new QWidget;
  ui->setupUi(m_layout);
  m_gridLayout = new QGridLayout;
  m_gridLayout->addWidget(m_layout, 0, 0);
  SetTopLayout(m_gridLayout);

  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

TransceiverDialogBox::~TransceiverDialogBox() { delete ui; }

void TransceiverDialogBox::
    SendCommandAbstractModule() { /*
                                         if(ui->groupBox_AmpDm->isChecked()){
                                         dspm::AbstractModule::PushCommand(
                                                 new
                                     dspm::RecieverControlModule::DMAmplifierTuneControlCommand(
                                                    m_module,
                                     ui->lineEdit->text().toInt(),
                                     ConvertDmData() ));
                                         dspm::AbstractModule::ExecutePendingCommands();}

                                          if(ui->groupBox_AmpIfa->isChecked()){
                                         dspm::AbstractModule::PushCommand(
                                                 new
                                     dspm::RecieverControlModule::IFAmplifierTuneControlCommand(
                                                    m_module,
                                     ui->lineEdit->text().toInt(),
                                     ConvertOtherAmpData(ui->lcdNumber_AmpIfa->value())
                                     ));
                                         dspm::AbstractModule::ExecutePendingCommands();}

                                         if(ui->groupBox_AmpUsp1->isChecked()){
                                         dspm::AbstractModule::PushCommand(
                                                 new
                                     dspm::RecieverControlModule::USP1AmplifierTuneControlCommand(
                                                    m_module,
                                     ui->lineEdit->text().toInt(),
                                     ConvertOtherAmpData(ui->lcdNumber_AmpUsp1->value())
                                     ));
                                         dspm::AbstractModule::ExecutePendingCommands();}

                                         if(ui->groupBox_AmpUsp2->isChecked()){
                                         dspm::AbstractModule::PushCommand(
                                                 new
                                     dspm::RecieverControlModule::USP2AmplifierTuneControlCommand(
                                                    m_module,
                                     ui->lineEdit->text().toInt(),
                                     ConvertOtherAmpData(ui->lcdNumber_AmpUsp2->value())
                                     ));
                                         dspm::AbstractModule::ExecutePendingCommands();}

                                         dspm::AbstractModule::PushCommand(
                                                 new
                                     dspm::RecieverControlModule::SyncronizationTuneControlCommand(
                                                    m_module,
                                     ui->lineEdit->text().toInt(),
                                     ui->radioButton_OutSync->isChecked()));
                                         dspm::AbstractModule::ExecutePendingCommands();

                                         dspm::AbstractModule::PushCommand(
                                                 new
                                     dspm::RecieverControlModule::BandWidthTuneControlCommand(
                                                    m_module,
                                     ui->lineEdit->text().toInt(),
                                     ui->radioButton_500MHz->isChecked()));
                                         dspm::AbstractModule::ExecutePendingCommands();


                                 */
}
