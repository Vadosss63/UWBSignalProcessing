#include "ptreecommandswriter.h"
#include <iostream>
#include <sstream>

PTreeCommandsWriter::PTreeCommandsWriter():m_senderCommand(m_port)
{
    m_senderCommand.Bind(9001);
}

void PTreeCommandsWriter::AddName(const std::string& name)
{
    m_ptreeWR.AddName(name);
}

void PTreeCommandsWriter::AddParameterCommand(uint8_t val)
{
    m_ptreeWR.AddParameter("data", val);
}

void PTreeCommandsWriter::AddParameterCommand(uint16_t val)
{
    m_ptreeWR.AddParameter("data",val);
}

void PTreeCommandsWriter::AddParameterCommand(uint32_t val)
{
    m_ptreeWR.AddParameter("data", val);
}

uint8_t PTreeCommandsWriter::ReadParameterValueUINT8_T(const std::string& name)
{
    return m_ptreeWR.ReadParameterValue<uint8_t>(name);
}

uint16_t PTreeCommandsWriter::ReadParameterValueUINT16_T(const std::string& name)
{
    return m_ptreeWR.ReadParameterValue<uint16_t>(name);
}

uint32_t PTreeCommandsWriter::ReadParameterValueUINT32_T(const std::string& name)
{
    return m_ptreeWR.ReadParameterValue<uint32_t>(name);
}

std::string PTreeCommandsWriter::ReadTypeParameter(const std::string &name)
{
    return m_ptreeWR.ReadParameterType(name);
}

void PTreeCommandsWriter::Clear()
{
    m_ptreeWR.Clear();
}

bool PTreeCommandsWriter::GetAnswer()
{
    bool ok =  m_senderCommand.ReadWithTimeout(boost::posix_time::seconds(1));
    if(ok)
    {
        std::string answer = m_senderCommand.GetData();
        m_ptreeWR.ReadJson(std::move(answer));
    }
    else {
        m_ptreeWR.Clear();
        m_ptreeWR.AddError(m_senderCommand.GetErrorsMessage());
    }
    return ok;
}

std::string PTreeCommandsWriter::GetNameAnswer()
{
    return m_ptreeWR.ReadName();
}

std::string PTreeCommandsWriter::GetError()
{
    return m_ptreeWR.ReadError();
}

std::list<std::string> PTreeCommandsWriter::GetParametersAnswer()
{
    return m_ptreeWR.ParametersKey();
}

bool PTreeCommandsWriter::IsNoError()
{
    return m_ptreeWR.ReadError().empty();
}

void PTreeCommandsWriter::WriteCommand()
{
    std::string buffer = m_ptreeWR.WriteToBuffer();
    bool ok = m_senderCommand.SendData(buffer);
    m_ptreeWR.Clear();
    if(!ok)
        m_ptreeWR.AddError(m_senderCommand.GetErrorsMessage());
}

void PTreeCommandsWriter::AddParameterCommand(const std::string& name, uint8_t val)
{
    m_ptreeWR.AddParameter(name, val);
}
