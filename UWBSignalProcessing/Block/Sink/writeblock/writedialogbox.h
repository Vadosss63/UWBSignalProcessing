#ifndef WRITEDIALOGBOX_H
#define WRITEDIALOGBOX_H

#include <QComboBox>
#include "abstractdialogcommand.h"
#include "typesignal.h"

class WriteDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:

    WriteDialogBox();
    ~WriteDialogBox() override = default;

    QList<std::pair<QString, QVariant> > GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant> > listSetting) override;

    std::string GetPathFile() const;
    TypeSignal GetTypeWrite() const;

private slots:
    void OpenFile();

private:

    //Удаление льшней информации
    void TrimmedPath(QString& path);
    // Создание виджета
    void CreateWidget();
    // тип сигнала
    QComboBox* m_typeSignal_CB = nullptr;
    // текстовое поля для пути к файлу
    QLineEdit* m_pathFile_TL = nullptr;
    QPushButton* m_openFileButton = nullptr;
};

#endif // WRITEDIALOGBOX_H
