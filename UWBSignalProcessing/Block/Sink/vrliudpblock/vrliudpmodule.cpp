#include "vrliudpmodule.h"
#include <iostream>

VRLIUDPModule::VRLIUDPModule() : SinkModule(1), m_server("192.168.1.1", 12213) {
  m_server.AddCallback(&VRLIUDPModule::CreateProtocol, this);
}

VRLIUDPModule::~VRLIUDPModule() { StopServer(); }

void VRLIUDPModule::StopServer() { m_server.Stop(); }

void VRLIUDPModule::StartServer() {
  StopServer();
  m_server.Start();
}

void VRLIUDPModule::CreateProtocol() {
  std::lock_guard<std::mutex> lock(operateLock);
  // первое значение в массиве это количество целей
  uint32_t countTarget = m_dataTarget.size();
  // расчет количества требуемой памяти
  uint32_t sizeMemory =
      countTarget * sizeof(DataVRLI) + sizeof(countTarget) + 2;
  uint8_t *dataSend = (uint8_t *)malloc(sizeMemory);
  uint8_t *data = dataSend;
  uint8_t *header = reinterpret_cast<uint8_t *>(data);
  *header = 35;
  data += sizeof(uint8_t);
  header = reinterpret_cast<uint8_t *>(data);
  *header = 35;
  data += sizeof(uint8_t);
  // записываем количество целей в пакете
  uint32_t *writeCountTarget = reinterpret_cast<uint32_t *>(data);
  *writeCountTarget = countTarget;
  data += sizeof(countTarget);

  // заполняем информацию о целе
  for (size_t i = 0; i < countTarget; ++i) {
    DataVRLI *dataVRLI = reinterpret_cast<DataVRLI *>(data);
    // Номер цели
    dataVRLI->Nc = m_dataTarget[i].Nc;
    // Пелинг до цели нормируем от -45 до 45
    dataVRLI->angel = m_dataTarget[i].angel * m_valueAngel + (m_angelBegin);
    // Растояние до цели нормируем по дискретам
    dataVRLI->range = m_dataTarget[i].range * m_valueDiscret;
    // Нормируем по ВАРУ
    dataVRLI->amp = m_dataTarget[i].amp;
    // Выполняем смещения по массиву данных
    data += sizeof(DataVRLI);
  }
  m_server.SendAnswer(dataSend, sizeMemory);
  // удаляем данные
  free(dataSend);
}

void VRLIUDPModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  if (!PopFromInput(m_dataTarget))
    return;
}

void VRLIUDPModule::StartEvent() { StartServer(); }
