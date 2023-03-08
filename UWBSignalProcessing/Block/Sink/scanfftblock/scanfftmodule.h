#ifndef SCANFFTMODULE_H
#define SCANFFTMODULE_H

#include <algorithm>
#include <QObject>
#include <QColor>
#include <iostream>
#include <QGraphicsWidget>

#include "sectoritem.h"
#include "sinkmodule.h"
#include "csignal.h"
#include "intrface.h"

class ScanfftModule: public QObject, public SinkModule<std::vector<std::valarray<double>>>
{
    Q_OBJECT
public:

    ScanfftModule(QObject* parent = nullptr);
    ~ScanfftModule() override = default;

    void Operate() override;
    // возврощает указатель на матрицу отображения
    std::vector<SectorBaseItem*>& DataMatrix();

    void SetUpdateScane(UpdateScane* updateScane);
    void ResetSize(size_t size, size_t count);
    void SetIsLog(bool isLog);
    void SetPloterLine(int ploterLine);
    void SetIsVerticalLine(bool isVertical);

signals:

    void ScanUpdated(); // Изменение элемента матрицы

private:

    void Transform(std::vector<std::valarray<double>>& newSignal);
    inline int CheckValColor(double val) noexcept;
    QColor GetColor(double val);
    // формируем вектор градиентов
    void CreateQGradientStops();
    // получаем длину сигнала для отрисовки
    int GetSizePlotSignal() noexcept;
    // устанавливаем значение яркости в отображении
    inline void SetValColor(int pos, double val);

    // вектор градиентов для отображения
    QGradientStops m_gradientPoints;
    // длина принимаемого сигнала
    size_t m_size = 0;
    int m_ploterLine = 30;
    bool m_isLog = false;
    bool m_isVertical = false;
    UpdateScane * m_updateScane = nullptr;

};

#endif // SCANFFTMODULE_H
