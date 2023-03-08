#ifndef READCOMPLEXDIALOGBOX_H
#define READCOMPLEXDIALOGBOX_H

#include <QSpinBox>
#include "abstractdialogcommand.h"

class ReadComplexDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    ReadComplexDialogBox();
    ~ReadComplexDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    std::string GetPathFileSigI() const;
    std::string GetPathFileSigQ() const;

private slots:
    void OpenFile();

private:

    void CreateWidget();
    // текстовое поля для пути к файлу
    QLineEdit* m_textPathSigI = nullptr;
    QLineEdit* m_textPathSigQ = nullptr;
    QPushButton* m_openPathSigI = nullptr;
    QPushButton* m_openPathSigQ = nullptr;
};

#endif // READCOMPLEXDIALOGBOX_H
