#ifndef SOURCEGAUSDIALOGBOX_H
#define SOURCEGAUSDIALOGBOX_H

#include <QComboBox>
#include "abstractdialogcommand.h"
#include "customdialwidget.h"

class SourceGausDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    SourceGausDialogBox();
    ~SourceGausDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    int GetLenght();
    int GetAmplitude();
    int GetSigma();
    int GetExpectation();

protected slots:
    void SetMaxRangeFrequecy(int index);

private:

    void CreateExpectationWidget();
    void CreateAmplitudeWidget();
    void CreateSigmaWidget();
    void CreateSTWidget(QGridLayout *layoutSampleTime);

    CustomDialWidget* m_expectation = nullptr;
    CustomDialWidget* m_amplitude = nullptr;
    CustomDialWidget* m_sigma = nullptr;
    QComboBox* m_comboBoxTS = nullptr;    // Частота дискретизации (Гц)
};

#endif // SOURCEGAUSDIALOGBOX_H
