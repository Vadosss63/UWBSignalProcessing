#ifndef MODMODULE_H
#define MODMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <vector>

class ThresholdModule : public IOModule<csignal<double>, csignal<double>> {

public:
  ThresholdModule();
  ~ThresholdModule() override = default;
  void Operate() override;
};
#endif // MODMODULE_H
