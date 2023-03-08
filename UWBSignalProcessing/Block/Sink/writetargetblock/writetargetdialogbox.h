#ifndef WRITETARGETDIALOGBOX_H
#define WRITETARGETDIALOGBOX_H

#include "abstractdialogcommand.h"

class WriteTargetDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:
    
    WriteTargetDialogBox();
    ~WriteTargetDialogBox() override = default;
    
    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;
    
    std::string GetPathFile() const;
    
private slots:
    void OpenFile();
    
private:
    
    void TrimmedPath(QString& path);
    //    текстовое поля для пути к файлу
    QLineEdit* m_textLine;
    QPushButton* m_button;
};

#endif // WRITETARGETDIALOGBOX_H
