#ifndef SQRTDIALOGBOX_H
#define SQRTDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QSpinBox>

class SqrtDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  SqrtDialogBox();
  ~SqrtDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  bool GetIsSqrt() const;

private:
  QCheckBox *misSqrtCB;
};

#endif // SQRTDIALOGBOX_H
