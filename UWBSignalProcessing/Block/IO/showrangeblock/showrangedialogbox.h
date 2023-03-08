#ifndef EXPDIALODBOX_H
#define EXPDIALODBOX_H

#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include "abstractdialogcommand.h"

class ShowRangeDialogBox: public AbstractDialogCommand
{

public:

    ShowRangeDialogBox();
    ~ShowRangeDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
    int GetRange();

private:
    void CreateWidget();
    QSpinBox* m_range = nullptr;
};

#endif // EXPDIALODBOX_H

