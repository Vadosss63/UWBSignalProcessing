#ifndef RANGEACCUMDIALOGBOX_H
#define RANGEACCUMDIALOGBOX_H

#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "abstractdialogcommand.h"

class RangeAccumDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:
    RangeAccumDialogBox();
    ~RangeAccumDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    size_t GetNumberRange();
    size_t GetCountAcumm();
    size_t GetRangeAcumm();

    double GetThresholAcumm();

    int GetWindow();
    int GetThresholType();
    double GetGain();
    bool IsVARU();

private:

    void CreateWidget();

    QSpinBox* m_countAcumm = nullptr;
    QSpinBox* m_numberRange = nullptr;
    QSpinBox* m_range = nullptr;
    QSpinBox* m_threshol = nullptr;
    QComboBox* m_windows = nullptr;
    QComboBox* m_thresholType = nullptr;
    QCheckBox* m_varu = nullptr;
    QDoubleSpinBox* m_gain = nullptr;

};

#endif // RANGEACCUMDIALOGBOX_H

