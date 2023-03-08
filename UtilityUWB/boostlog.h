#ifndef BOOSTLOG_H
#define BOOSTLOG_H

#include <radar_global_logger.h>

void LogInfo(const std::string& message);
void LogError(const std::string& message);

class BoostLog
{

public:    
    static BoostLog* Instance();
    ~BoostLog() noexcept;
    void WriteInfoLog(const std::string& message);
    void WriteErrorLog(const std::string& message);

private:
    static BoostLog* m_logWriter;
    BoostLog() noexcept;
};

#endif // BOOSTLOG_H
