#ifndef COMMANDCONTROL_H
#define COMMANDCONTROL_H

#include "controlword.h"
#include "interfaces.h"
#include <stdexcept>
#include <vector>

class CommandExecute : public Command {

public:
  CommandExecute(std::string name, COMMANDS command, int countData);
  void ResetAnswer() override;
  void ResetRequest() override;
  void SetParameter(std::string name, uint8_t val) override;
  void SetParameter(std::string name, uint16_t val) override;
  void SetParameter(std::string name, uint32_t val) override;

  std::string Name() const override;
  std::vector<uint8_t> GetRequest() override;
  std::vector<uint8_t> &GetAnswer() override;
  std::vector<uint8_t> GetAnswerData() const override;

  void SetError(const std::string &error) override;
  std::string GetError() override;
  bool IsNoError() const override;
  bool CheckCRC() const override;

private:
  std::string m_name;
  COMMANDS m_com;
  int m_countData;
  ControlWord m_answer;
  ControlWord m_request;
  static const int SYNCRO_ANSWER = 0x55;
  static const int SYNCRO_REQUEST = 0xAA;
  std::string m_error;
};

#endif // COMMANDCONTROL_H
