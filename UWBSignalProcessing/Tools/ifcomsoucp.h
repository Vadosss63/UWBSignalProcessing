#ifndef __IFCOMSOUCP__
#define __IFCOMSOUCP__

// Protocol: ifcomsoucp, version: 1.0

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  IFCOMSOUCP_STRUCT_ID_TIME = 0,
  IFCOMSOUCP_STRUCT_ID_HEADER = 1,
  IFCOMSOUCP_STRUCT_ID_OBJECTINFO = 2,
  IFCOMSOUCP_STRUCT_ID_RES = 3,
  IFCOMSOUCP_STRUCT_ID_METEO = 4,
  IFCOMSOUCP_STRUCT_ID_CONTROL = 5,
  IFCOMSOUCP_STRUCT_ID_VZOIVZORDATA = 6,
  IFCOMSOUCP_STRUCT_ID_CRPLAN_POINT = 7,
  IFCOMSOUCP_STRUCT_ID_CRPLAN_OBJECT = 8,
  IFCOMSOUCP_STRUCT_ID_DESTROYOBJECT = 9
} ifcomsoucp_StructID;

typedef enum {
  IFCOMSOUCP_FROM_IFCO_MODE = 0,
  IFCOMSOUCP_TO_IFCO_MODE = 1
} ifcomsoucp_Mode;

typedef enum {
  IFCOMSOUCP_MESSAGETYPE_CONTROL = 1,
  IFCOMSOUCP_MESSAGETYPE_OBJECT_INFO = 2
} ifcomsoucp_MessageType;

typedef enum {
  IFCOMSOUCP_SOURCENUMBER_IFCO = 14,
  IFCOMSOUCP_SOURCENUMBER_IM_ARLM_X = 19,
  IFCOMSOUCP_SOURCENUMBER_IM_ARLM_S = 13,
  IFCOMSOUCP_SOURCENUMBER_IM_PRLM = 17,
  IFCOMSOUCP_SOURCENUMBER_IM_MOEP = 16,
  IFCOMSOUCP_SOURCENUMBER_IM_ARLM_35 = 23,
  IFCOMSOUCP_SOURCENUMBER_IM_SHP = 10,
  IFCOMSOUCP_SOURCENUMBER_IM_ARLM_GO = 18,
  IFCOMSOUCP_SOURCENUMBER_IM_RLM = 11,
  IFCOMSOUCP_SOURCENUMBER_IM_ARLK_X = 12,
  IFCOMSOUCP_SOURCENUMBER_IM_CIFAR = 27,
  IFCOMSOUCP_SOURCENUMBER_IM_OEM = 15,
  IFCOMSOUCP_SOURCENUMBER_IM_AK = 28,
  IFCOMSOUCP_SOURCENUMBER_IM_KPAS_PRCU = 20,
  IFCOMSOUCP_SOURCENUMBER_IM_VZOI_VZOR = 21,
  IFCOMSOUCP_SOURCENUMBER_IM_AKNG = 22,
  IFCOMSOUCP_SOURCENUMBER_IM_PVO = 25,
  IFCOMSOUCP_SOURCENUMBER_IM_URO = 26,
  IFCOMSOUCP_SOURCENUMBER_IM_NPRO = 29,
  IFCOMSOUCP_SOURCENUMBER_IM_SEV = 30,
  IFCOMSOUCP_SOURCENUMBER_IM_ASUDB = 24
} ifcomsoucp_SourceNumber;

typedef enum {
  IFCOMSOUCP_OBJECTCLASS_LA = 1,
  IFCOMSOUCP_OBJECTCLASS_ROCKET = 2,
  IFCOMSOUCP_OBJECTCLASS_SUBMARINE = 3,
  IFCOMSOUCP_OBJECTCLASS_SHIP = 4,
  IFCOMSOUCP_OBJECTCLASS_METEO = 5,
  IFCOMSOUCP_OBJECTCLASS_NOISE_SOURCE = 6,
  IFCOMSOUCP_OBJECTCLASS_NOISE_OBJECT = 7,
  IFCOMSOUCP_OBJECTCLASS_STATIC_OBJECT = 8
} ifcomsoucp_ObjectClass;

typedef enum {
  IFCOMSOUCP_OBJECTTYPE_LA_BOMBER = 1,
  IFCOMSOUCP_OBJECTTYPE_LA_FIGHTER = 2,
  IFCOMSOUCP_OBJECTTYPE_LA_HELICOPTER = 3
} ifcomsoucp_ObjectType_LA;

typedef enum {
  IFCOMSOUCP_OBJECTTYPE_ROCKET_PRK = 1,
  IFCOMSOUCP_OBJECTTYPE_ROCKET_PRR = 2,
  IFCOMSOUCP_OBJECTTYPE_ROCKET_BR = 3
} ifcomsoucp_ObjectType_Rocket;

typedef enum {
  IFCOMSOUCP_OBJECTTYPE_SHIP_NK = 1,
  IFCOMSOUCP_OBJECTTYPE_SHIP_BEK = 2
} ifcomsoucp_ObjectType_Ship;

typedef enum {
  IFCOMSOUCP_OBJECTTYPE_SUBMARINE_PL = 1,
  IFCOMSOUCP_OBJECTTYPE_SUBMARINE_NPA = 2
} ifcomsoucp_ObjectType_Submarine;

typedef enum {
  IFCOMSOUCP_OBJECTTYPE_METEO_CLOUD = 1,
  IFCOMSOUCP_OBJECTTYPE_METEO_FOG = 2,
  IFCOMSOUCP_OBJECTTYPE_METEO_RAIN = 3,
  IFCOMSOUCP_OBJECTTYPE_METEO_SNOW = 4
} ifcomsoucp_ObjectType_Meteo;

typedef enum {
  IFCOMSOUCP_TYPEEXTDATA_NO_DATA = 0,
  IFCOMSOUCP_TYPEEXTDATA_RES = 1,
  IFCOMSOUCP_TYPEEXTDATA_METEO = 2
} ifcomsoucp_TypeExtData;

typedef enum {
  IFCOMSOUCP_RES_TYPE_SHOOTING_RES = 1,
  IFCOMSOUCP_RES_TYPE_OES = 2,
  IFCOMSOUCP_RES_TYPE_GAS = 3,
  IFCOMSOUCP_RES_TYPE_DATA_TRANSFER_NET = 4,
  IFCOMSOUCP_RES_TYPE_NAVIGATION_RLS = 5,
  IFCOMSOUCP_RES_TYPE_NOISE_SOURCE = 6,
  IFCOMSOUCP_RES_TYPE_MULTIFUNCTIONAL_RLS = 7,
  IFCOMSOUCP_RES_TYPE_LAZER_TARGETING = 8
} ifcomsoucp_RES_Type;

typedef enum {
  IFCOMSOUCP_CONTROLMESSAGETYPE_SET_TIME = 1,
  IFCOMSOUCP_CONTROLMESSAGETYPE_START = 2,
  IFCOMSOUCP_CONTROLMESSAGETYPE_STOP = 3,
  IFCOMSOUCP_CONTROLMESSAGETYPE_RESET = 4,
  IFCOMSOUCP_CONTROLMESSAGETYPE_VZOI_VZOR_SETTINGS = 5,
  IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_POINT = 6,
  IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_OBJECT = 7,
  IFCOMSOUCP_CONTROLMESSAGETYPE_CR_PLAN_OEP = 8,
  IFCOMSOUCP_CONTROLMESSAGETYPE_DESTROY_OBJECT = 9,
  IFCOMSOUCP_CONTROLMESSAGETYPE_TARGET_ENVIROPMENT_SEND = 10
} ifcomsoucp_ControlMessageType;

typedef struct {
  uint32_t sec;
  uint32_t msec;
} ifcomsoucp_Time;

typedef struct {
  uint16_t messageType;
  uint8_t packageCount;
  uint8_t packageFragmentID;
  uint32_t messageCyclicNumber;
  ifcomsoucp_Time time;
  uint8_t sndrID;
  uint8_t recvID;
  uint16_t CRC;
  uint16_t words32Count;
} ifcomsoucp_Header;

typedef struct {
  uint32_t objectDBID;
  uint32_t objectID;
  float latitude;
  float longitude;
  float altitude;
  float velocityX;
  float velocityY;
  float velocityZ;
  float course;
  float tangage;
  float roll;
  uint8_t objectClass;
  uint8_t objectType;
  uint8_t typeExtData;
  uint16_t countExtData;
} ifcomsoucp_ObjectInfo;

typedef struct {
  uint8_t type;
  uint32_t numberInDB;
  float frequency;
} ifcomsoucp_RES;

typedef struct {
  float A11;
  float A12;
  float A13;
  float A14;
  float A21;
  float A22;
  float A23;
  float A24;
  float A31;
  float A32;
  float A33;
  float A34;
  float A41;
  float A42;
  float A43;
  float A44;
  float B11;
  float B12;
  float B21;
  float B22;
  float B31;
  float B32;
  float INT;
} ifcomsoucp_Meteo;

typedef struct {
  uint8_t type;
  uint32_t selfID;
} ifcomsoucp_Control;

typedef struct {
  uint16_t distance;
  uint16_t latency;
  uint32_t groupID;
} ifcomsoucp_VzoiVzorData;

typedef struct {
  float latitude;
  float longitude;
  float altitude;
} ifcomsoucp_CRPlan_Point;

typedef struct {
  uint32_t targetID;
} ifcomsoucp_CRPlan_Object;

typedef struct {
  uint32_t ID;
} ifcomsoucp_DestroyObject;

typedef struct {
  int id;
  int count;
} ifcomsoucp_StackItem;

typedef struct {
  uint8_t *data;
  size_t dataLength;
  size_t dataPtr;
  size_t bitPtr;
} ifcomsoucp_DataStream;

typedef struct {
  char *name;
  char *version;
  struct {
    ifcomsoucp_DataStream data;
    int state;
    uint8_t structPtr[92];
    ifcomsoucp_StructID structID;
    ifcomsoucp_StackItem stack[10];
    int stackPtr;
    int Control_type;
    int ObjectInfo_countExtData;
    int Header_messageType;
    int ObjectInfo_typeExtData;
    size_t Header_CRC_dataPtr;
    size_t Header_CRC_bitPtr;
    size_t Header_words32Count_dataPtr;
    size_t Header_words32Count_bitPtr;
  } base, backup;
} ifcomsoucp_Status;

void ifcomsoucp_init(ifcomsoucp_Status *status, uint8_t *data,
                     size_t dataLength, ifcomsoucp_Mode mode);
void ifcomsoucp_reset(ifcomsoucp_Status *status, uint8_t *data,
                      size_t dataLength, ifcomsoucp_Mode mode);
void ifcomsoucp_resetDataStream(ifcomsoucp_Status *status, uint8_t *data,
                                size_t dataLength);
int ifcomsoucp_updateHeader_CRC(ifcomsoucp_Status *status, uint16_t value);
int ifcomsoucp_updateHeader_words32Count(ifcomsoucp_Status *status,
                                         uint16_t value);
void ifcomsoucp_errorRecovery(ifcomsoucp_Status *status);
size_t ifcomsoucp_getDataLength(ifcomsoucp_Status *status);
ifcomsoucp_Time *ifcomsoucp_getTime(ifcomsoucp_Status *status);
ifcomsoucp_Header *ifcomsoucp_getHeader(ifcomsoucp_Status *status);
ifcomsoucp_ObjectInfo *ifcomsoucp_getObjectInfo(ifcomsoucp_Status *status);
ifcomsoucp_RES *ifcomsoucp_getRES(ifcomsoucp_Status *status);
ifcomsoucp_Meteo *ifcomsoucp_getMeteo(ifcomsoucp_Status *status);
ifcomsoucp_Control *ifcomsoucp_getControl(ifcomsoucp_Status *status);
ifcomsoucp_VzoiVzorData *ifcomsoucp_getVzoiVzorData(ifcomsoucp_Status *status);
ifcomsoucp_CRPlan_Point *ifcomsoucp_getCRPlan_Point(ifcomsoucp_Status *status);
ifcomsoucp_CRPlan_Object *
ifcomsoucp_getCRPlan_Object(ifcomsoucp_Status *status);
ifcomsoucp_DestroyObject *
ifcomsoucp_getDestroyObject(ifcomsoucp_Status *status);
void ifcomsoucp_setStructID(ifcomsoucp_Status *status,
                            ifcomsoucp_StructID structID);
ifcomsoucp_StructID ifcomsoucp_getStructID(ifcomsoucp_Status *status);
int ifcomsoucp_writeStruct(ifcomsoucp_Status *status);
int ifcomsoucp_readStruct(ifcomsoucp_Status *status);
char *ifcomsoucp_getStructNameByID(ifcomsoucp_StructID id);

#ifdef __cplusplus
}
#endif

#endif // __IFCOMSOUCP__
