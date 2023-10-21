#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

class Subject;

class Observer {
public:
  virtual ~Observer();
  virtual void Update() = 0;
  // оповещение о удалении субъекта
  virtual void DestroySubgect(Subject *);
};

class Subject {
public:
  virtual ~Subject();
  virtual void Attach(Observer *observer);
  virtual void Detach(Observer *observer);
  virtual void Notify();

private:
  std::list<Observer *> m_observeres;
};

#endif // OBSERVER_H
