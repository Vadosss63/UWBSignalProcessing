#ifndef ACCUMULATIONDIALOGBOX_H
#define ACCUMULATIONDIALOGBOX_H

#include <QComboBox>
#include "abstractdialogcommand.h"
#include "customdialwidget.h"
#include "accumulationmodule.h"

class AccumulationDialogBox : public AbstractDialogCommand
{

public:
    AccumulationDialogBox();
    ~AccumulationDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    size_t GetSizeBuf();
    TypeAccumulation GetTypeAccum();

private:
    CustomDialWidget* m_widget;
    QGridLayout* m_gridLayoout;   
    QComboBox* m_comboBox;
};

#endif // ACCUMULATIONDIALOGBOX_H
