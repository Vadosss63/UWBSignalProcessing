#ifndef VRLIVIEWMODULE_H
#define VRLIVIEWMODULE_H

#include <algorithm>
#include <QObject>
#include "sinkmodule.h"

#include "vrliitem.h"
#include "datavrli.h"

class VRLIViewModule: public QObject, public SinkModule<std::vector<DataVRLI>>
{
    Q_OBJECT
public:

    VRLIViewModule(QObject* parent = nullptr);
    ~VRLIViewModule() override = default;

    void Operate() override;
    std::vector<VRLIItem*>& GetVRLIItem();
    void SetCoeffRange(float coeffRange);

signals:   
    void ScanUpdated(); // Изменение элемента матрицы

private:

    void PasingTarget();

    std::vector<DataVRLI> m_dataTarget;
    std::vector<VRLIItem*> m_VRLIItem;
    // 1024 укладывается 2048 точек
    float m_coeffRange = 0.5;
    // коефициент перевода в реальные углы
    static constexpr float m_coeffAngel = 90.f/64.f;
    static constexpr float m_startAngel = -45.f;
};

#endif // VRLIVIEWMODULE_H
