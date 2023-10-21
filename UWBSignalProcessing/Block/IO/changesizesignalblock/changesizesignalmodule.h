#ifndef CHANGESIZESIGNALMODULE_H
#define CHANGESIZESIGNALMODULE_H

#include "csignal.h"
#include "iomodule.h"

class ChangeSizeSignalModule
    : public IOModule<csignal<double>, csignal<double>> {

public:
  ChangeSizeSignalModule();
  ~ChangeSizeSignalModule() override = default;

  ChangeSizeSignalModule(const ChangeSizeSignalModule &rhs) = delete;
  ChangeSizeSignalModule &operator=(const ChangeSizeSignalModule &rhs) = delete;

  void Operate() override;
  void SetParameters(size_t start, size_t size);

private:
  // стартовый индекс
  size_t m_start = 0;
  // размер конечного сигнала
  size_t m_size = 0;
};

#endif // CHANGESIZESIGNALMODULE_H
