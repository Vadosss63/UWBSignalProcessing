#ifndef RECEIVERUDPDIALOGBOX_H
#define RECEIVERUDPDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "receiverudpmodule.h"
#include <QCheckBox>
#include <QSpinBox>

// Класс осуществяющий взаимодействие между пользователем и GainModule
// Содержит в себе графическое диологовое окно, предоставляющее пользователю
// возможность изменять парамерты GainModule

class ReceiverUDPDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  ReceiverUDPDialogBox(dspm::ReceiverUDPModule *parent,
                       QWidget *wparent = nullptr);
  ~ReceiverUDPDialogBox() override;
  ReceiverUDPDialogBox(const ReceiverUDPDialogBox &rhs) = delete;
  ReceiverUDPDialogBox &operator=(const ReceiverUDPDialogBox &rhs) = delete;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

protected slots:

  void ChangedQDial(int val);
  void SendCommandAbstractModule() override;

private:
  //    создание сетки
  QGridLayout *m_gridLayoout;
  //    название поля
  QLabel *m_label;
  //    для отображения информации
  QLCDNumber *m_QLCDNumber;
  //    для слайдера
  QDial *m_QDial;
  dspm::ReceiverUDPModule *m_module;
};

#endif // RECEIVERUDPDIALOGBOX_H
