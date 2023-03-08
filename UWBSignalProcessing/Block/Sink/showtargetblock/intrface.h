#ifndef SHOWTARGETUPDATESCANE_H
#define SHOWTARGETUPDATESCANE_H

#include <vector>

class Target;

class UpdateScane
{

public:
    UpdateScane() = default;

    virtual ~UpdateScane() = default;
    virtual void AddTarget(std::vector<Target>&& targets) = 0;
    virtual void ResetTarget() = 0;
    virtual void Update() = 0;
};

#endif // SHOWTARGETUPDATESCANE_H
