#ifndef VARYDIALOGBOX_H
#define VARYDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class VARYDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  VARYDialogBox();
  ~VARYDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
  const std::vector<double> &GetVARYCoeff() const;

private slots:
  void OpenFile();

private:
  QLineEdit *m_path;
  QPushButton *m_openButton;
  std::vector<double> m_varyCoeff;
};

#endif // VARYDIALOGBOX_H
