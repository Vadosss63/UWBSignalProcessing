#ifndef DATAVRLI_H
#define DATAVRLI_H

#include <stdint.h>

//#pragma pack(push, 1)
struct DataVRLI {
  // номер цели
  uint32_t Nc;
  // угол от -45 до 45
  float angel;
  // расстояние в метрах
  float range;
  // амплитуда цели(характеристика)
  float amp;

} __attribute__((packed));
//#pragma pack(pop)

#endif // DATAVRLI_H
