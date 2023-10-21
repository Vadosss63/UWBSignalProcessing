#ifndef BINDIALOGBOX_H
#define BINDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QSpinBox>

class BinDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  BinDialogBox();
  ~BinDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  int GetShiftI() const;
  int GetShiftQ() const;
  int GetRangeUp() const;
  int GetRangeBottom() const;

private:
  QSpinBox *m_shiftI;
  QSpinBox *m_shiftQ;
  QSpinBox *m_rangeUp;
  QSpinBox *m_rangeBottom;
};

#endif // BINDIALOGBOX_H
