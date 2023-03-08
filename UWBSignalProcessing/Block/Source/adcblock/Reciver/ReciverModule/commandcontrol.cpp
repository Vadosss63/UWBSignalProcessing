#include "commandcontrol.h"

CommandExecute::CommandExecute(std::string name, COMMANDS command, int countData):m_name(name), m_com(command), m_countData(countData)
{
    ResetAnswer();
    ResetRequest();
}

void CommandExecute::ResetAnswer()
{
    m_answer = ControlWord();
    for (int i = 0; i < m_countData; ++i)
        m_answer.AddData(0);
}

void CommandExecute::ResetRequest()
{
    m_request = ControlWord();
    m_request.SetSync(SYNCRO_REQUEST);
    m_request.SetCommand(m_com);
    for (int i = 0; i < m_countData; ++i) {
        m_request.AddData(0);
    }
}

void CommandExecute::SetParameter(std::string name, uint8_t val)
{
    if(name == "address"){
        m_request.SetAddr(val);
        return;
    }
    if(name == "data"){
        m_request.GetData(0) = val;
        return;
    }
}

void CommandExecute::SetParameter(std::string name, uint16_t val)
{
    if(name == "data"){
        m_request.GetData(0) = (uint8_t)val;
        m_request.GetData(1) = val >> 8;
        return;
    }
}

void CommandExecute::SetParameter(std::string name, uint32_t val)
{
    if(name == "data"){
        m_request.GetData(0) = (uint8_t)val;
        m_request.GetData(1) = (uint8_t)(val >> 8);
        m_request.GetData(2) = (uint8_t)(val >> 16);
        m_request.GetData(3) = (uint8_t)(val >> 24);
        return;
    }
}

std::string CommandExecute::Name() const
{
    return m_name;
}

std::vector<uint8_t> CommandExecute::GetRequest()
{
    return m_request.GetWord();
}

std::vector<uint8_t> &CommandExecute::GetAnswer()
{
    return  m_answer.GetWord();
}

std::vector<uint8_t> CommandExecute::GetAnswerData() const
{
    std::vector<uint8_t> res(m_answer.SizeData());
    for (size_t i = 0; i < res.size(); ++i)
        res[i] = m_answer.GetData(i);
    return res;
}


std::string CommandExecute::GetError()
{
    return m_error;
}

bool CommandExecute::IsNoError() const
{
    return m_error.empty();
}


void CommandExecute::SetError(const std::string& error)
{
    m_error = error;
}


bool CommandExecute::CheckCRC() const
{
    return m_answer.CheckCRC();
}
