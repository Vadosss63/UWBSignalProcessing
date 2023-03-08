#ifndef SYNTHETICAPERTUREDIALOGBOX_H
#define SYNTHETICAPERTUREDIALOGBOX_H

#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include "abstractdialogcommand.h"

class SyntheticApertureDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    SyntheticApertureDialogBox();
    ~SyntheticApertureDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    std::vector<size_t> GetIndexStop();

private:

    void DeleteWidget();
    void CreateWidget(int countShift);

    QList<QSpinBox*> m_stopIndex;
    QVBoxLayout* m_mainLayout = nullptr;

};

#endif // SYNTHETICAPERTUREDIALOGBOX_H

