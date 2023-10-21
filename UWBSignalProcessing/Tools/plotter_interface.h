#ifndef PLOTTER_INTERFACE_H
#define PLOTTER_INTERFACE_H

#include <QObject>

#include "csignal.h"

class CSignalPlotter : public QObject {
  Q_OBJECT
public:
  virtual bool PopSignal(csignal<double> &csignal) = 0;
};

class CSignalComplexPlotter : public QObject {
  Q_OBJECT
public:
  virtual bool
  PopSignal(std::pair<csignal<double>, csignal<double>> &csignal) = 0;
};

#endif // PLOTTER_INTERFACE_H
