#ifndef THRESHOLDCONSTDIALODBOX_H
#define THRESHOLDCONSTDIALODBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <qmath.h>

class ThresholdDialogBox : public AbstractDialogCommand {

public:
  ThresholdDialogBox();
  ~ThresholdDialogBox() override = default;
  ThresholdDialogBox(const ThresholdDialogBox &rhs) = delete;
  ThresholdDialogBox &operator=(const ThresholdDialogBox &rhs) = delete;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  double GetThreshold();

private:
  void CreateWidget();
  QDoubleSpinBox *m_threshold = nullptr;
};

#endif // THRESHOLDCONSTDIALODBOX_H
