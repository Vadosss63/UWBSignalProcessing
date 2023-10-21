#ifndef READCOMPLEXMODULE_H
#define READCOMPLEXMODULE_H

#include "csignal.h"
#include "sourcemodule.h"
#include "typesignal.h"
#include <string>

using OutBuffer = std::pair<csignal<double>, csignal<double>>;

class ReadComplexModule : public SourceModule<OutBuffer> {

public:
  ReadComplexModule();
  ~ReadComplexModule() override;

  //проверяет наличие потока на открытость, и если открыт, то закрывает его.
  //устанавливает путь файла с csignal и запускает
  //read_service_parameters_in_bin()
  void SetPathFile(const std::string &pathFileSigI,
                   const std::string &pathFileSigQ);
  //производит выталкивание csignal<double> в выходной порт
  //пока, флаг имеет значение m_flagReadFile = true
  //и файл не прочитан до конца. Если файл прочитан, то флаг переводится в
  //положение false, а выталкивание сигнала не происходит.
  void Operate() override;
  void StartEvent() override;
  void StopEvent() override;
  void TimerTimout() override;

private:
  //читает сервисную информацию из файла. и если она верна,
  //то устанавливает m_flagReadFile = true
  bool ReadServiceParametersInBinSigI();
  bool ReadServiceParametersInBinSigQ();

  // Чтение сигнала из файла
  csignal<double> ReadSignalI();
  // Чтение сигнала из файла
  csignal<double> ReadSignalQ();

  //флаг чтения файла (если false, то файл или имеет не верный формат, или
  //закончился)
  volatile bool m_flagReadFile = false;

  //путь к файлу csignal
  std::string m_pathFileSigI = "";
  std::string m_pathFileSigQ = "";
  //длина csignal
  size_t m_lengthSigI = 0;
  size_t m_lengthSigQ = 0;
  //поток чтения из файла в csignal
  std::ifstream m_readFileSigI;
  std::ifstream m_readFileSigQ;
  // тип сохраненного сигнала
  TypeSignal m_typeSignalI = TypeSignal::doubleType;
  TypeSignal m_typeSignalQ = TypeSignal::doubleType;

  void CloseStream();
};

#endif // READMODULE_H
