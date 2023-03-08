#ifndef STATISTICALESTIMATEDIALOGBOX_H
#define STATISTICALESTIMATEDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "showstatisticalestimate.h"
#include <QSpinBox>

class StatisticaleStimateDialogBox: public AbstractDialogCommand, public ShowStatisticaleStimate
{
    Q_OBJECT

public:

    StatisticaleStimateDialogBox();
    ~StatisticaleStimateDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    void ShowStimate(double mean, double meanValue, double variance, double osv, double osv_to_meanValue) override;

    size_t GetStartIndex() const;
    size_t GetStopIndex() const;

private:

    void CreateWidget();

    QSpinBox* m_startIndex = nullptr;
    QSpinBox* m_stopIndex = nullptr;
    QLabel* m_textMean = nullptr;
    QLabel* m_textMeanValue = nullptr;
    QLabel* m_textVariance = nullptr;
    QLabel* m_textOSV = nullptr;
    QLabel* m_textOSV_to_meanValue = nullptr;

};

#endif // STATISTICALESTIMATEDIALOGBOX_H
