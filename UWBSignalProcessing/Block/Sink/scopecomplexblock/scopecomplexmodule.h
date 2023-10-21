#ifndef SCOPECOMPLEXMODULE_H
#define SCOPECOMPLEXMODULE_H

#include "csignal.h"
#include "plotter_interface.h"
#include "sinkmodule.h"
#include <QObject>
#include <QTimer>

using buffer = std::pair<csignal<double>, csignal<double>>;

class ScopeComplexModule : public CSignalComplexPlotter,
                           public SinkModule<buffer> {
  Q_OBJECT
public:
  ScopeComplexModule();
  ~ScopeComplexModule() override = default;

  bool PopSignal(buffer &csignal);

  void Operate() override;
  void StopEvent() override;

signals:
  // сигнал добавления данных в буфер
  void BufferUpdated();

private:
  // потоко-безопастный буфер
  tbb::concurrent_queue<buffer> m_dataCsignal;
};

#endif // SCOPECOMPLEXMODULE_H
