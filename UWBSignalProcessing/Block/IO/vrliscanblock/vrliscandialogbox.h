#ifndef VRLISCANDIALOGBOX_H
#define VRLISCANDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QComboBox>

class VRLIScanDialogBox : public AbstractDialogCommand {
public:
  VRLIScanDialogBox();
  ~VRLIScanDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
  bool GetCreateVRLI();

private:
  void CreateWidget();
  //создание сетки
  QGridLayout *m_gridLayout = nullptr;
  QComboBox *m_isPRLI = nullptr;
};

#endif // ADCNEWDIALOGBOX_H
