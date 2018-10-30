#ifndef _CATEGORY80_H_
#define _CATEGORY80_H_

#define CommandMulticastIPAddressEth1   (0x13)
#define CommandPortEth1                 (0x14)
#define CommandMulticastIPAddressEth2   (0x23)
#define CommandPortEth2                 (0x24)

typedef struct
{
   uint32_t cmdMulticastIpAddrEth1;
   uint16_t cmdPortEth1;
   uint32_t cmdMulticastIpAddrEth2;
   uint16_t cmdPortEth2;
}CsiCommandType;

#endif // _CATEGORY80_H_
