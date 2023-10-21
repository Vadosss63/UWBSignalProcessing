#ifndef D3TARGETMODULE_H
#define D3TARGETMODULE_H

#include "interface.h"
#include "sinkmodule.h"
#include "target.h"
#include <algorithm>
#include <iostream>

using inBuffer = std::vector<Target>;

class D3TargetModule : public SinkModule<inBuffer> {
public:
  D3TargetModule(UpdateDataTarget *update);
  ~D3TargetModule() override = default;

  void Operate() override;

private:
  UpdateDataTarget *m_update;
  size_t m_countData = 256;
  size_t m_currentData = 0;
};

#endif // D3TARGETMODULE_H
