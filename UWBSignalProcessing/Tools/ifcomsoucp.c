#include "ifcomsoucp.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int write_RES(ifcomsoucp_Status *status, ifcomsoucp_RES *s);
int read_RES(ifcomsoucp_Status *status, ifcomsoucp_RES *s);
int write_CRPlan_Point(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Point *s);
int read_CRPlan_Point(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Point *s);
int write_DestroyObject(ifcomsoucp_Status *status, ifcomsoucp_DestroyObject *s);
int read_DestroyObject(ifcomsoucp_Status *status, ifcomsoucp_DestroyObject *s);
int write_Time(ifcomsoucp_Status *status, ifcomsoucp_Time *s);
int read_Time(ifcomsoucp_Status *status, ifcomsoucp_Time *s);
int write_Meteo(ifcomsoucp_Status *status, ifcomsoucp_Meteo *s);
int read_Meteo(ifcomsoucp_Status *status, ifcomsoucp_Meteo *s);
void write_bitfield_uint16_t_rl(ifcomsoucp_DataStream *data, uint16_t value,
                                size_t bitsCount);
uint16_t read_bitfield_uint16_t_rl(ifcomsoucp_DataStream *data,
                                   size_t bitsCount);
int write_CRPlan_Object(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Object *s);
int read_CRPlan_Object(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Object *s);
void write_float_be(ifcomsoucp_DataStream *data, float value);
float read_float_be(ifcomsoucp_DataStream *data);
int write_VzoiVzorData(ifcomsoucp_Status *status, ifcomsoucp_VzoiVzorData *s);
int read_VzoiVzorData(ifcomsoucp_Status *status, ifcomsoucp_VzoiVzorData *s);
void write_bitfield_uint8_t_rl(ifcomsoucp_DataStream *data, uint8_t value,
                               size_t bitsCount);
uint8_t read_bitfield_uint8_t_rl(ifcomsoucp_DataStream *data, size_t bitsCount);
int write_ObjectInfo(ifcomsoucp_Status *status, ifcomsoucp_ObjectInfo *s);
int read_ObjectInfo(ifcomsoucp_Status *status, ifcomsoucp_ObjectInfo *s);
void write_uint_24_be(ifcomsoucp_DataStream *data, uint32_t value);
uint32_t read_uint_24_be(ifcomsoucp_DataStream *data);
void write_uint_32_be(ifcomsoucp_DataStream *data, uint32_t value);
uint32_t read_uint_32_be(ifcomsoucp_DataStream *data);
int write_Control(ifcomsoucp_Status *status, ifcomsoucp_Control *s);
int read_Control(ifcomsoucp_Status *status, ifcomsoucp_Control *s);
void write_bitfield_uint32_t_rl(ifcomsoucp_DataStream *data, uint32_t value,
                                size_t bitsCount);
uint32_t read_bitfield_uint32_t_rl(ifcomsoucp_DataStream *data,
                                   size_t bitsCount);
int write_Header(ifcomsoucp_Status *status, ifcomsoucp_Header *s);
int read_Header(ifcomsoucp_Status *status, ifcomsoucp_Header *s);
void write_uint_8(ifcomsoucp_DataStream *data, uint8_t value);
uint8_t read_uint_8(ifcomsoucp_DataStream *data);
void write_uint_16_be(ifcomsoucp_DataStream *data, uint16_t value);
uint16_t read_uint_16_be(ifcomsoucp_DataStream *data);

int write_RES(ifcomsoucp_Status *status, ifcomsoucp_RES *s) {
  write_bitfield_uint8_t_rl(&status->base.data, s->type, 4);
  write_bitfield_uint32_t_rl(&status->base.data, s->numberInDB, 28);
  write_float_be(&status->base.data, s->frequency);
  return 1;
}

int read_RES(ifcomsoucp_Status *status, ifcomsoucp_RES *s) {
  s->type = read_bitfield_uint8_t_rl(&status->base.data, 4);
  s->numberInDB = read_bitfield_uint32_t_rl(&status->base.data, 28);
  s->frequency = read_float_be(&status->base.data);
  return 1;
}

int write_CRPlan_Point(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Point *s) {
  write_float_be(&status->base.data, s->latitude);
  write_float_be(&status->base.data, s->longitude);
  write_float_be(&status->base.data, s->altitude);
  return 1;
}

int read_CRPlan_Point(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Point *s) {
  s->latitude = read_float_be(&status->base.data);
  s->longitude = read_float_be(&status->base.data);
  s->altitude = read_float_be(&status->base.data);
  return 1;
}

int write_DestroyObject(ifcomsoucp_Status *status,
                        ifcomsoucp_DestroyObject *s) {
  write_uint_32_be(&status->base.data, s->ID);
  return 1;
}

int read_DestroyObject(ifcomsoucp_Status *status, ifcomsoucp_DestroyObject *s) {
  s->ID = read_uint_32_be(&status->base.data);
  return 1;
}

int write_Time(ifcomsoucp_Status *status, ifcomsoucp_Time *s) {
  write_uint_32_be(&status->base.data, s->sec);
  write_uint_32_be(&status->base.data, s->msec);
  return 1;
}

int read_Time(ifcomsoucp_Status *status, ifcomsoucp_Time *s) {
  s->sec = read_uint_32_be(&status->base.data);
  s->msec = read_uint_32_be(&status->base.data);
  return 1;
}

int write_Meteo(ifcomsoucp_Status *status, ifcomsoucp_Meteo *s) {
  write_float_be(&status->base.data, s->A11);
  write_float_be(&status->base.data, s->A12);
  write_float_be(&status->base.data, s->A13);
  write_float_be(&status->base.data, s->A14);
  write_float_be(&status->base.data, s->A21);
  write_float_be(&status->base.data, s->A22);
  write_float_be(&status->base.data, s->A23);
  write_float_be(&status->base.data, s->A24);
  write_float_be(&status->base.data, s->A31);
  write_float_be(&status->base.data, s->A32);
  write_float_be(&status->base.data, s->A33);
  write_float_be(&status->base.data, s->A34);
  write_float_be(&status->base.data, s->A41);
  write_float_be(&status->base.data, s->A42);
  write_float_be(&status->base.data, s->A43);
  write_float_be(&status->base.data, s->A44);
  write_float_be(&status->base.data, s->B11);
  write_float_be(&status->base.data, s->B12);
  write_float_be(&status->base.data, s->B21);
  write_float_be(&status->base.data, s->B22);
  write_float_be(&status->base.data, s->B31);
  write_float_be(&status->base.data, s->B32);
  write_float_be(&status->base.data, s->INT);
  return 1;
}

int read_Meteo(ifcomsoucp_Status *status, ifcomsoucp_Meteo *s) {
  s->A11 = read_float_be(&status->base.data);
  s->A12 = read_float_be(&status->base.data);
  s->A13 = read_float_be(&status->base.data);
  s->A14 = read_float_be(&status->base.data);
  s->A21 = read_float_be(&status->base.data);
  s->A22 = read_float_be(&status->base.data);
  s->A23 = read_float_be(&status->base.data);
  s->A24 = read_float_be(&status->base.data);
  s->A31 = read_float_be(&status->base.data);
  s->A32 = read_float_be(&status->base.data);
  s->A33 = read_float_be(&status->base.data);
  s->A34 = read_float_be(&status->base.data);
  s->A41 = read_float_be(&status->base.data);
  s->A42 = read_float_be(&status->base.data);
  s->A43 = read_float_be(&status->base.data);
  s->A44 = read_float_be(&status->base.data);
  s->B11 = read_float_be(&status->base.data);
  s->B12 = read_float_be(&status->base.data);
  s->B21 = read_float_be(&status->base.data);
  s->B22 = read_float_be(&status->base.data);
  s->B31 = read_float_be(&status->base.data);
  s->B32 = read_float_be(&status->base.data);
  s->INT = read_float_be(&status->base.data);
  return 1;
}

void write_bitfield_uint16_t_rl(ifcomsoucp_DataStream *data, uint16_t value,
                                size_t bitsCount) {
  while (bitsCount > 0) {
    if (data->bitPtr == 0)
      data->data[data->dataPtr] = 0;
    size_t bitsToMove = 8 - data->bitPtr;

    if (bitsToMove < bitsCount) {
      data->data[data->dataPtr] |= (uint8_t)(value >> (bitsCount - bitsToMove));
      data->bitPtr = 0;
      data->dataPtr += 1;
      bitsCount -= bitsToMove;
    } else {
      data->data[data->dataPtr] |= (uint8_t)(value << (bitsToMove - bitsCount));
      data->bitPtr += bitsCount;
      bitsCount = 0;
      if (data->bitPtr >= 8) {
        data->dataPtr++;
        data->bitPtr = 0;
      }
    }
  }
}

uint16_t read_bitfield_uint16_t_rl(ifcomsoucp_DataStream *data,
                                   size_t bitsCount) {
  uint16_t value = 0;
  while (bitsCount > 0) {
    uint8_t byte = data->data[data->dataPtr];
    byte <<= data->bitPtr;
    byte >>= data->bitPtr;

    size_t bitstomove = 8 - data->bitPtr;
    if (bitstomove < bitsCount) {
      value |= byte << (bitsCount - bitstomove);
    } else {
      value |= byte >> (bitstomove - bitsCount);
      bitstomove = bitsCount;
    }

    bitsCount -= bitstomove;
    data->bitPtr += bitstomove;

    if (data->bitPtr >= 8) {
      data->dataPtr++;
      data->bitPtr = 0;
    }
  }
  return value;
}

int write_CRPlan_Object(ifcomsoucp_Status *status,
                        ifcomsoucp_CRPlan_Object *s) {
  write_uint_32_be(&status->base.data, s->targetID);
  return 1;
}

int read_CRPlan_Object(ifcomsoucp_Status *status, ifcomsoucp_CRPlan_Object *s) {
  s->targetID = read_uint_32_be(&status->base.data);
  return 1;
}

void write_float_be(ifcomsoucp_DataStream *data, float value) {
  uint8_t *t = data->data + data->dataPtr + 3;
  uint32_t *tmp = (uint32_t *)&value;

  *(t--) = (uint8_t)(*tmp);
  *tmp = *tmp >> 8;
  *(t--) = (uint8_t)(*tmp);
  *tmp = *tmp >> 8;
  *(t--) = (uint8_t)(*tmp);
  *tmp = *tmp >> 8;
  *t = (uint8_t)(*tmp);

  data->dataPtr += 4;
}

float read_float_be(ifcomsoucp_DataStream *data) {
  uint8_t *t = data->data + data->dataPtr;

  int32_t tmp = *(t++);
  tmp = (tmp << 8) | *(t++);
  tmp = (tmp << 8) | *(t++);
  tmp = (tmp << 8) | *t;

  data->dataPtr += 4;
  return *((float *)&tmp);
}

int write_VzoiVzorData(ifcomsoucp_Status *status, ifcomsoucp_VzoiVzorData *s) {
  write_uint_16_be(&status->base.data, s->distance);
  write_uint_16_be(&status->base.data, s->latency);
  write_uint_32_be(&status->base.data, s->groupID);
  return 1;
}

int read_VzoiVzorData(ifcomsoucp_Status *status, ifcomsoucp_VzoiVzorData *s) {
  s->distance = read_uint_16_be(&status->base.data);
  s->latency = read_uint_16_be(&status->base.data);
  s->groupID = read_uint_32_be(&status->base.data);
  return 1;
}

void write_bitfield_uint8_t_rl(ifcomsoucp_DataStream *data, uint8_t value,
                               size_t bitsCount) {
  while (bitsCount > 0) {
    if (data->bitPtr == 0)
      data->data[data->dataPtr] = 0;
    size_t bitsToMove = 8 - data->bitPtr;

    if (bitsToMove < bitsCount) {
      data->data[data->dataPtr] |= (uint8_t)(value >> (bitsCount - bitsToMove));
      data->bitPtr = 0;
      data->dataPtr += 1;
      bitsCount -= bitsToMove;
    } else {
      data->data[data->dataPtr] |= (uint8_t)(value << (bitsToMove - bitsCount));
      data->bitPtr += bitsCount;
      bitsCount = 0;
      if (data->bitPtr >= 8) {
        data->dataPtr++;
        data->bitPtr = 0;
      }
    }
  }
}

uint8_t read_bitfield_uint8_t_rl(ifcomsoucp_DataStream *data,
                                 size_t bitsCount) {
  uint8_t value = 0;
  while (bitsCount > 0) {
    uint8_t byte = data->data[data->dataPtr];
    byte <<= data->bitPtr;
    byte >>= data->bitPtr;

    size_t bitstomove = 8 - data->bitPtr;
    if (bitstomove < bitsCount) {
      value |= byte << (bitsCount - bitstomove);
    } else {
      value |= byte >> (bitstomove - bitsCount);
      bitstomove = bitsCount;
    }

    bitsCount -= bitstomove;
    data->bitPtr += bitstomove;

    if (data->bitPtr >= 8) {
      data->dataPtr++;
      data->bitPtr = 0;
    }
  }
  return value;
}

int write_ObjectInfo(ifcomsoucp_Status *status, ifcomsoucp_ObjectInfo *s) {
  write_uint_32_be(&status->base.data, s->objectDBID);
  write_uint_32_be(&status->base.data, s->objectID);
  write_float_be(&status->base.data, s->latitude);
  write_float_be(&status->base.data, s->longitude);
  write_float_be(&status->base.data, s->altitude);
  write_float_be(&status->base.data, s->velocityX);
  write_float_be(&status->base.data, s->velocityY);
  write_float_be(&status->base.data, s->velocityZ);
  write_float_be(&status->base.data, s->course);
  write_float_be(&status->base.data, s->tangage);
  write_float_be(&status->base.data, s->roll);
  write_bitfield_uint8_t_rl(&status->base.data, s->objectClass, 7);
  write_bitfield_uint8_t_rl(&status->base.data, s->objectType, 7);
  write_bitfield_uint8_t_rl(&status->base.data, s->typeExtData, 5);
  status->base.ObjectInfo_typeExtData = s->typeExtData;
  write_bitfield_uint16_t_rl(&status->base.data, s->countExtData, 13);
  status->base.ObjectInfo_countExtData = s->countExtData;
  return 1;
}

int read_ObjectInfo(ifcomsoucp_Status *status, ifcomsoucp_ObjectInfo *s) {
  s->objectDBID = read_uint_32_be(&status->base.data);
  s->objectID = read_uint_32_be(&status->base.data);
  s->latitude = read_float_be(&status->base.data);
  s->longitude = read_float_be(&status->base.data);
  s->altitude = read_float_be(&status->base.data);
  s->velocityX = read_float_be(&status->base.data);
  s->velocityY = read_float_be(&status->base.data);
  s->velocityZ = read_float_be(&status->base.data);
  s->course = read_float_be(&status->base.data);
  s->tangage = read_float_be(&status->base.data);
  s->roll = read_float_be(&status->base.data);
  s->objectClass = read_bitfield_uint8_t_rl(&status->base.data, 7);
  s->objectType = read_bitfield_uint8_t_rl(&status->base.data, 7);
  s->typeExtData = read_bitfield_uint8_t_rl(&status->base.data, 5);
  status->base.ObjectInfo_typeExtData = s->typeExtData;
  s->countExtData = read_bitfield_uint16_t_rl(&status->base.data, 13);
  status->base.ObjectInfo_countExtData = s->countExtData;
  return 1;
}

void write_uint_24_be(ifcomsoucp_DataStream *data, uint32_t value) {
  uint8_t *t = data->data + data->dataPtr + 2;

  *(t--) = (uint8_t)(value);
  value = value >> 8;
  *(t--) = (uint8_t)(value);
  value = value >> 8;
  *t = (uint8_t)(value);

  data->dataPtr += 3;
}

uint32_t read_uint_24_be(ifcomsoucp_DataStream *data) {
  uint8_t *t = data->data + data->dataPtr;

  uint32_t result = *(t++);
  result = (result << 8) | *(t++);
  result = (result << 8) | *t;

  data->dataPtr += 3;
  return result;
}

void write_uint_32_be(ifcomsoucp_DataStream *data, uint32_t value) {
  uint8_t *t = data->data + data->dataPtr + 3;

  *(t--) = (uint8_t)(value);
  value = value >> 8;
  *(t--) = (uint8_t)(value);
  value = value >> 8;
  *(t--) = (uint8_t)(value);
  value = value >> 8;
  *t = (uint8_t)(value);

  data->dataPtr += 4;
}

uint32_t read_uint_32_be(ifcomsoucp_DataStream *data) {
  uint8_t *t = data->data + data->dataPtr;

  uint32_t result = *(t++);
  result = (result << 8) | *(t++);
  result = (result << 8) | *(t++);
  result = (result << 8) | *t;

  data->dataPtr += 4;
  return result;
}

int write_Control(ifcomsoucp_Status *status, ifcomsoucp_Control *s) {
  write_uint_8(&status->base.data, s->type);
  status->base.Control_type = s->type;
  write_uint_24_be(&status->base.data, s->selfID);
  return 1;
}

int read_Control(ifcomsoucp_Status *status, ifcomsoucp_Control *s) {
  s->type = read_uint_8(&status->base.data);
  status->base.Control_type = s->type;
  s->selfID = read_uint_24_be(&status->base.data);
  return 1;
}

void write_bitfield_uint32_t_rl(ifcomsoucp_DataStream *data, uint32_t value,
                                size_t bitsCount) {
  while (bitsCount > 0) {
    if (data->bitPtr == 0)
      data->data[data->dataPtr] = 0;
    size_t bitsToMove = 8 - data->bitPtr;

    if (bitsToMove < bitsCount) {
      data->data[data->dataPtr] |= (uint8_t)(value >> (bitsCount - bitsToMove));
      data->bitPtr = 0;
      data->dataPtr += 1;
      bitsCount -= bitsToMove;
    } else {
      data->data[data->dataPtr] |= (uint8_t)(value << (bitsToMove - bitsCount));
      data->bitPtr += bitsCount;
      bitsCount = 0;
      if (data->bitPtr >= 8) {
        data->dataPtr++;
        data->bitPtr = 0;
      }
    }
  }
}

uint32_t read_bitfield_uint32_t_rl(ifcomsoucp_DataStream *data,
                                   size_t bitsCount) {
  uint32_t value = 0;
  while (bitsCount > 0) {
    uint8_t byte = data->data[data->dataPtr];
    byte <<= data->bitPtr;
    byte >>= data->bitPtr;

    size_t bitstomove = 8 - data->bitPtr;
    if (bitstomove < bitsCount) {
      value |= byte << (bitsCount - bitstomove);
    } else {
      value |= byte >> (bitstomove - bitsCount);
      bitstomove = bitsCount;
    }

    bitsCount -= bitstomove;
    data->bitPtr += bitstomove;

    if (data->bitPtr >= 8) {
      data->dataPtr++;
      data->bitPtr = 0;
    }
  }
  return value;
}

int write_Header(ifcomsoucp_Status *status, ifcomsoucp_Header *s) {
  write_uint_16_be(&status->base.data, s->messageType);
  status->base.Header_messageType = s->messageType;
  write_uint_8(&status->base.data, s->packageCount);
  write_uint_8(&status->base.data, s->packageFragmentID);
  write_uint_32_be(&status->base.data, s->messageCyclicNumber);
  write_Time(status, &s->time);
  write_uint_8(&status->base.data, s->sndrID);
  write_uint_8(&status->base.data, s->recvID);
  status->base.Header_CRC_dataPtr = status->base.data.dataPtr;
  status->base.Header_CRC_bitPtr = status->base.data.bitPtr;
  write_uint_16_be(&status->base.data, s->CRC);
  status->base.data.dataPtr = status->base.data.dataPtr + 2;
  status->base.Header_words32Count_dataPtr = status->base.data.dataPtr;
  status->base.Header_words32Count_bitPtr = status->base.data.bitPtr;
  write_uint_16_be(&status->base.data, s->words32Count);
  return 1;
}

int read_Header(ifcomsoucp_Status *status, ifcomsoucp_Header *s) {
  s->messageType = read_uint_16_be(&status->base.data);
  status->base.Header_messageType = s->messageType;
  s->packageCount = read_uint_8(&status->base.data);
  s->packageFragmentID = read_uint_8(&status->base.data);
  s->messageCyclicNumber = read_uint_32_be(&status->base.data);
  read_Time(status, &s->time);
  s->sndrID = read_uint_8(&status->base.data);
  s->recvID = read_uint_8(&status->base.data);
  status->base.Header_CRC_dataPtr = status->base.data.dataPtr;
  status->base.Header_CRC_bitPtr = status->base.data.bitPtr;
  s->CRC = read_uint_16_be(&status->base.data);
  status->base.data.dataPtr = status->base.data.dataPtr + 2;
  status->base.Header_words32Count_dataPtr = status->base.data.dataPtr;
  status->base.Header_words32Count_bitPtr = status->base.data.bitPtr;
  s->words32Count = read_uint_16_be(&status->base.data);
  return 1;
}

void write_uint_8(ifcomsoucp_DataStream *data, uint8_t value) {
  data->data[data->dataPtr++] = (uint8_t)value;
}

uint8_t read_uint_8(ifcomsoucp_DataStream *data) {
  return (uint8_t)data->data[data->dataPtr++];
}

void write_uint_16_be(ifcomsoucp_DataStream *data, uint16_t value) {
  uint8_t *t = data->data + data->dataPtr + 1;

  *(t--) = (uint8_t)(value);
  value = value >> 8;
  *t = (uint8_t)(value);

  data->dataPtr += 2;
}

uint16_t read_uint_16_be(ifcomsoucp_DataStream *data) {
  uint8_t *t = data->data + data->dataPtr;

  uint16_t result = *(t++);
  result = (result << 8) | *t;

  data->dataPtr += 2;
  return result;
}

void ifcomsoucp_init(ifcomsoucp_Status *status, uint8_t *data,
                     size_t dataLength, ifcomsoucp_Mode mode) {
  ifcomsoucp_reset(status, data, dataLength, mode);
  status->name = "ifcomsoucp";
  status->version = "1.0";
}

void ifcomsoucp_reset(ifcomsoucp_Status *status, uint8_t *data,
                      size_t dataLength, ifcomsoucp_Mode mode) {
  memset(status, 0, sizeof(ifcomsoucp_Status));
  switch (mode) {
  case IFCOMSOUCP_FROM_IFCO_MODE:
    status->base.state = 1;
    break;
  case IFCOMSOUCP_TO_IFCO_MODE:
    status->base.state = 15;
    break;
  }
  status->base.stackPtr = -1;
  ifcomsoucp_resetDataStream(status, data, dataLength);
}

void ifcomsoucp_resetDataStream(ifcomsoucp_Status *status, uint8_t *data,
                                size_t dataLength) {
  status->base.data.dataPtr = 0;
  status->base.data.bitPtr = 0;
  status->base.data.data = data;
  status->base.data.dataLength = dataLength;
  status->base.Header_CRC_bitPtr = 9;
  status->base.Header_words32Count_bitPtr = 9;
}

int ifcomsoucp_updateHeader_CRC(ifcomsoucp_Status *status, uint16_t value) {
  if (status->base.Header_CRC_bitPtr > 8)
    return 0;
  size_t dataPtr = status->base.data.dataPtr;
  size_t bitPtr = status->base.data.bitPtr;
  status->base.data.dataPtr = status->base.Header_CRC_dataPtr;
  status->base.data.bitPtr = status->base.Header_CRC_bitPtr;
  status->base.Header_CRC_dataPtr = status->base.data.dataPtr;
  status->base.Header_CRC_bitPtr = status->base.data.bitPtr;
  write_uint_16_be(&status->base.data, value);
  status->base.data.dataPtr = dataPtr;
  status->base.data.bitPtr = bitPtr;
  return 1;
}

int ifcomsoucp_updateHeader_words32Count(ifcomsoucp_Status *status,
                                         uint16_t value) {
  if (status->base.Header_words32Count_bitPtr > 8)
    return 0;
  size_t dataPtr = status->base.data.dataPtr;
  size_t bitPtr = status->base.data.bitPtr;
  status->base.data.dataPtr = status->base.Header_words32Count_dataPtr;
  status->base.data.bitPtr = status->base.Header_words32Count_bitPtr;
  status->base.Header_words32Count_dataPtr = status->base.data.dataPtr;
  status->base.Header_words32Count_bitPtr = status->base.data.bitPtr;
  write_uint_16_be(&status->base.data, value);
  status->base.data.dataPtr = dataPtr;
  status->base.data.bitPtr = bitPtr;
  return 1;
}

void ifcomsoucp_errorRecovery(ifcomsoucp_Status *status) {
  status->base = status->backup;
}

size_t ifcomsoucp_getDataLength(ifcomsoucp_Status *status) {
  return status->base.data.dataPtr;
}

ifcomsoucp_Time *ifcomsoucp_getTime(ifcomsoucp_Status *status) {
  return (ifcomsoucp_Time *)status->base.structPtr;
}

ifcomsoucp_Header *ifcomsoucp_getHeader(ifcomsoucp_Status *status) {
  return (ifcomsoucp_Header *)status->base.structPtr;
}

ifcomsoucp_ObjectInfo *ifcomsoucp_getObjectInfo(ifcomsoucp_Status *status) {
  return (ifcomsoucp_ObjectInfo *)status->base.structPtr;
}

ifcomsoucp_RES *ifcomsoucp_getRES(ifcomsoucp_Status *status) {
  return (ifcomsoucp_RES *)status->base.structPtr;
}

ifcomsoucp_Meteo *ifcomsoucp_getMeteo(ifcomsoucp_Status *status) {
  return (ifcomsoucp_Meteo *)status->base.structPtr;
}

ifcomsoucp_Control *ifcomsoucp_getControl(ifcomsoucp_Status *status) {
  return (ifcomsoucp_Control *)status->base.structPtr;
}

ifcomsoucp_VzoiVzorData *ifcomsoucp_getVzoiVzorData(ifcomsoucp_Status *status) {
  return (ifcomsoucp_VzoiVzorData *)status->base.structPtr;
}

ifcomsoucp_CRPlan_Point *ifcomsoucp_getCRPlan_Point(ifcomsoucp_Status *status) {
  return (ifcomsoucp_CRPlan_Point *)status->base.structPtr;
}

ifcomsoucp_CRPlan_Object *
ifcomsoucp_getCRPlan_Object(ifcomsoucp_Status *status) {
  return (ifcomsoucp_CRPlan_Object *)status->base.structPtr;
}

ifcomsoucp_DestroyObject *
ifcomsoucp_getDestroyObject(ifcomsoucp_Status *status) {
  return (ifcomsoucp_DestroyObject *)status->base.structPtr;
}

void ifcomsoucp_setStructID(ifcomsoucp_Status *status,
                            ifcomsoucp_StructID structID) {
  status->base.structID = structID;
}

ifcomsoucp_StructID ifcomsoucp_getStructID(ifcomsoucp_Status *status) {
  return status->base.structID;
}

int ifcomsoucp_push(ifcomsoucp_Status *status, int id, int count) {
  if (count != 0) {
    if (status->base.stack[status->base.stackPtr].id == id)
      return 1;
    if (++status->base.stackPtr >= 10) {
      printf("ifcomsoucp Protocol ERROR: Stack Overflow\n");
      return 0;
    }
    status->base.stack[status->base.stackPtr].id = id;
    status->base.stack[status->base.stackPtr].count = count;
    return 1;
  }
  return 0;
}

int ifcomsoucp_pop(ifcomsoucp_Status *status) {
  while (status->base.stack[status->base.stackPtr].count - 1 == 0)
    status->base.stackPtr--;
  status->base.stack[status->base.stackPtr].count--;
  return status->base.stack[status->base.stackPtr].id;
}

int ifcomsoucp_writeStruct(ifcomsoucp_Status *status) {
  int stack[10];
  stack[0] = status->base.state;
  int stackPtr = 0;
  while (stackPtr != -1)
    switch (stack[stackPtr--]) {
    case 1:
      stack[++stackPtr] = 2;
      if (!ifcomsoucp_push(status, 1, -1))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 2:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_HEADER)
        continue;
      if (write_Header(status, (ifcomsoucp_Header *)status->base.structPtr)) {
        status->base.state = 3;
        status->backup = status->base;
        return 1;
      }
      continue;
    case 3:
      stack[++stackPtr] = 4;
      if (!(status->base.Header_messageType ==
            IFCOMSOUCP_MESSAGETYPE_OBJECT_INFO))
        stack[stackPtr] = 11;
      else
        ifcomsoucp_push(status, 11, 2);
      continue;
    case 4:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_OBJECTINFO)
        continue;
      if (write_ObjectInfo(status,
                           (ifcomsoucp_ObjectInfo *)status->base.structPtr)) {
        status->base.state = 5;
        status->backup = status->base;
        return 1;
      }
      continue;
    case 5:
      stack[++stackPtr] = 6;
      if (!(status->base.ObjectInfo_typeExtData == IFCOMSOUCP_TYPEEXTDATA_RES))
        stack[stackPtr] = 8;
      else
        ifcomsoucp_push(status, 8, 2);
      continue;
    case 6:
      stack[++stackPtr] = 7;
      if (!ifcomsoucp_push(status, 6, status->base.ObjectInfo_countExtData))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 7:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_RES)
        continue;
      if (write_RES(status, (ifcomsoucp_RES *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        return 1;
      }
      continue;
    case 8:
      stack[++stackPtr] = 9;
      if (!(status->base.ObjectInfo_typeExtData ==
            IFCOMSOUCP_TYPEEXTDATA_METEO))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 9:
      stack[++stackPtr] = 10;
      if (!ifcomsoucp_push(status, 9, status->base.ObjectInfo_countExtData))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 10:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_METEO)
        continue;
      if (write_Meteo(status, (ifcomsoucp_Meteo *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        return 1;
      }
      continue;
    case 11:
      stack[++stackPtr] = 12;
      if (!(status->base.Header_messageType == IFCOMSOUCP_MESSAGETYPE_CONTROL))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 12:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_CONTROL)
        continue;
      if (write_Control(status, (ifcomsoucp_Control *)status->base.structPtr)) {
        status->base.state = 13;
        status->backup = status->base;
        return 1;
      }
      continue;
    case 13:
      stack[++stackPtr] = 14;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_VZOI_VZOR_SETTINGS))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 14:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_VZOIVZORDATA)
        continue;
      if (write_VzoiVzorData(
              status, (ifcomsoucp_VzoiVzorData *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        return 1;
      }
      continue;
    case 15:
      stack[++stackPtr] = 16;
      if (!ifcomsoucp_push(status, 15, -1))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 16:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_HEADER)
        continue;
      if (write_Header(status, (ifcomsoucp_Header *)status->base.structPtr)) {
        status->base.state = 17;
        status->backup = status->base;
        return 1;
      }
      continue;
    case 17:
      stack[++stackPtr] = 18;
      if (!(status->base.Header_messageType == IFCOMSOUCP_MESSAGETYPE_CONTROL))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 18:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_CONTROL)
        continue;
      if (write_Control(status, (ifcomsoucp_Control *)status->base.structPtr)) {
        status->base.state = 19;
        status->backup = status->base;
        return 1;
      }
      continue;
    case 19:
      stack[++stackPtr] = 20;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_POINT))
        stack[stackPtr] = 21;
      else
        ifcomsoucp_push(status, 21, 2);
      continue;
    case 20:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_CRPLAN_POINT)
        continue;
      if (write_CRPlan_Point(
              status, (ifcomsoucp_CRPlan_Point *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        return 1;
      }
      continue;
    case 21:
      stack[++stackPtr] = 22;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_OBJECT))
        stack[stackPtr] = 23;
      else
        ifcomsoucp_push(status, 23, 2);
      continue;
    case 22:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_CRPLAN_OBJECT)
        continue;
      if (write_CRPlan_Object(
              status, (ifcomsoucp_CRPlan_Object *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        return 1;
      }
      continue;
    case 23:
      stack[++stackPtr] = 24;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_DESTROY_OBJECT))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 24:
      if (status->base.structID != IFCOMSOUCP_STRUCT_ID_DESTROYOBJECT)
        continue;
      if (write_DestroyObject(
              status, (ifcomsoucp_DestroyObject *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        return 1;
      }
      continue;
    default:
      printf("ifcomsoucp Protocol ERROR: Unknown state\n");
    }
  return 0;
}

int ifcomsoucp_readStruct(ifcomsoucp_Status *status) {
  int stack[10];
  stack[0] = status->base.state;
  int stackPtr = 0;
  while (stackPtr != -1)
    switch (stack[stackPtr--]) {
    case 1:
      stack[++stackPtr] = 2;
      if (!ifcomsoucp_push(status, 1, -1))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 2:
      if (read_Header(status, (ifcomsoucp_Header *)status->base.structPtr)) {
        status->base.state = 3;
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_HEADER;
        return 1;
      }
      continue;
    case 3:
      stack[++stackPtr] = 4;
      if (!(status->base.Header_messageType ==
            IFCOMSOUCP_MESSAGETYPE_OBJECT_INFO))
        stack[stackPtr] = 11;
      else
        ifcomsoucp_push(status, 11, 2);
      continue;
    case 4:
      if (read_ObjectInfo(status,
                          (ifcomsoucp_ObjectInfo *)status->base.structPtr)) {
        status->base.state = 5;
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_OBJECTINFO;
        return 1;
      }
      continue;
    case 5:
      stack[++stackPtr] = 6;
      if (!(status->base.ObjectInfo_typeExtData == IFCOMSOUCP_TYPEEXTDATA_RES))
        stack[stackPtr] = 8;
      else
        ifcomsoucp_push(status, 8, 2);
      continue;
    case 6:
      stack[++stackPtr] = 7;
      if (!ifcomsoucp_push(status, 6, status->base.ObjectInfo_countExtData))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 7:
      if (read_RES(status, (ifcomsoucp_RES *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_RES;
        return 1;
      }
      continue;
    case 8:
      stack[++stackPtr] = 9;
      if (!(status->base.ObjectInfo_typeExtData ==
            IFCOMSOUCP_TYPEEXTDATA_METEO))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 9:
      stack[++stackPtr] = 10;
      if (!ifcomsoucp_push(status, 9, status->base.ObjectInfo_countExtData))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 10:
      if (read_Meteo(status, (ifcomsoucp_Meteo *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_METEO;
        return 1;
      }
      continue;
    case 11:
      stack[++stackPtr] = 12;
      if (!(status->base.Header_messageType == IFCOMSOUCP_MESSAGETYPE_CONTROL))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 12:
      if (read_Control(status, (ifcomsoucp_Control *)status->base.structPtr)) {
        status->base.state = 13;
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_CONTROL;
        return 1;
      }
      continue;
    case 13:
      stack[++stackPtr] = 14;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_VZOI_VZOR_SETTINGS))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 14:
      if (read_VzoiVzorData(
              status, (ifcomsoucp_VzoiVzorData *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_VZOIVZORDATA;
        return 1;
      }
      continue;
    case 15:
      stack[++stackPtr] = 16;
      if (!ifcomsoucp_push(status, 15, -1))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 16:
      if (read_Header(status, (ifcomsoucp_Header *)status->base.structPtr)) {
        status->base.state = 17;
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_HEADER;
        return 1;
      }
      continue;
    case 17:
      stack[++stackPtr] = 18;
      if (!(status->base.Header_messageType == IFCOMSOUCP_MESSAGETYPE_CONTROL))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 18:
      if (read_Control(status, (ifcomsoucp_Control *)status->base.structPtr)) {
        status->base.state = 19;
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_CONTROL;
        return 1;
      }
      continue;
    case 19:
      stack[++stackPtr] = 20;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_POINT))
        stack[stackPtr] = 21;
      else
        ifcomsoucp_push(status, 21, 2);
      continue;
    case 20:
      if (read_CRPlan_Point(
              status, (ifcomsoucp_CRPlan_Point *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_CRPLAN_POINT;
        return 1;
      }
      continue;
    case 21:
      stack[++stackPtr] = 22;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_OBJECT))
        stack[stackPtr] = 23;
      else
        ifcomsoucp_push(status, 23, 2);
      continue;
    case 22:
      if (read_CRPlan_Object(
              status, (ifcomsoucp_CRPlan_Object *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_CRPLAN_OBJECT;
        return 1;
      }
      continue;
    case 23:
      stack[++stackPtr] = 24;
      if (!(status->base.Control_type ==
            IFCOMSOUCP_CONTROLMESSAGETYPE_DESTROY_OBJECT))
        stack[stackPtr] = ifcomsoucp_pop(status);
      continue;
    case 24:
      if (read_DestroyObject(
              status, (ifcomsoucp_DestroyObject *)status->base.structPtr)) {
        status->base.state = ifcomsoucp_pop(status);
        status->backup = status->base;
        status->base.structID = IFCOMSOUCP_STRUCT_ID_DESTROYOBJECT;
        return 1;
      }
      continue;
    default:
      printf("ifcomsoucp Protocol ERROR: Unknown state\n");
    }
  return 0;
}

char *ifcomsoucp_getStructNameByID(ifcomsoucp_StructID id) {
  switch (id) {
  case IFCOMSOUCP_STRUCT_ID_TIME:
    return "Time";
  case IFCOMSOUCP_STRUCT_ID_HEADER:
    return "Header";
  case IFCOMSOUCP_STRUCT_ID_OBJECTINFO:
    return "ObjectInfo";
  case IFCOMSOUCP_STRUCT_ID_RES:
    return "RES";
  case IFCOMSOUCP_STRUCT_ID_METEO:
    return "Meteo";
  case IFCOMSOUCP_STRUCT_ID_CONTROL:
    return "Control";
  case IFCOMSOUCP_STRUCT_ID_VZOIVZORDATA:
    return "VzoiVzorData";
  case IFCOMSOUCP_STRUCT_ID_CRPLAN_POINT:
    return "CRPlan_Point";
  case IFCOMSOUCP_STRUCT_ID_CRPLAN_OBJECT:
    return "CRPlan_Object";
  case IFCOMSOUCP_STRUCT_ID_DESTROYOBJECT:
    return "DestroyObject";
  }
  return "no such structure";
}
