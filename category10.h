#ifndef _CATEGORY10_H_
#define _CATEGORY10_H_
#include <stdint.h>

#define RestoreFactoryID    (0x10)
#define SoftwareVersionID (0x21)
#define SystemNameID        (0x30)
#define Configuration_00ID  (0x40)
#define Configuration_01ID  (0x41)
#define Configuration_02ID  (0x42)
#define Configuration_03ID  (0x43)
#define Configuration_04ID  (0x44)
#define Configuration_05ID  (0x45)
#define Configuration_06ID  (0x46)
#define Configuration_07ID  (0x47)
#define Configuration_08ID  (0x48)
#define Configuration_09ID  (0x49)
#define Configuration_10ID  (0x4a)
#define Configuration_11ID  (0x4b)
#define Configuration_12ID  (0x4c)
#define Configuration_13ID  (0x4d)
#define Configuration_14ID  (0x4e)
#define Configuration_15ID  (0x4f)
#define Configuration_16ID  (0x50)
#define Configuration_17ID  (0x51)
#define Configuration_18ID  (0x52)
#define Configuration_19ID  (0x53)
#define Configuration_20ID  (0x54)
#define Configuration_21ID  (0x55)
#define Configuration_22ID  (0x56)
#define Configuration_23ID  (0x57)
#define Configuration_24ID  (0x58)
#define Configuration_25ID  (0x59)
#define Configuration_26ID  (0x5a)
#define Configuration_27ID  (0x5b)
#define Configuration_28ID  (0x5c)
#define Configuration_29ID  (0x5d)
#define Configuration_30ID  (0x5e)
#define Configuration_31ID  (0x5f)
#define ConfigRestoreID     (0x60)
#define NetworkingModeID    (0x81)
#define FullNetModeTypeID   (0x82)
#define NetTEnableID        (0x83)

typedef struct
{
   uint8_t  restoreDefaults;
   uint32_t swVersion;
   char     configStr[32][128];
   uint8_t  configRestore;
   uint8_t  networkingMode;
   uint8_t  fullNetworkingModeType;
   uint8_t  netTenable;
} ConfigType;

#endif // _CATEGORY10_H_
