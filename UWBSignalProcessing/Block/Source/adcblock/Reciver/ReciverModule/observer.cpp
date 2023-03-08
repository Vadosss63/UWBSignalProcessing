#include "observer.h"

Observer::~Observer(){}

void Observer::DestroySubgect(Subject*){}

Subject::~Subject()
{
    for (auto observer : m_observeres)
        observer->DestroySubgect(this);
}

void Subject::Attach(Observer *observer)
{
    m_observeres.push_back(observer);
}

void Subject::Detach(Observer *observer)
{
    m_observeres.remove(observer);
}

void Subject::Notify()
{
    for (auto observer : m_observeres)
        observer->Update();
}
