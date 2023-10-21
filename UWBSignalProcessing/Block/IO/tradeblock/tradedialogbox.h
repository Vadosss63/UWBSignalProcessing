#ifndef TRADEDIALODBOX_H
#define TRADEDIALODBOX_H

#include "abstractdialogcommand.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

class TradeDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  TradeDialogBox();
  ~TradeDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
  std::vector<double> GetShiftBoard();

protected slots:
  void ChangeCountShift(int index);

private:
  void DeleteWidget();
  void CreateWidget(int countShift);

  QList<QDoubleSpinBox *> m_shiftBoards;
  QVBoxLayout *m_mainLayout = nullptr;
  QComboBox *m_countShift;
};

#endif // TRADEDIALODBOX_H
