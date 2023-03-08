#ifndef UPDATESCANE_H
#define UPDATESCANE_H

#include <vector>
#include <QVector>
#include <QPointF>

class SectorBaseItem;

class UpdateScane
{

public:

    UpdateScane() = default;

    virtual ~UpdateScane() = default;
    virtual void Update() = 0;
    virtual void CreateSector(int count) = 0;

    virtual std::vector<SectorBaseItem*>& DataMatrix() = 0;

    virtual QVector<QPointF>& DataPloter() = 0;
    virtual void UpdatePloter() = 0;

    virtual void ResizePloter(int size) = 0;

};

#endif // INTRFACE_H
