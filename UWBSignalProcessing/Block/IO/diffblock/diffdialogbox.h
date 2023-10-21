#ifndef DIFFDIALOGBOX_H
#define DIFFDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "customdialwidget.h"

class DiffDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  DiffDialogBox();
  ~DiffDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  int GetDeltaX();

private:
  CustomDialWidget *m_widget;
  // создание сетки
  QGridLayout *m_gridLayoout;
};

#endif // DIFFDIALOGBOX_H
