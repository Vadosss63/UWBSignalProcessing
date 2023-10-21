#ifndef DRAWCURVER_H
#define DRAWCURVER_H

#include "abstractdialogcommand.h"
#include "customdialwidget.h"
#include <QComboBox>
#include <QSpinBox>
#include <QVector>

class FilterState;

class FilterDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  FilterDialogBox();
  ~FilterDialogBox() override;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  uint8_t GetInputPortsNumber();
  uint8_t GetOutputPortsNumber();
  bool GetPortState() const;

  double GetSampleFreq();
  double GetCutFreq1();
  double GetCutFreq2();

  int GetOrder();
  int GetFilterType();

  void AddState(FilterState *state);

private slots:
  void SelectState(int index);

private:
  friend class FilterState;
  void ChangeState(FilterState *state);
  FilterState *m_currentState = nullptr;
  QVector<FilterState *> m_filterStates;
  // выбор состояния генератора
  QComboBox *m_selectState = nullptr;
  QGridLayout *layout = nullptr;
};

class SettingSignal : public QWidget {
  Q_OBJECT
public:
  SettingSignal();
  ~SettingSignal() = default;

  int GetOrderFilter();
  double GetSamp();

  uint8_t GetInputPortsNumber();
  uint8_t GetOutputPortsNumber();

  bool GetPortState();

  QList<std::pair<QString, QVariant>> SaveSettings();
  void SetParameter(const QList<std::pair<QString, QVariant>> &param);

private:
  void CreateOrderFilter();
  void CreateSamp();
  void CreateCountPorts();
  void AddWidget(const QString &name, QWidget *widget);
  void AddLayout(const QString &name, QLayout *layout);

  QGridLayout *m_mainLayout;

  QSpinBox *m_orderFilter; // Период фильтра
  QSpinBox *m_countPorts;  // Количество портов
  QComboBox *m_sampUnit;   // Частота дискретизации

  int m_currentCount = 1;
};

// отбражение выбранного окна
class FilterState : public QWidget {
  Q_OBJECT
public:
  FilterState(QWidget *wparent = nullptr);
  ~FilterState() override;

  // получает параметры генератора
  virtual QList<std::pair<QString, QVariant>> SaveSettings() const;
  virtual void SetParameter(const QList<std::pair<QString, QVariant>> &);
  // имя гениратора для отображения
  virtual QString Name();
  // Изменения состояния
  void ChangeState(FilterDialogBox *dialog, FilterState *state);
  SettingSignal *GetSettingSignal() const;

  virtual double GetCutFreq1();
  virtual double GetCutFreq2();

  virtual int GetFilterType();

public slots:
  void ShowSettingFilter();

protected:
  QPushButton *buttonSettingSignal() const;

private:
  SettingSignal *m_settingSignal;
  QPushButton *m_buttonSettingSignal;
};

class Lowpass : public FilterState {
  Q_OBJECT
public:
  //    конструктор создает диологовое поле пользователя
  Lowpass(QWidget *wparent = nullptr);
  ~Lowpass() override = default;
  Lowpass(const Lowpass &rhs) = delete;
  Lowpass &operator=(const Lowpass &rhs) = delete;

  QList<std::pair<QString, QVariant>> SaveSettings() const override;
  void SetParameter(const QList<std::pair<QString, QVariant>> &param) override;
  QString Name() override;

  int GetFilterType() override;
  double GetCutFreq1() override;

private:
  void CreateWidget();
  void CreateSettingWidget();

  CustomDialWidget *m_freqCut = nullptr;
  QGridLayout *m_gridLayoout = nullptr;
};

class Hightpass : public FilterState {
  Q_OBJECT
public:
  Hightpass(QWidget *wparent = nullptr);
  ~Hightpass() override = default;
  Hightpass(const Hightpass &rhs) = delete;
  Hightpass &operator=(const Hightpass &rhs) = delete;

  QList<std::pair<QString, QVariant>> SaveSettings() const override;
  void SetParameter(const QList<std::pair<QString, QVariant>> &param) override;

  QString Name() override;
  int GetFilterType() override;
  double GetCutFreq1() override;

private:
  void CreateWidget();
  void CreateSettingWidget();
  CustomDialWidget *m_freqCut = nullptr;
  QGridLayout *m_gridLayoout = nullptr;
};

class Bandpass : public FilterState {
  Q_OBJECT
public:
  Bandpass(QWidget *wparent = nullptr);
  ~Bandpass() override = default;
  Bandpass(const Bandpass &rhs) = delete;
  Bandpass &operator=(const Bandpass &rhs) = delete;
  QList<std::pair<QString, QVariant>> SaveSettings() const override;
  void SetParameter(const QList<std::pair<QString, QVariant>> &param) override;
  QString Name() override;

  double GetCutFreq1() override;
  double GetCutFreq2() override;

  int GetFilterType() override;

private:
  void CreateWidget();

  void CreateSettingWidget();

  CustomDialWidget *m_freqCutFirst = nullptr;
  CustomDialWidget *m_freqCutSecond = nullptr;
  QGridLayout *m_gridLayoout = nullptr;
};

class Bandstop : public FilterState {
  Q_OBJECT
public:
  Bandstop(QWidget *wparent = nullptr);
  ~Bandstop() override = default;
  Bandstop(const Bandstop &rhs) = delete;
  Bandstop &operator=(const Bandstop &rhs) = delete;

  QList<std::pair<QString, QVariant>> SaveSettings() const override;
  void SetParameter(const QList<std::pair<QString, QVariant>> &param) override;
  QString Name() override;

  int GetFilterType() override;

  double GetCutFreq1() override;
  double GetCutFreq2() override;

private:
  void CreateWidget();
  void CreateSettingWidget();

  CustomDialWidget *m_freqCutFirst = nullptr;
  CustomDialWidget *m_freqCutSecond = nullptr;
  QGridLayout *m_gridLayoout = nullptr;
};

class Hlbrt : public FilterState {
  Q_OBJECT
public:
  Hlbrt(QWidget *wparent = nullptr);
  ~Hlbrt() override = default;
  Hlbrt(const Hlbrt &rhs) = delete;
  Hlbrt &operator=(const Hlbrt &rhs) = delete;

  QList<std::pair<QString, QVariant>> SaveSettings() const override;
  void SetParameter(const QList<std::pair<QString, QVariant>> &param) override;
  QString Name() override;

  int GetFilterType() override;

private:
  void CreateWidget();

  void CreateSettingWidget();

  CustomDialWidget *m_freqCutFirst = nullptr;
  CustomDialWidget *m_freqCutSecond = nullptr;
  QGridLayout *m_gridLayoout = nullptr;
};

#endif // DRAWCURVER_H
