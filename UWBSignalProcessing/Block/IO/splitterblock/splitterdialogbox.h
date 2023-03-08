#ifndef SPLITTERDIALOGBOX_H
#define SPLITTERDIALOGBOX_H

#include <QSpinBox>
#include "abstractdialogcommand.h"

class SplitterDialogBox: public AbstractDialogCommand
{
public:

    SplitterDialogBox();
    ~SplitterDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    uint8_t GetOutputPortsNumber();

private:

    // создание сетки
    QGridLayout* m_gridLayoout;
    // название поля
    QLabel* m_label;
    // для отображения информации
    QSpinBox* m_spinBox;

};

#endif // SPLITTERDIALOGBOX_H
