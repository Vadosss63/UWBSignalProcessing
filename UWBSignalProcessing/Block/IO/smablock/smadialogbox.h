#ifndef SMADIALOGBOX_H
#define SMADIALOGBOX_H

#include "abstractdialogcommand.h"
#include "customdialwidget.h"

class SMADialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    SMADialogBox();
    ~SMADialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    int GetSizeWindow();

private:

    CustomDialWidget* m_widget;
    QGridLayout* m_gridLayout;    // создание сетки

};

#endif // SMADIALOGBOX_H
