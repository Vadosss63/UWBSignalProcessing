#ifndef ADCMODULE_H
#define ADCMODULE_H

#include "csignal.h"
#include "networkuwb.h"
#include "sourcemodule.h"
#include <QObject>
#include <map>
#include <vector>

using OutBuffer = std::pair<csignal<double>, csignal<double>>;

class ADCModule : public QObject, public SourceModule<OutBuffer> {
  Q_OBJECT

  struct ADCdatagram {
    uint16_t data_x;
    uint16_t az;
    uint16_t idPackage;
    uint16_t AA; // 0x55AA или 0
    int16_t data[512];
  };

public:
  ADCModule();
  ~ADCModule() override = default;

  void Operate() override;
  void StopEvent() override;
  void StartEvent() override;

  void Stop();
  void SetCountBoards(uint8_t boards);

  void SetAutoTrend(bool m_autoTrend);
  void SetCountAzimut(int countAzimut);

  //инициализация сокетов
  void DataSocketOpen();
  void DataSocketClose();

signals:
  void StartADC();
  void StopADC();

private:
  void AddDatagram();
  void CheckTrend();

  std::vector<ADCdatagram> m_dataPackI;
  std::vector<ADCdatagram> m_dataPackQ;
  std::vector<int> m_trendQ;
  std::vector<int> m_trendI;

  size_t m_countDiscretDatagram = 512;
  size_t m_countAzimut = 64;
  uint16_t m_nextIdPackage = 0;
  uint16_t m_currentFrame = 0;
  uint16_t m_countError = 0;

  uint8_t m_countBoard = 2;
  uint8_t m_countPackageForChanel = 4;
  uint8_t m_countAntans = 2;
  bool m_autoTrend = true;
  NetworkUWB::ReciverUDP m_reciverData;
};

#endif // ADCMODULE_H
