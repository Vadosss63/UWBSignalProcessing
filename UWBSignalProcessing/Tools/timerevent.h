#ifndef TIMEREVENT_H
#define TIMEREVENT_H

#include <algorithm>
#include <forward_list>
#include <mutex>

class TimerEvent;

class TimerListEvents {

public:
  TimerListEvents() = default;
  ~TimerListEvents() = default;

  // добавление слушателя событий
  void AttachEvent(TimerEvent *event);

  // удаления слушателя событий
  void DetachEvent(TimerEvent *event);

protected:
  void Timout();

private:
  std::mutex m_mutex;
  std::forward_list<TimerEvent *> m_listEvent;
};

class TimerEvent {
  friend TimerListEvents;

public:
  virtual ~TimerEvent();

  virtual void TimerTimout();
  // Включает таймер для модулей
  void SetEnableTimer(bool isEnable);
  bool IsEnableTimer();

private:
  void RegisterTimer(TimerListEvents *timer);

  TimerListEvents *m_timer = nullptr;
  bool m_isEnable = false;
};

#endif // TIMEREVENT_H
