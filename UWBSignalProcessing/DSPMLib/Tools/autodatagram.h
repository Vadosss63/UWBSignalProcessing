#ifndef AUTODATAGRAM_H
#define AUTODATAGRAM_H

#include <functional>

namespace AutoDatagram {

#pragma pack(push, 1)
struct Header {
  // количество пакетов
  uint32_t type;
  uint32_t countTarget;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DataTarget {
  // скорость км/ч
  float speed = 0;
  // дистанция м
  float distence = 0;
  // пелинг в градусах
  float direction = 0;
  // амплитуда отн. ед.
  float amp = 0;
  // предпологаемое начало цели м
  float startDistence = 0;
  // предпологаемый конец цели м
  float stopDistence = 0;
};
#pragma pack(pop)

} // namespace AutoDatagram

#endif // AUTODATAGRAM_H
