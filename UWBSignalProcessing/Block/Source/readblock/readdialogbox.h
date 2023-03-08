#ifndef READDIALOGBOX_H
#define READDIALOGBOX_H

#include <QSpinBox>
#include "abstractdialogcommand.h"

class ReadDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    ReadDialogBox();
    ~ReadDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    std::string GetPathFile() const;

private slots:
    void OpenFile();

private:
    void CreateWidget();
    //    текстовое поля для пути к файлу
    QLineEdit* m_textPath = nullptr;
    QPushButton* m_openPath = nullptr;

};

#endif // READDIALOGBOX_H
