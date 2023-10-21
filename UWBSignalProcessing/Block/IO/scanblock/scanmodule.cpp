#include "scanmodule.h"

ScanModule::ScanModule() : IOModule(1, 1) { ResetScan(); }

void ScanModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  csignal<double> temp;
  if (!PopFromInput(temp))
    return;

  if (temp.azimuth() <= m_countAngel) {
    m_scan[temp.azimuth()].swap(temp);
    if (IsFullScan()) {
      std::vector<csignal<double>> temp;
      temp.swap(m_scan);
      PushToOutput(std::move(temp));
      ResetScan();
    }
  }
}

bool ScanModule::IsFullScan() {
  return !std::any_of(m_scan.begin(), m_scan.end(),
                      [](csignal<double> &csignal) { return csignal.empty(); });
}

void ScanModule::ResetScan() {
  m_scan.clear();
  m_scan.resize(m_countAngel);
}
