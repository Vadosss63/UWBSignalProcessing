#include "boostlog.h"
#include <iostream>

BoostLog* BoostLog::m_logWriter = nullptr;

BoostLog* BoostLog::Instance()
{
    if(!m_logWriter)
        m_logWriter = new BoostLog;

    return m_logWriter;
}

BoostLog::~BoostLog() noexcept {}

void BoostLog::WriteInfoLog(const std::string& message)
{
//    LOG_INFO << message;
}

void BoostLog::WriteErrorLog(const std::string& message)
{
//    LOG_ERROR << message;
}

BoostLog::BoostLog() noexcept{}

void LogInfo(const std::string& message)
{
    BoostLog::Instance()->WriteInfoLog(message);
    std::cout << message << std::endl;
}

void LogError(const std::string& message)
{
    BoostLog::Instance()->WriteErrorLog(message);
    std::cout << message << std::endl;
}
