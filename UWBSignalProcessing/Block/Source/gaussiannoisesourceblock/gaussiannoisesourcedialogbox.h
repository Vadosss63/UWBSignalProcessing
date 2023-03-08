#ifndef GAUSSIANNOISESOURCEDIALODBOX_H
#define GAUSSIANNOISESOURCEDIALODBOX_H

#include <QComboBox>
#include "abstractdialogcommand.h"
#include "customdialwidget.h"

class GaussianNoiseSourceDialogBox : public AbstractDialogCommand
{

public:

    GaussianNoiseSourceDialogBox();
    ~GaussianNoiseSourceDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    size_t GetSignalSize();
    double GetDeviation();

private:

    void CreateWidget();
    CustomDialWidget* m_customDialWidget = nullptr;
    QComboBox* m_comboBoxTS = nullptr;
};

#endif // GAUSSIANNOISESOURCEDIALODBOX_H
