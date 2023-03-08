#ifndef ABSTRACTDIALOGCOMMAND_H
#define ABSTRACTDIALOGCOMMAND_H

#include <QDialog>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QDial>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QList>
#include <QString>

#include "changeevent.h"
#include "dialogwindow.h"

// абстрактный класс для создния диологовых окон
class AbstractDialogCommand : public QDialog, public DialogWindow
{
    Q_OBJECT
public:
    explicit AbstractDialogCommand(QWidget *parent = nullptr);
    ~AbstractDialogCommand() override;
    AbstractDialogCommand(const AbstractDialogCommand& rhs) = delete;
    AbstractDialogCommand& operator=(const AbstractDialogCommand& rhs) = delete;
    // добавления слоя для надстройки
    void SetTopLayout(QLayout *topLayout);
    // блокировка/разблокировка кнопок
    void SetEnabledOkButton(bool flag);
    void SetEnabledApplyButton(bool flag);

    void ShowDialog() override;
    void SendCommand() override;
    void Attach(ChangeEvent* observer) override;
    void Notification() override;

public slots:
    void SendCommandAbstractModule();

private:
    //  кнопка отменить
    QPushButton* m_cancelButton;
    // кнопка ок
    QPushButton* m_okButton;
    // кнопка принять
    QPushButton* m_applyButton;
    // область создания параметров
    QHBoxLayout* m_topLayout;
    // слой ддля кнопок деяйствия
    QHBoxLayout* m_buttonLayout;
    // главный слой
    QGridLayout* m_mainLayout;
    // за изменениями диалогового окна
    ChangeEvent* m_observer = nullptr;
};

#endif // ABSTRACTDIALOGCOMMAND_H
