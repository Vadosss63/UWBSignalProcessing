#ifndef LOGICMODULE_H
#define LOGICMODULE_H

#include "csignal.h"
#include "iomodule.h"

class LogicModule : public IOModule<csignal<double>, csignal<double>> {

public:
  LogicModule();
  ~LogicModule() override = default;

  void Operate() override;

private:
  volatile bool m_sendFirst = true;
};

#endif // LOGICMODULE_H
