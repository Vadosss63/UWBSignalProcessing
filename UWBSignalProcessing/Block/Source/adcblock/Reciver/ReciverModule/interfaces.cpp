#include "interfaces.h"

Command::~Command(){}

ControllerCommands::~ControllerCommands(){}

Command* ControllerCommands::ReadCommand()
{
    return nullptr;
}

void ControllerCommands::Start(){}

void ControllerCommands::Stop(){}

HandlerDevice::~HandlerDevice(){}

bool HandlerDevice::ExicuteCommand(Command *)
{
    return false;
}

std::string HandlerDevice::GetError()
{
    return "No Open device";
}

bool HandlerDevice::IsNoError()
{
    return false;
}

bool HandlerDevice::Open(const std::string &)
{
    return false;
}
