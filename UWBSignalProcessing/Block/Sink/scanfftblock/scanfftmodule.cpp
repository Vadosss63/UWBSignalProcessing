#include "scanfftmodule.h"

ScanfftModule::ScanfftModule(QObject *parent)
    : QObject(parent), SinkModule(1) {}

void ScanfftModule::Transform(std::vector<std::valarray<double>> &newSignal) {
  if (m_isVertical) /// TODO переделать это костыль
  {
    m_updateScane->ResizePloter(newSignal.size());
    for (size_t i = 0; i < newSignal.size(); ++i) {
      for (size_t j = 0; j < newSignal[i].size(); ++j) {
        double val = newSignal[i][j];
        SetValColor(static_cast<int>(j), val);
        if (m_ploterLine == static_cast<int>(j)) {
          if (m_isLog) {
            val = 20 * std::log(val);
          }
          m_updateScane->DataPloter()[(newSignal.size() - 1) - i].setY(val);
        }
      }
      m_updateScane->DataMatrix()[i]->GetGradientImage()->setStops(
          m_gradientPoints);
    }

  } else {
    for (size_t i = 0; i < newSignal.size(); ++i) {
      m_updateScane->ResizePloter(newSignal[i].size());
      for (size_t j = 0; j < newSignal[i].size(); ++j) {
        double val = newSignal[i][j];
        SetValColor(static_cast<int>(j), val);
        if (static_cast<size_t>(m_ploterLine) == (newSignal.size() - 1) - i) {
          if (m_isLog) {
            val = 20 * std::log(val);
          }
          m_updateScane->DataPloter()[j].setY(val);
        }
      }
      m_updateScane->DataMatrix()[i]->GetGradientImage()->setStops(
          m_gradientPoints);
    }
  }
}

int ScanfftModule::CheckValColor(double val) noexcept {
  if (val < 0)
    return 0;

  if (val > 2047)
    return 255;

  return (static_cast<short>(val) >> 3);
}

QColor ScanfftModule::GetColor(double val) {
  int valInt = CheckValColor(val);
  //    return QColor(255, 255, 255);

  return QColor(valInt, valInt, valInt);
}

void ScanfftModule::ResetSize(size_t size, size_t count) {
  if (size != m_size) {
    m_size = size;
    m_updateScane->CreateSector(count);
    for (auto *i : m_updateScane->DataMatrix()) {
      i->update();
    }
    CreateQGradientStops();
  }
}

void ScanfftModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  std::vector<std::valarray<double>> signal;
  if (!PopFromInput(signal))
    return;

  ResetSize(signal.front().size(), signal.size());

  Transform(signal);
  m_updateScane->Update();
  m_updateScane->UpdatePloter();
}

void ScanfftModule::CreateQGradientStops() {
  int sizeSignal = GetSizePlotSignal();
  m_gradientPoints.resize(sizeSignal);
  qreal step = 1.0 / sizeSignal;
  int i = 1;
  // заполняем массив с точками
  for (auto &posPoint : m_gradientPoints) {
    posPoint.first = i * step;
    i++;
  }
}

int ScanfftModule::GetSizePlotSignal() noexcept {
  return static_cast<int>(m_size);
}

void ScanfftModule::SetValColor(int pos, double val) {
  if (m_isLog) {
    val = 20 * std::log(val);

    if (val < 0)
      val = 0;

    if (val > 255)
      val = 255;

    m_gradientPoints[pos].second = QColor(
        static_cast<int>(val), static_cast<int>(val), static_cast<int>(val));

    return;
  }
  m_gradientPoints[pos].second = GetColor(val);
}

void ScanfftModule::SetPloterLine(int ploterLine) { m_ploterLine = ploterLine; }

void ScanfftModule::SetIsVerticalLine(bool isVertical) {
  m_isVertical = isVertical;
}

void ScanfftModule::SetIsLog(bool isLog) { m_isLog = isLog; }

void ScanfftModule::SetUpdateScane(UpdateScane *updateScane) {
  m_updateScane = updateScane;
}
