#ifndef AVTOELECTRONICADIALOGBOX_H
#define AVTOELECTRONICADIALOGBOX_H

#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include "abstractdialogcommand.h"

class AvtoelectronicaDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:
    AvtoelectronicaDialogBox();
    ~AvtoelectronicaDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    size_t GetNumberRange();
    size_t GetCountAcumm();
    size_t GetRangeAcumm();

    double GetThresholAcumm();
    double GetThresholAcummDynamic();

    double GetBaseAntenna();
    int GetMaxSpeed();
    double GetGain();
    double GetGainDynamic();
    bool IsVARU();

private:

    void CreateWidget();

    QSpinBox* m_countAcumm;
    QSpinBox* m_numberRange;
    QSpinBox* m_range;
    QSpinBox* m_threshol;
    QSpinBox* m_thresholDynamic;
    QDoubleSpinBox* m_baseAntenna;
    QSpinBox* m_maxSpeed;
    QCheckBox* m_varu;
    QDoubleSpinBox* m_gain;
    QDoubleSpinBox* m_gainDynamic;
};

#endif // AVTOELECTRONICADIALOGBOX_H

