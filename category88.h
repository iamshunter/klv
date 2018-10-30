#ifndef _CATEGORY88_H_
#define _CATEGORY88_H_

#define StatusMulticastIPAddressEth1   (0x13)
#define StatusPortEth1                 (0x14)
#define StatusMulticastIPAddressEth2   (0x23)
#define StatusPortEth2                 (0x24)

typedef struct
{
   uint32_t statMulticastIpAddrEth1;
   uint16_t statPortEth1;
   uint32_t statMulticastIpAddrEth2;
   uint16_t statPortEth2;
}CsiStatusType;

#endif // _CATEGORY88_H_
