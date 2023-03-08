#include "target.h"

size_t Target::m_maxSpeed = 30;

double Target::GetDirection(size_t numberPoint)
{
    return (*GetItter(numberPoint)).direction;
}

double Target::GetX(const Point& point) const
{
    return point.distence * RESULUTION * std::sin(point.direction * M_PI/ 180);
}

double Target::GetY(const Point& point) const
{
    return point.distence * RESULUTION * std::cos(point.direction  * M_PI/ 180);
}

double Target::GetSpeedKMH(size_t numberPoint)
{
    return 3.6 * GetSpeedMS(static_cast<size_t>((*GetItter(numberPoint)).speed));
}

double Target::GetToSpeedKMH(size_t s) const
{
    return 3.6 * GetSpeedMS(s);
}

double Target::GetSpeedMS(size_t s) const
{
    if(s >= m_countAcumm)
        return 0;

    if(s >= (m_countAcumm / 2))
        return (2. * (m_countAcumm - s) * m_maxSpeed) / m_countAcumm;

    return -(2. * s * m_maxSpeed) / m_countAcumm;
}

size_t Target::GetCountAcumm() const
{
    return m_countAcumm;
}

void Target::SetCountAcumm(const size_t &countAcumm)
{
    m_countAcumm = countAcumm;
}

size_t Target::GetMaxSpeed()
{
    return m_maxSpeed;
}

void Target::SetMaxSpeed(const size_t& maxSpeed)
{
    m_maxSpeed = maxSpeed;
}


double Target::GetAMP(size_t numberPoint)
{
    return (*GetItter(numberPoint)).amp;
}

void Target::AddPoint(const Target::Point& point)
{
    m_pointTarget.push_back(point);
}

void Target::RemovePoint(size_t numberPoint)
{
    m_pointTarget.erase(GetItter(numberPoint));
}

Target::Point& Target::GetPoint(size_t numberPoint)
{
    return (*GetItter(numberPoint));
}

size_t Target::Size() const
{
    return m_pointTarget.size();
}

size_t Target::Size()
{
    return m_pointTarget.size();
}

std::list<Target::Point>& Target::GetPoints()
{
    return m_pointTarget;
}

const std::list<Target::Point>& Target::GetPoints() const
{
    return m_pointTarget;
}

Target::Point&Target::GetFont()
{
    return m_pointTarget.front();
}

std::list<Target::Point>::iterator Target::GetItter(size_t numberPoint)
{
    auto it = m_pointTarget.begin();
    for (size_t i = 0; i < numberPoint; ++i){++it;}
    return it;
}
