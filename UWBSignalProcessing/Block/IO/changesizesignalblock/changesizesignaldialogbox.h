#ifndef CHANGESIZESIGNALDIALOGBOX_H
#define CHANGESIZESIGNALDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <qmath.h>

class ChangeSizeSignalDialogBox : public AbstractDialogCommand {

public:
  ChangeSizeSignalDialogBox();
  ~ChangeSizeSignalDialogBox() override = default;
  ChangeSizeSignalDialogBox(const ChangeSizeSignalDialogBox &rhs) = delete;
  ChangeSizeSignalDialogBox &
  operator=(const ChangeSizeSignalDialogBox &rhs) = delete;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  size_t GetStartIndex();
  size_t GetSiseSig();

private:
  void CreateWidget();

  QSpinBox *m_start = nullptr;
  QSpinBox *m_size = nullptr;
};

#endif // CHANGESIZESIGNALDIALOGBOX_H
