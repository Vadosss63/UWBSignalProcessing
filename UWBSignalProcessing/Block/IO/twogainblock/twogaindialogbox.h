#ifndef TWOGAINDIALODBOX_H
#define TWOGAINDIALODBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QSpinBox>

class TwoGainDialogBox : public AbstractDialogCommand {

public:
  TwoGainDialogBox();
  ~TwoGainDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
  int GetIndexOtherGainSignal() const;

private:
  void CreateWidget();
  QSpinBox *m_indexOtherGain = nullptr;
};

#endif // TWOGAINDIALODBOX_H
