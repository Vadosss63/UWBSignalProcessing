#ifndef GAINDIALODBOX_H
#define GAINDIALODBOX_H

#include <QCheckBox>
#include <QSpinBox>
#include "abstractdialogcommand.h"

class GainDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:
    GainDialogBox();
    ~GainDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    double GetGain();

signals:
    void ValueChanged(double);

protected slots:

    void ChangedQDial(int val);
    void SetIncDouble();
    void SetDecDouble();
    void SetSigned();

private:
    //    создание сетки
    QGridLayout* m_gridLayoout;
    //    название поля
    QLabel* m_label;
    //    для отображения информации
    QLCDNumber* m_QLCDNumber;
    //    для слайдера
    QDial* m_QDial;
    //    для увеличения на 0.1
    QPushButton* m_incButton;
    //    для уменьшения на 0.1
    QPushButton* m_decButton;
    // знак усиления
    QCheckBox* m_signed;
    // название SpinBox
    QLabel* m_labelSpinBox;
    // для отображения информации
    QSpinBox* m_spinBox;

    // десятичная часть
    uint8_t m_numderDouble = 0;
    // интовая часть
    uint8_t m_numderInt = 0;
    // знак усиления
    int8_t m_signedGain = 1;
};

#endif // GAINDIALODBOX_H
