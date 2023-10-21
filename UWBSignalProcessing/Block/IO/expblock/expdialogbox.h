#ifndef EXPDIALODBOX_H
#define EXPDIALODBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

class ExpDialogBox : public AbstractDialogCommand {

public:
  ExpDialogBox();
  ~ExpDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  double GetThreshold();

private:
  void CreateWidget();

  QDoubleSpinBox *m_threshold = nullptr;
};

#endif // EXPDIALODBOX_H
