#ifndef SHOWTARGETMODULE_H
#define SHOWTARGETMODULE_H

#include "intrface.h"
#include "sinkmodule.h"
#include "target.h"
#include <algorithm>
#include <iostream>

class ShowTargetModule : public SinkModule<std::vector<Target>> {

public:
  ShowTargetModule();
  ~ShowTargetModule() override = default;

  // Установка интерфейса обновления класса
  void SetUpdateScane(UpdateScane *updateScane);
  void Operate() override;

private:
  UpdateScane *m_updateScane = nullptr;
};

#endif // SHOWTARGETMODULE_H
