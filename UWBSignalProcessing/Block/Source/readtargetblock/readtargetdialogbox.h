#ifndef READTARGETDIALOGBOX_H
#define READTARGETDIALOGBOX_H

#include "abstractdialogcommand.h"
#include <QSpinBox>

class ReadTargetDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  ReadTargetDialogBox();
  ~ReadTargetDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  std::string GetPathFile() const;

private slots:
  void OpenFile();

private:
  void CreateWidget();
  //    текстовое поля для пути к файлу
  QLineEdit *m_textPath = nullptr;
  QPushButton *m_openPath = nullptr;
};

#endif // READTARGETDIALOGBOX_H
