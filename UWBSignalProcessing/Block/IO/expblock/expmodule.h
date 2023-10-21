#ifndef EXPMODULE_H
#define EXPMODULE_H

#include "csignal.h"
#include "iomodule.h"

class ExpModule : public IOModule<csignal<double>, csignal<double>> {

public:
  ExpModule();
  ~ExpModule() override = default;

  void Operate() override;

  void SetParameters(double threshold);

private:
  double m_threshold = 0;
};

#endif // EXPMODULE_H
