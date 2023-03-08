#include "logicmodule.h"

LogicModule::LogicModule() :
    IOModule(2, 1){}

void LogicModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    csignal<double> sigFirst;
    if(!PopFromInput(sigFirst, 0))
        return;


    csignal<double> sigSecond;
    if(!PopFromInput(sigSecond, 1))
        return;
    if(m_sendFirst)
    {
        PushToOutput(std::move(sigFirst));
        m_sendFirst = false;
    }
    else
    {
        PushToOutput(std::move(sigSecond));
        m_sendFirst = true;
    }
}

