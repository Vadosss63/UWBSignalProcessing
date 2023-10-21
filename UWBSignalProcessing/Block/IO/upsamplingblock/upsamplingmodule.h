#ifndef UPSAMPLINGMODULE_H
#define UPSAMPLINGMODULE_H

#include "csignal.h"
#include "iomodule.h"
#include <atomic>
#include <fftw3.h>

class UpsamplingModule : public IOModule<csignal<double>, csignal<double>> {

public:
  UpsamplingModule();
  ~UpsamplingModule() override = default;

  void Operate() override;

  void SetUpsamplingValue(int upsamp_value);

private:
  // коофициент повышения частоты дискретизации
  size_t m_upsamplingVal = 4;
  // темповые контейнеры для выполнения преобразований фурье
  csignal<double> m_tempInAndOut;
  int test = 0;
  std::atomic_bool m_expectedMutex;
};

#endif // UPSAMPLINGMODULE_H
