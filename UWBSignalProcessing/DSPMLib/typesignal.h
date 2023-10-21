#ifndef TYPESIGNAL_H
#define TYPESIGNAL_H

#include <stdint.h>

enum class TypeSignal : uint8_t {
  doubleType = 8,
  int_8_Type = 1,
  int_16_Type = 2,
  uint_8_Type = 3,
  uint_16_Type = 4,
};

#endif // TYPESIGNAL_H
