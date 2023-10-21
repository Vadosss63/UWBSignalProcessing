#include "upsamplingmodule.h"
#include <algorithm>

UpsamplingModule::UpsamplingModule()
    : IOModule(1, 1), m_tempInAndOut(8192), m_expectedMutex(false) {}

void UpsamplingModule::Operate() {
  csignal<double> temp;

  if (!PopFromInput(temp))
    return;

  bool flag = false;
  if (m_expectedMutex.compare_exchange_strong(flag, true)) {
    std::fill(m_tempInAndOut.begin(), m_tempInAndOut.end(), 0);
    m_tempInAndOut.setServiceParameters(temp.azimuth(), temp.frameNumber());
    size_t k = 0;
    for (size_t i = 0; i < m_tempInAndOut.size(); i += m_upsamplingVal)
      m_tempInAndOut[i] = temp[k++];

    PushToOutput((m_tempInAndOut));
    m_expectedMutex.store(false);
    return;
  }
}

void UpsamplingModule::SetUpsamplingValue(int upsamp_value) {
  m_upsamplingVal = upsamp_value;

  if (m_upsamplingVal == 2)
    m_tempInAndOut.reset(4096);

  if (m_upsamplingVal == 4)
    m_tempInAndOut.reset(8192);

  if (m_upsamplingVal == 8)
    m_tempInAndOut.reset(16384);
}
