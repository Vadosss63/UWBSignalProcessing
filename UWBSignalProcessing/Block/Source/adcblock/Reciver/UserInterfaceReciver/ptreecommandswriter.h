#ifndef PTREECOMMANDSWRITER_H
#define PTREECOMMANDSWRITER_H

#include "networkuwb.h"
#include "ptreewr.h"

class PTreeCommandsWriter {
public:
  PTreeCommandsWriter();
  ~PTreeCommandsWriter() = default;

  void AddName(const std::string &name);
  void AddParameterCommand(const std::string &name, uint8_t val);
  void AddParameterCommand(uint8_t val);
  void AddParameterCommand(uint16_t val);
  void AddParameterCommand(uint32_t val);

  uint8_t ReadParameterValueUINT8_T(const std::string &name);
  uint16_t ReadParameterValueUINT16_T(const std::string &name);
  uint32_t ReadParameterValueUINT32_T(const std::string &name);

  std::string ReadTypeParameter(const std::string &name);

  // Запись команды в сокет
  void WriteCommand();
  // Чистка
  void Clear();
  bool GetAnswer();
  std::string GetNameAnswer();
  std::string GetError();
  std::list<std::string> GetParametersAnswer();
  bool IsNoError();

private:
  const int m_port = 8000;
  UtilityUWB::PtreeWR m_ptreeWR;
  NetworkUWB::SenderUDP m_senderCommand;
};

#endif // PTREECOMMANDSWRITER_H
