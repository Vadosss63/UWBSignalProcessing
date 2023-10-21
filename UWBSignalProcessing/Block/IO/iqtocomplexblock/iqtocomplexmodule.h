#ifndef IQTOCOMPLEXMODULE_H
#define IQTOCOMPLEXMODULE_H

#include "csignal.h"
#include "iomodule.h"

typedef std::pair<csignal<double>, csignal<double>> OutBuffer;
typedef csignal<double> InBuffer;

class IQToComplexModule : public IOModule<InBuffer, OutBuffer> {

public:
  IQToComplexModule();
  ~IQToComplexModule() override = default;

  void Operate() override;
};

#endif // IQTOCOMPLEXMODULE_H
