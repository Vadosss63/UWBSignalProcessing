#include "controllercommands.h"
#include <sstream>

PTreeController::PTreeController(): m_reciverUDP(8000)
{
    m_reciverUDP.AddCallback(&PTreeController::ReadJson, this);
    InitCommands();
}

PTreeController::~PTreeController()
{
    for (auto& command : m_commands)
        delete command.second;

    m_ptreeWR.Clear();
}

void PTreeController::ReadJson()
{
    m_ptreeWR.ReadJson(m_reciverUDP.GetData());
    Notify();
    SendAnswer();
}

void PTreeController::Start()
{
    m_reciverUDP.Start();
}

void PTreeController::Stop()
{
    m_reciverUDP.Stop();
}

void PTreeController::SetParameter(const std::string& param)
{
    std::string type = m_ptreeWR.ReadParameterType(param);
    if(type == "uint8_t")
        SetParametrForType<uint8_t>(param);
    else if (type == "uint16_t")
        SetParametrForType<uint16_t>(param);
    else if (type == "uint32_t")
        SetParametrForType<uint32_t>(param);
}

Command* PTreeController::ReadCommand()
{
    std::string name = m_ptreeWR.ReadName();

    m_currentCommand = m_commands[name];
    if(!m_currentCommand)
        return nullptr;

    m_currentCommand->ResetAnswer();
    m_currentCommand->ResetRequest();

    std::list<std::string> lParam = m_ptreeWR.ParametersKey();

    for(auto param : lParam)
        SetParameter(param);

    return m_currentCommand;
}

void PTreeController::AddCommand(Command* command)
{
    m_commands[command->Name()] = command;
}

void PTreeController::InitCommands()
{
    AddCommand(new CommandExecute("RebootCommand", COMMANDS::REBOOT, 0));
    AddCommand(new CommandExecute("SaveCommand", COMMANDS::SAVE, 0));

    AddCommand(new CommandExecute("GetStatusCommand", COMMANDS::GET_STATUS, 1));
    AddCommand(new CommandExecute("DMAmplifierTune", COMMANDS::DM_AMP, 1));
    AddCommand(new CommandExecute("USP1AmplifierTune", COMMANDS::USP1Amp, 1));
    AddCommand(new CommandExecute("USP2AmplifierTune", COMMANDS::USP2Amp, 1));

    AddCommand(new CommandExecute("DMAmplifierTune_2", COMMANDS::DM_AMP_2, 1));
    AddCommand(new CommandExecute("USP1AmplifierTune_2", COMMANDS::USP1Amp_2, 1));
    AddCommand(new CommandExecute("USP2AmplifierTune_2", COMMANDS::USP2Amp_2, 1));

    AddCommand(new CommandExecute("SyncronizationTune", COMMANDS::SYNCRONIZATION, 1));
    AddCommand(new CommandExecute("BandWidthTune", COMMANDS::BAND, 1));

    AddCommand(new CommandExecute("GetSettingCommand", COMMANDS::GET_SETTING, 4));
    AddCommand(new CommandExecute("DMSynthesizerTune", COMMANDS::DMSynt, 4));

    AddCommand(new CommandExecute("GetStatusSynchronitionCommand", COMMANDS::GET_STATUS_SYNCH, 1));
    AddCommand(new CommandExecute("AccumSynchronition", COMMANDS::ACCUM, 4));
    AddCommand(new CommandExecute("ControlScanSynchronition", COMMANDS::CONTROL_SCANE, 4));

    AddCommand(new CommandExecute("RebootSynchronitionCommand", COMMANDS::REBOOT, 0));
    AddCommand(new CommandExecute("SaveSynchronitionCommand", COMMANDS::SAVE, 0));
}

void PTreeController::SendAnswer()
{
    m_ptreeWR.Clear();
    if(m_currentCommand->IsNoError())
    {
        m_ptreeWR.AddName(m_currentCommand->Name());
        if(m_currentCommand->CheckCRC()){
            std::vector<uint8_t> data = m_currentCommand->GetAnswerData();

            if(data.size() == 1)
                m_ptreeWR.AddParameter("data", data[0]);

            if(data.size() == 2)
            {
                uint16_t data_16 = (uint16_t)data[0] + ((uint16_t)data[1] << 8);
                m_ptreeWR.AddParameter("data", data_16);
            }

            if(data.size() == 4)
            {
                uint32_t data_32 = (uint32_t)data[0] + (uint32_t)(data[1] << 8) + (uint32_t)(data[2] << 16) + (uint32_t)(data[3] << 24);
                m_ptreeWR.AddParameter("data", data_32);
            }
        }
        else
        {
            m_ptreeWR.AddError("Error check answer CRC");
        }

        m_reciverUDP.SendAnswer(m_ptreeWR.WriteToBuffer());
    }
    else
    {
        m_ptreeWR.AddError(m_currentCommand->GetError());
        m_reciverUDP.SendAnswer(m_ptreeWR.WriteToBuffer());
    }
}
