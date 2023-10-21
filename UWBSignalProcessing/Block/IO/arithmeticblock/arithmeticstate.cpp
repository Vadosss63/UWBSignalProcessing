#include "arithmeticstate.h"

void SumState::execute(csignal<double> &sigA, csignal<double> &sigB) {
  sigA += sigB;
}

void DiffState::execute(csignal<double> &sigA, csignal<double> &sigB) {
  sigA -= sigB;
}

void MultiState::execute(csignal<double> &sigA, csignal<double> &sigB) {
  sigA *= sigB;
}

void DivState::execute(csignal<double> &sigA, csignal<double> &sigB) {
  sigA /= sigB;
}
