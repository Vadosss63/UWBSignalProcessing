#include "abstractmodule.h"



AbstractModule::~AbstractModule()
{
    if(m_operationManager)
        m_operationManager->DetachEvent(this);
}

void AbstractModule::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    m_operationManager = operationManager;
    if(m_operationManager)
        m_operationManager->AttachEvent(this);
}

void AbstractModule::StopEvent(){}

void AbstractModule::StartEvent(){}

void AbstractModule::TimerTimout(){}

void AbstractModule::QueueOperation()
{
    if(m_operationManager)
        m_operationManager->AddTask(this);
}

void AbstractModule::StopedModule()
{
    if(m_operationManager)
        m_operationManager->StopManager();
}
