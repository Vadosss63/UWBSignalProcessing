#ifndef SCOPEBUFFERDIALODBOX_H
#define SCOPEBUFFERDIALODBOX_H

#include "changeevent.h"
#include "dialogwindow.h"
#include "plotter.h"
#include "plotter_interface.h"
#include <QComboBox>
#include <QDial>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QSpinBox>

class SettingScopeDialog;

class ScopeBufferDialogBox : public QDialog, public DialogWindow {
  Q_OBJECT
public:
  ScopeBufferDialogBox(CSignalPlotter *parent, QWidget *wparent = nullptr);
  ~ScopeBufferDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  void SendCommand() override;
  void ShowDialog() override;

  int GetMagnitudeDimention() const;
  void SetMagnitudeDimention(int magnitudeDimention);

  int GetTimeDimention() const;
  void SetTimeDimention(int timeDimention);

  TypeX GetTypeX() const;
  void SetTypeX(TypeX typeX);

  int GetVal() const;
  void SetVal(int val);

  QString GetValueName() const;
  void SetValueName(const QString &valueName);

  void Attach(ChangeEvent *observer) override;
  void Notification() override;

private slots:
  void SetMagnitude(int Ymax);
  void SetTime(int Xmax);
  void ShowDialogSetting();

private:
  void SetPliting();

  Plotter *m_memoryScope;
  //    для слайдера
  QDial *m_dialMagnitudeDimention;
  //    для слайдера
  QDial *m_dialTimeDimention;
  QLCDNumber *m_LCDMagnitude;
  QLCDNumber *m_LCDTime;
  SettingScopeDialog *m_settingScopeDialog;
  ChangeEvent *m_observer = nullptr;
};

class SettingScopeDialog : public QDialog {
  Q_OBJECT
public:
  SettingScopeDialog(QWidget *parent = nullptr);
  ~SettingScopeDialog() = default;

  TypeX GetTypeX() const;
  void SetTypeX(TypeX typeX);

  int GetVal() const;
  void SetVal(int val);

  QString GetValueName() const;
  void SetValueName(const QString &valueName);

public slots:
  void ChangeType(int val);

private:
  QDialogButtonBox *m_dialogButtonBox;
  QComboBox *m_typeComboBox;
  QSpinBox *m_valueSpinBox;
  QComboBox *m_valueNameComboBox;
  QLabel *valueLabel;
};

#endif // SCOPEBUFFERDIALODBOX_H
