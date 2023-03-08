#include "recievercontrolmodule.h"
#include <iostream>

RecieverControlModule::RecieverControlModule(){}

RecieverControlModule::~RecieverControlModule()
{
    delete m_controllerCommands;
    delete m_handlerDevice;
}

void RecieverControlModule::SetControllerCommands(ControllerCommands* controllerCommands)
{
    delete m_controllerCommands;
    m_controllerCommands = controllerCommands;
    // добавляем наблюдателя за контроллером
    m_controllerCommands->Attach(this);
}

void RecieverControlModule::SetHandlerDevice(HandlerDevice* handlerDevice)
{
    delete m_handlerDevice;
    m_handlerDevice = handlerDevice;
}

void RecieverControlModule::Start()
{
    m_controllerCommands->Start();
}

void RecieverControlModule::Stop()
{
    m_controllerCommands->Stop();
}

void RecieverControlModule::Execute()
{
    Command* command = m_controllerCommands->ReadCommand();
    command->SetError("");
    if(!m_handlerDevice->ExicuteCommand(command))
        command->SetError(m_handlerDevice->GetError());
}

void RecieverControlModule::Update()
{
    Execute();
}
