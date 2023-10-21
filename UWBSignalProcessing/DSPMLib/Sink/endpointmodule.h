#ifndef ENDPOINTMODULE_H
#define ENDPOINTMODULE_H

#include "sinkmodule.h"

// Класс заглушки портов элементов
// Разработчик Парусов В.А.
class EndPointModule : public SinkModule<::csignal<double>> {
  Q_OBJECT
public:
  explicit EndPointModule(QObject *parent = nullptr);
  ~EndPointModule() override;

  EndPointModule(const EndPointModule &rhs) = delete;
  EndPointModule &operator=(const EndPointModule &rhs) = delete;
  //принимает данные из порта  и никуда их не передает.
  void Operate() override;
};
}
#endif // ENDPOINTMODULE_H
