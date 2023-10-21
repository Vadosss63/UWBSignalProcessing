#ifndef CONTROLFRAMES_H
#define CONTROLFRAMES_H

#include <algorithm>
#include <iostream>

#include "csignal.h"
#include "iomodule.h"

class ControlFramesModule : public IOModule<csignal<double>, csignal<double>> {

public:
  ControlFramesModule();
  ~ControlFramesModule() override = default;

  void Operate() override;

private:
  unsigned short m_frame;
};

#endif // CONTROLFRAMES_H
