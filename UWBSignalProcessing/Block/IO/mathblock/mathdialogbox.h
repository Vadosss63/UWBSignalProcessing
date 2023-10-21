#ifndef MATHDIALOGBOX_H
#define MATHDIALOGBOX_H

#include <QComboBox>
#include <QString>

#include "abstractdialogcommand.h"
#include "mathstate.h"

class MathDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  MathDialogBox();
  ~MathDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  std::string GetMathState() const;
  QString GetTitleState() const;

private:
  void CreateWidget();

  QComboBox *m_mathState = nullptr;
};

#endif // UPSAMPLINGDIALOGBOX_H
