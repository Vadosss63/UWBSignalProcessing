#ifndef WRITETARGETMODULE_H
#define WRITETARGETMODULE_H

#include "autodatagram.h"
#include "sinkmodule.h"
#include "target.h"
#include <cstdio>
#include <string>

using InBuffer = std::vector<Target>;
class WriteTargetModule : public SinkModule<InBuffer> {
public:
  WriteTargetModule();
  ~WriteTargetModule() override;

  void Operate() override;

  void StartEvent() override;

  //устанавливает путь файла с csignal и устанавливает
  //m_flagAddServiceParameters = true
  void SetPathFile(const std::string &pathFile);

private:
  void CreateProtocol(InBuffer &target);
  //добавляет сервисную информацию в файл.
  void AddServiceParametersInBin();
  //флаг сообщает Operate() что указан новый путь к файлу bin
  bool m_flagAddServiceParameters = false;
  //путь к файлу csignal
  std::string m_pathFile = "";
  std::ofstream m_writeFile;
  void CloseStream();
};

#endif // WRITETARGETMODULE_H
