#ifndef ACCUMPHASERDIALOGBOX_H
#define ACCUMPHASERDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "accumphasermodule.h"
#include "customdialwidget.h"

class AccumPhaserDialogBox : public AbstractDialogCommand {
public:
  AccumPhaserDialogBox();
  ~AccumPhaserDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  double GetPhase();
  uint16_t GetCountAccum();

private:
  QGridLayout *m_gridLayoout;
  CustomDialWidget *m_wPhase;
  CustomDialWidget *m_wCountAccum;
};

#endif // ACCUMPHASERDIALOGBOX_H
