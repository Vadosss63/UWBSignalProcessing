#include "scanscenemodule.h"
#include <QColor>
#include <iostream>
#include <QGraphicsWidget>

#include <chrono>

ScanBufferSceneModule::ScanBufferSceneModule(QObject* parent)
    :QObject (parent), SinkModule(1)
{
    m_lastTimeUpdate = std::chrono::high_resolution_clock::now();
}

void ScanBufferSceneModule::UpdateSector(uint32_t azimuth)
{
    m_dataMatrix[azimuth]->SetActiveWhiteSector(false);
    m_dataMatrix[azimuth]->update();
}

void ScanBufferSceneModule::ShowWhiteLine(uint32_t azimuth)
{
    if(m_isWhiteLine)
    {
        size_t activeWhite = (azimuth + 1) % m_dataMatrix.size();
        m_dataMatrix[activeWhite]->SetActiveWhiteSector(true);
        m_dataMatrix[activeWhite]->update();
    }
}

void ScanBufferSceneModule::UpdateScan(uint32_t azimuth)
{
    if(m_updateScan)
    {
        if(m_currentFrame % m_updateStep)
        {
            m_currentFrame++;
            return;
        }

        m_currentFrame++;
        emit ScanUpdated();
        return;

    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto res_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_lastTimeUpdate);

    if(res_time.count() > 20)
    {
        if(m_isUpdateAll)
        {
            emit ScanUpdated();
            m_isUpdateAll = false;
        }
        else
        {
            ShowWhiteLine(azimuth);
            UpdateSector(azimuth);
        }
    }
    else
    {
        m_isUpdateAll = true;
        return;
    }

    m_lastTimeUpdate = end_time;
}

void ScanBufferSceneModule::Transform(csignal<double>& newSignal)
{
    for (size_t i = m_startIndexPlot, index = 0; i < m_stopIndexPolot + 1; ++i, ++index)
        SetValColor(static_cast<int>(index), newSignal[i]);

    m_dataMatrix[newSignal.azimuth()]->GetGradientImage()->setStops(m_gradientPoints);
}

void ScanBufferSceneModule::SetupSignal(csignal<double>& signal)
{
    if(m_verticalShift)
        signal += static_cast<double>(m_verticalShift);

    if(m_gain - 1)
        signal *= static_cast<double>(m_gain);
}

void ScanBufferSceneModule::SetUpdateScan(bool updateScan, int updateStep) noexcept
{
    m_updateScan = updateScan;
    m_updateStep = updateStep;
}

void ScanBufferSceneModule::SetWhiteLine(bool isWhiteLine) noexcept
{
    m_isWhiteLine = isWhiteLine;
}

int ScanBufferSceneModule::CheckValColor(double val) noexcept
{
    if(val < 0)
        return 0;

    if(val > 2047)
        return 255;

    return (static_cast<short>(val) >> 3);
}

QColor ScanBufferSceneModule::GetColor(double val)
{
    if(m_isColorView)
        return GetColorView(val);

    int valInt = CheckValColor(val);
    return QColor(valInt, valInt, valInt);
}

QColor ScanBufferSceneModule::GetColorView(double val) noexcept
{
    // диапазон чисел 2046 / 3
    constexpr int partRange = 648;
    constexpr int part2Range = partRange * 2;
    constexpr int part3Range = partRange * 3;
    constexpr double coef = 1.0 / partRange;


    if(val <= 0)
    {
        return m_colorMin;
    }

    if(val > 0 && val <= partRange)
    {
        // зеленый
        val = val * coef;
        m_color1.setAlphaF(val);
        return m_color1;
    }

    if(val > partRange && (val <= part2Range))
    {
        // желтый
        val = (val - partRange) * coef;
        m_color2.setAlphaF(val);
        return m_color2;
    }

    if(val > part2Range && (val <= part3Range))
    {
        val = (val - part2Range) * coef;
        m_color3.setAlphaF(val);
        return m_color3;
    }
    return m_colorMax;
}

void ScanBufferSceneModule::SetColorView(bool isColorView) noexcept
{
    m_isColorView = isColorView;
}

void ScanBufferSceneModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    ///TODO попробывать перевезти за мютех и посмотреть что будет ???

    csignal<double> newSignal;
    if (!PopFromInput(newSignal))
        return;

    // проверка азимута отображения
    if(newSignal.azimuth() >= m_coutAngel)
        return;

    if(newSignal.size() != m_currentSignalSize)
    {
        m_currentSignalSize = newSignal.size();
        SetupStopIndex();
        CreateQGradientStops();
    }

    SetupSignal(newSignal);
    Transform(newSignal);

    uint32_t azimuth = newSignal.azimuth();
    // Обновляем скан экрана
    UpdateScan(azimuth);
}

std::vector<SectorBaseItem*>& ScanBufferSceneModule::DataMatrix()
{
    return m_dataMatrix;
}

void ScanBufferSceneModule::SetColor1(const QColor& color) noexcept
{
    m_color1 = color;
}

void ScanBufferSceneModule::SetColor2(const QColor& color) noexcept
{
    m_color2 = color;
}

void ScanBufferSceneModule::SetColor3(const QColor& color) noexcept
{
    m_color3 = color;
    m_colorMax = color;
}

void ScanBufferSceneModule::SetupStopIndex() noexcept
{
    m_stopIndexPolot = (m_currentSignalSize / m_coefStreatch) - 1;
}

void ScanBufferSceneModule::SetValue(int verticalShift, int gain, uint8_t coefStreatch)
{
    std::lock_guard<std::mutex> lock(operateLock);
    m_verticalShift = verticalShift;
    m_gain = gain;
    if(m_coefStreatch != coefStreatch)
    {
        m_coefStreatch = coefStreatch;
        SetupStopIndex();
        CreateQGradientStops();
    }
}

void ScanBufferSceneModule::CreateQGradientStops()
{
    int sizeSignal = GetSizePlotSignal();
    m_gradientPoints.resize(sizeSignal);
    qreal step = 1.0 / sizeSignal;
    int i = 1;
    // заполняем массив с точками
    for(auto& posPoint : m_gradientPoints)
    {
        posPoint.first = i * step;
        i++;
    }
}

int ScanBufferSceneModule::GetSizePlotSignal() noexcept
{
    return static_cast<int>((m_stopIndexPolot - m_startIndexPlot) + 1);
}

void ScanBufferSceneModule::SetValColor(int pos, double val)
{
    m_gradientPoints[pos].second = GetColor(val);
}
