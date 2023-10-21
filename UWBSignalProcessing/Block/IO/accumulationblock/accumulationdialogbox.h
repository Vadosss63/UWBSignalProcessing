#ifndef ACCUMULATIONDIALOGBOX_H
#define ACCUMULATIONDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "accumulationmodule.h"
#include "customdialwidget.h"
#include <QComboBox>

class AccumulationDialogBox : public AbstractDialogCommand {

public:
  AccumulationDialogBox();
  ~AccumulationDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
  size_t GetSizeBuf();
  TypeAccumulation GetTypeAccum();

private:
  CustomDialWidget *m_widget;
  QGridLayout *m_gridLayoout;
  QComboBox *m_comboBox;
};

#endif // ACCUMULATIONDIALOGBOX_H
