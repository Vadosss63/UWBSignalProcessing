#ifndef ARITHMETICDIALOGBOX_H
#define ARITHMETICDIALOGBOX_H

#include <QComboBox>
#include <QString>

#include "abstractdialogcommand.h"
#include "arithmeticstate.h"

class ArithmeticDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    ArithmeticDialogBox();
    ~ArithmeticDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    std::string GetState() const;
    QString GetTitleState() const;

private:

    void CreateWidget();

    QComboBox* m_arithmeticState = nullptr;
};

#endif // ARITHMETICDIALOGBOX_H
