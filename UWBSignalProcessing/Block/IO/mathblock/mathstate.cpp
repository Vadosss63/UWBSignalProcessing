#include "mathstate.h"

void StateAbs::execute(csignal<double> &sign) { sign.abs(); }

void StateExp::execute(csignal<double> &sign) { sign.exp(); }

void StateSqrt::execute(csignal<double> &sign) { sign.sqrt(); }

void StateLog::execute(csignal<double> &sign) { sign.log(); }

void StateLog10::execute(csignal<double> &sign) { sign.log10(); }

void StateSin::execute(csignal<double> &sign) { sign.sin(); }

void StateCos::execute(csignal<double> &sign) { sign.cos(); }

void StateTan::execute(csignal<double> &sign) { sign.tan(); }

void StateSinh::execute(csignal<double> &sign) { sign.sinh(); }

void StateCosh::execute(csignal<double> &sign) { sign.cosh(); }

void StateTanh::execute(csignal<double> &sign) { sign.tanh(); }

void StateAsin::execute(csignal<double> &sign) { sign.asin(); }

void StateAcos::execute(csignal<double> &sign) { sign.acos(); }

void StateAtan::execute(csignal<double> &sign) { sign.atan(); }
