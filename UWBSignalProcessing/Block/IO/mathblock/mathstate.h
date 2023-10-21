#ifndef MATHSTATE_H
#define MATHSTATE_H

#include "csignal.h"

class StateMath {
public:
  virtual ~StateMath() = default;
  virtual void execute(csignal<double> &sign) = 0;
};

class StateAbs : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateExp : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateSqrt : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateLog : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateLog10 : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateSin : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateCos : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateTan : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateSinh : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateCosh : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateTanh : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateAsin : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateAcos : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

class StateAtan : public StateMath {
public:
  void execute(csignal<double> &sign) override;
};

#endif // MATHSTATE_H
