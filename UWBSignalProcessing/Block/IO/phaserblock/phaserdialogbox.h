#ifndef PhaserDialodBox_H
#define PhaserDialodBox_H

#include "abstractdialogcommand.h"
#include "customdialwidget.h"

class PhaserDialogBox : public AbstractDialogCommand {

public:
  PhaserDialogBox();
  ~PhaserDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  int GetPhase();

private:
  //    создание сетки
  QGridLayout *m_gridLayoout;
  CustomDialWidget *m_widget;
};

#endif // PhaserDialodBox_H
