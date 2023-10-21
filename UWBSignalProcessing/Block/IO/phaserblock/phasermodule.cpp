#include "phasermodule.h"

PhaserModule::PhaserModule() : IOModule(2, 2) {}

void PhaserModule::Operate() {
  csignal<double> ITemp;
  if (!PopFromInput(ITemp, 0))
    return;
  csignal<double> QTemp;
  if (!PopFromInput(QTemp, 1))
    return;

  std::complex<double> expJ = std::exp(std::complex<double>(0, -m_phase));
  std::complex<double> temp;
  for (size_t i = 0; i < QTemp.size(); ++i) {
    temp = std::complex<double>(ITemp[i], QTemp[i]) * expJ;
    ITemp[i] = temp.real();
    QTemp[i] = temp.imag();
  }

  //отправка I канала
  PushToOutput(std::move(ITemp), 0);
  //отправка Q канала
  PushToOutput(std::move(QTemp), 1);
}

void PhaserModule::SetPhase(double phase) {
  m_phase = (phase * 2 * M_PI) / (360);
}
