#ifndef COSTRANSFERDIALOGBOX_H
#define COSTRANSFERDIALOGBOX_H

#include "abstractdialogcommand.h"

namespace Ui {
class CosTransferDialogBox;
}

class CosTransferDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  CosTransferDialogBox();
  ~CosTransferDialogBox() override;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  double GetFrequency();
  bool GetPhase();
  bool GetSimpleFrequency();

protected slots:
  void SetInc();
  void SetDec();

  void SetMaxRangeFrequecy500MHz();
  void SetMaxRangeFrequecy2GHz();

private:
  //    создание сетки
  QGridLayout *m_gridLayout;
  // загрузка формы диоллогового окна
  Ui::CosTransferDialogBox *ui;
  // для добавления виждета
  QWidget *m_layout;
};

#endif // COSTRANSFERDIALOGBOX_H
