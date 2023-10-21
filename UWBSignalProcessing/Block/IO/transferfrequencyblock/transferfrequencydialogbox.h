#ifndef TRANSFERFREQUENCYDIALOGBOX_H
#define TRANSFERFREQUENCYDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QComboBox>
#include <QDoubleSpinBox>

class TransferFrequencyDialogBox : public AbstractDialogCommand {

public:
  TransferFrequencyDialogBox();
  ~TransferFrequencyDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
  double GetSampFreq() const;
  double GetTransferFrequency() const;

private:
  void CreateFreqBox(QGridLayout *layout);

  QDoubleSpinBox *m_box = nullptr;
  QComboBox *m_sampFreq = nullptr;
  // создание сетки
  QGridLayout *m_gridLayoout = nullptr;
};

#endif // TRANSFERFREQUENCYDIALOGBOX_H
