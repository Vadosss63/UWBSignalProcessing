#ifndef UPDATEDATATARGET_H
#define UPDATEDATATARGET_H

#include <vector>

class Target;

class UpdateDataTarget
{

public:
    virtual ~UpdateDataTarget() = default;
    virtual void AddData(const std::vector<Target>& data) = 0;
    virtual void ResetData() = 0;
    virtual void Update() = 0;
};

#endif // UPDATEDATATARGET_H
