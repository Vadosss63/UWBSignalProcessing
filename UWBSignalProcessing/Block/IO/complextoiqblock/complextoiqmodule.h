#ifndef COMPLEXTOIQMODULE_H
#define COMPLEXTOIQMODULE_H

#include "csignal.h"
#include "iomodule.h"

typedef std::pair<csignal<double>, csignal<double>> InBuffer;
typedef csignal<double> OutBuffer;

class ComplexToIQModule : public IOModule<InBuffer, OutBuffer> {

public:
  ComplexToIQModule();
  ~ComplexToIQModule() override = default;

  void Operate() override;
};

#endif // COMPLEXTOIQMODULE_H
