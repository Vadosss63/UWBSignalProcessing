#include "adcmodule.h"
#include <iostream>

ADCModule::ADCModule() : SourceModule(0), m_reciverData("192.168.0.1", 0x4002) {
  SetCountBoards(2);
  m_reciverData.AddCallback([&] { AddDatagram(); });
}

void ADCModule::StopEvent() {
  StopADC();
  m_trendQ.resize(0);
  m_trendI.resize(0);
  SourceModule::StopEvent();
}

void ADCModule::StartEvent() {
  m_currentFrame = 0;
  m_nextIdPackage = 0;
  StartADC();
}

void ADCModule::Operate() {
  CheckTrend();

  size_t sizeSig = m_countDiscretDatagram * m_countPackageForChanel;

  csignal<double> Isignal(sizeSig);
  csignal<double> Qsignal(sizeSig);

  if (m_currentFrame % 4 != m_dataPackI[0].az % 4 ||
      m_currentFrame % 4 != m_dataPackQ[0].az % 4) {

    return;
  }
  Isignal.setFrameNumber(m_currentFrame);
  Qsignal.setFrameNumber(m_currentFrame);
  m_currentFrame++;

  uint16_t azimut = m_dataPackI[0].az % m_countAzimut;
  Isignal.setAzimuth(azimut);
  Qsignal.setAzimuth(azimut);

  for (size_t i = 0; i < m_countPackageForChanel; ++i) {
    for (size_t j = 0; j < m_countDiscretDatagram; ++j) {
      // size_t indexShift =  (i * m_countDiscretDatagram + j) % m_countBoard;
      size_t indexShift = (i * m_countDiscretDatagram + j) % 4;

      Isignal[i * m_countDiscretDatagram + j] =
          m_dataPackI[i].data[j] - m_trendI[indexShift];
      Qsignal[i * m_countDiscretDatagram + j] =
          m_dataPackQ[i].data[j] - m_trendQ[indexShift];
    }
  }

  PushToOutput(std::make_pair(std::move(Isignal), std::move(Qsignal)));

  if (!m_currentFrame) {
    std::cout << m_countError << std::endl;
    m_countError = 0;
  }
}

void ADCModule::Stop() { StopEvent(); }

void ADCModule::DataSocketOpen() {
  /// TODO для ассинхроного приема
  if (!m_reciverData.Start()) {
    std::cerr << "ADC SocketOpen" << m_reciverData.GetErrorsMessage()
              << std::endl;
    StopedModule();
  }
}

void ADCModule::DataSocketClose() {
  /// TODO для ассинхроного приема
  m_reciverData.Stop();
}

void ADCModule::AddDatagram() {
  ADCdatagram adcDatagram = m_reciverData.GetData<ADCdatagram>();
  uint8_t sizeFrame = m_countAntans * m_countPackageForChanel * 2;
  uint8_t indexCheckPack = adcDatagram.idPackage % (sizeFrame);

  if (m_nextIdPackage != indexCheckPack) {
    m_nextIdPackage = 0;
    return;
  }
  m_nextIdPackage = (m_nextIdPackage + 1) % (sizeFrame);

  uint8_t indexPack = adcDatagram.idPackage % (m_countPackageForChanel);

  //определяем четность сигнала
  bool isQchanel = (adcDatagram.idPackage / m_countPackageForChanel) % 2;

  bool isOperate = (adcDatagram.idPackage + 1) % (m_countPackageForChanel * 2);
  isOperate = !isOperate;

  if (isQchanel)
    m_dataPackQ[indexPack] = adcDatagram;
  else
    m_dataPackI[indexPack] = adcDatagram;

  if (isOperate)
    Operate();
}

void ADCModule::CheckTrend() {
  size_t sizeTrend = 4 /*m_countBoard*/;
  if (m_autoTrend) {
    size_t indexPack = m_countPackageForChanel - 1;
    m_trendQ.resize(sizeTrend);
    m_trendI.resize(sizeTrend);

    for (size_t i = 0; i < m_countDiscretDatagram; ++i) {
      size_t indexTrend = i % sizeTrend;
      m_trendQ[indexTrend] += m_dataPackQ[indexPack].data[i];
      m_trendI[indexTrend] += m_dataPackI[indexPack].data[i];
    }

    for (size_t i = 0; i < sizeTrend; ++i) {
      m_trendQ[i] = m_trendQ[i] / (m_countDiscretDatagram / sizeTrend);
      m_trendI[i] = m_trendI[i] / (m_countDiscretDatagram / sizeTrend);
    }
  } else {
    m_trendQ.resize(sizeTrend);
    m_trendI.resize(sizeTrend);
  }
}

void ADCModule::SetCountAzimut(int countAzimut) {
  m_countAzimut = static_cast<size_t>(countAzimut);
}

void ADCModule::SetAutoTrend(bool autoTrend) { m_autoTrend = autoTrend; }

void ADCModule::SetCountBoards(uint8_t boards) {
  m_countBoard = boards;
  m_countPackageForChanel = 2 * boards;
  m_dataPackI.resize(m_countPackageForChanel);
  m_dataPackQ.resize(m_countPackageForChanel);
}
