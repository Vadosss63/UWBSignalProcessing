#ifndef SCOPEBUFFERMODULE_H
#define SCOPEBUFFERMODULE_H

#include "plotter_interface.h"
#include "sinkmodule.h"

using buffer = csignal<double>;
class ScopeBufferModule : public CSignalPlotter, public SinkModule<buffer> {
  Q_OBJECT
public:
  ScopeBufferModule();
  ~ScopeBufferModule() override = default;

  bool PopSignal(buffer &csignal) override;

  void Operate() override;
  void StopEvent() override;

signals:
  // сигнал добавления данных в буфер
  void BufferUpdated();

private:
  // потоко-безопастный буфер
  tbb::concurrent_queue<buffer> m_dataCsignal;
};

#endif // SCOPEBUFFERMODULE_H
