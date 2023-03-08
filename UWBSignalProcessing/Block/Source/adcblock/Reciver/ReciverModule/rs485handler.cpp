#include "rs485handler.h"

RS485Handler::RS485Handler()
{

}

RS485Handler::~RS485Handler()
{
    m_rs485Port.Close();
}

bool RS485Handler::Open(const std::string &dev)
{
    m_rs485Port.Open(dev);
    m_rs485Port.SetRate();
    return m_rs485Port.IsNoError();
}

bool RS485Handler::ExicuteCommand(Command* command)
{
    if(command)
        if(WriteCommand(command->GetRequest()))
            return GetAnswer(command->GetAnswer());

    return false;
}

bool RS485Handler::WriteCommand(const std::vector<uint8_t>& reqest)
{
    return m_rs485Port.WriteCommand(reqest);
}

bool RS485Handler::GetAnswer(std::vector<uint8_t>& answer)
{
    return m_rs485Port.GetAnswer(answer);
}

std::string RS485Handler::GetError()
{
    return m_rs485Port.GetErrorsMessage();
}

bool RS485Handler::IsNoError()
{
    return m_rs485Port.IsNoError();
}
