#ifndef ACCUMULATIONDIALOGBOX_DUAL_H
#define ACCUMULATIONDIALOGBOX_DUAL_H

#include <QComboBox>
#include "abstractdialogcommand.h"
#include "customdialwidget.h"
#include "accumulationdualmodule.h"

class AccumulationDialogBoxDual : public AbstractDialogCommand
{
    Q_OBJECT
public:
    AccumulationDialogBoxDual(AccumulationDualModule *parent, QWidget *wparent = nullptr);
    ~AccumulationDialogBoxDual() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

protected slots:

    void SendCommandAbstractModule();
    void ChangeValCountAcc();

private:

    CustomDialWidget* m_widget;
    CustomDialWidget* m_widget2;
    QGridLayout* m_gridLayoout;
    AccumulationDualModule * m_module;
};

#endif // ACCUMULATIONDIALOGBOX_DUAL_H
