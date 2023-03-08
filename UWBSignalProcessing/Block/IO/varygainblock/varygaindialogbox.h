#ifndef GAINDIALODBOX_H
#define GAINDIALODBOX_H

#include <QCheckBox>
#include <QSpinBox>
#include "abstractdialogcommand.h"

class VaryGainDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    VaryGainDialogBox();
    ~VaryGainDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    size_t GetIndex();

private:
    //    создание сетки
    QGridLayout* m_gridLayoout;
    //    название поля
    QLabel* m_label;
    //    для отображения информации
    QLCDNumber* m_QLCDNumber;
    //    для слайдера
    QDial* m_QDial;

};

#endif // GAINDIALODBOX_H
