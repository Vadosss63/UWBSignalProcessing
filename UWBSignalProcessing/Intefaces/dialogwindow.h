#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QList>
#include <QString>

class ChangeEvent;
class QVariant;

class DialogWindow {
public:
  virtual ~DialogWindow() = default;
  // Сделать рефакторинг сокрытия настроек через итераторы
  virtual QList<std::pair<QString, QVariant>> GetSetting() = 0;
  virtual bool SetSetting(QList<std::pair<QString, QVariant>>) = 0;
  virtual void ShowDialog() = 0;
  virtual void SendCommand() = 0;
  // добавление наблюдателя за событием
  virtual void Attach(ChangeEvent *observer) = 0;
  // оповещение наблюдателя при каком любо изменении изменении
  virtual void Notification() = 0;
};

#endif // DIALOGWINDOW_H
