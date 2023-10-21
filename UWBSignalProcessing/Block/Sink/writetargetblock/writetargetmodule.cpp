#include "writetargetmodule.h"
#include <iostream>

WriteTargetModule::WriteTargetModule() : SinkModule(1) {}

void WriteTargetModule::CloseStream() {
  if (m_writeFile.is_open())
    m_writeFile.close();
}

void WriteTargetModule::AddServiceParametersInBin() {
  m_writeFile.open(m_pathFile, std::ios_base::binary);
  if (m_writeFile) {
    CloseStream();
    m_writeFile.open(m_pathFile, std::ios_base::binary | std::ios_base::out |
                                     std::ios_base::app);
    m_flagAddServiceParameters = false;
  }
}

WriteTargetModule::~WriteTargetModule() { CloseStream(); }

void WriteTargetModule::SetPathFile(const std::string &pathFile) {
  CloseStream();
  m_pathFile = pathFile;
  m_flagAddServiceParameters = true;
}

void WriteTargetModule::CreateProtocol(InBuffer &target) {
  size_t countTarget = target.size();
  // Заполняем заголовок
  AutoDatagram::Header header;
  header.type = 0x2323;
  header.countTarget = static_cast<uint32_t>(countTarget);
  m_writeFile.write(reinterpret_cast<char *>(&header), sizeof(header));
  m_writeFile << Target::GetMaxSpeed();

  for (auto &tar : target) {
    // Заполнение данных о цели
    m_writeFile << tar;
  }
}

void WriteTargetModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  if (m_pathFile.empty()) {
    InBuffer temp;
    PopFromInput(temp);
    return;
  }

  InBuffer temp;
  if (!PopFromInput(temp))
    return;

  if (m_flagAddServiceParameters)
    AddServiceParametersInBin();

  CreateProtocol(temp);
}

void WriteTargetModule::StartEvent() {
  CloseStream();
  m_flagAddServiceParameters = true;
}
