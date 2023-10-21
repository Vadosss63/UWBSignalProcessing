#ifndef TESTFILTERDIALOGBOX_H
#define TESTFILTERDIALOGBOX_H

#include "abstractdialogcommand.h"

namespace Ui {
class TestFilterDialogBox;
}

class TestFilterDialogBox : public AbstractDialogCommand {
  Q_OBJECT

public:
  TestFilterDialogBox();
  ~TestFilterDialogBox() override;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  uint8_t GetInputPortsNumber();
  uint8_t GetOutputPortsNumber();
  bool GetPortState() const;

  double GetSampleFreq();
  double GetCutFreq1();
  double GetCutFreq2();
  int GetOrder();
  int GetFilterType();

protected slots:
  void FilterDialogSet(const int &type);
  void NumberPorts();

private:
  //Функции настройки диалогового окна
  void DialogSet(int type);
  //Настройка для каждого типа фильтра
  void LowTypeSet();
  void HightTypeSet();
  void BandPassTypeSet();
  void BandStopTypeSet();
  void HilbertTypeSet();
  void DiffTypeSet();
  void MultibandTypeSet();

  //    создание сетки
  QGridLayout *m_gridLayout;
  //Загрузка формы
  Ui::TestFilterDialogBox *ui;
  // для добавления виждета

  QWidget *m_layout;
  bool port_state = false;
};

#endif // TESTFILTERDIALOGBOX_H
