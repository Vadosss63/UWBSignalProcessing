#ifndef TIMEDELAYDIALOGBOX_H
#define TIMEDELAYDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "customdialwidget.h"

class TimeDelayDialogBox: public AbstractDialogCommand
{

public:

    TimeDelayDialogBox();
    ~TimeDelayDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    int GetNumberOfZeros() const;

private:

    CustomDialWidget* m_widget;
    // создание сетки
    QGridLayout* m_gridLayoout;
};

#endif // TIMEDELAYDIALOGBOX_H
