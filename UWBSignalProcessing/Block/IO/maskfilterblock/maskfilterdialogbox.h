#ifndef MASKFILTERDIALOGBOX_H
#define MASKFILTERDIALOGBOX_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <QMessageBox>
#include "abstractdialogcommand.h"

class MaskFilterDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    MaskFilterDialogBox();
    ~MaskFilterDialogBox() override = default;
    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    const std::vector<double>& GetFilterArray() const;

private slots:
    void OpenFile();

private:

    //    текстовое поля для пути к файлу
    QLineEdit* m_textLine;
    QPushButton* m_button;
    std::vector<double> m_filterArray;
};

#endif // MASKFILTERDIALOGBOX_H
