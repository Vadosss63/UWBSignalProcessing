#ifndef SPLITTERMODULE_H
#define SPLITTERMODULE_H

#include "csignal.h"
#include "iomodule.h"

class SplitterModule : public IOModule<csignal<double>, csignal<double>> {

public:
  SplitterModule();
  ~SplitterModule() override = default;

  void Operate() override;
};

#endif // SPLITTERELEMENT_H
