#ifndef UPSAMPLINGDIALOGBOX_H
#define UPSAMPLINGDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QComboBox>

class UpsamplingDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  UpsamplingDialogBox();
  ~UpsamplingDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  int GetUpsamplingValue() const;

protected slots:
  void SetN(int index);

private:
  void CreateFreqBox(QGridLayout *layout);

  QComboBox *m_box = nullptr;
  // создание сетки
  QGridLayout *m_gridLayoout = nullptr;
  //Коэффициент повышения частоты дискретизации
  int m_sampl = 4;
};

#endif // UPSAMPLINGDIALOGBOX_H
