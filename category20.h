#ifndef _CATEGORY20_H_
#define _CATEGORY20_H_

// Interfaces    0x20
#define Eth1IPAddressID     (0x13)
#define Eth1NetmaskID       (0x15)
#define Eth1MACAddressID    (0x16)
#define Eth1DHCP_DnsServID  (0x17)
#define Eth2IPAddressID     (0x23)
#define Eth2NetmaskID       (0x25)
#define Eth2MACAddressID    (0x26)
#define Eth2DHCP_DNS ServID (0x27)
#define IpNetNetwModeID     (0x19)
#define IpNetDynRoutingID   (0x1A)

typedef struct
{
   uint32_t eth1Addr;
   uint32_t eth1Netmask;
   uint64_t eth1MAC;
   uint8_t  eth1DHCP_DNS;
   uint32_t eth2Addr;
   uint32_t eth2Netmask;
   uint64_t eth2MAC;
   uint8_t  eth2DHCP_DNS;
   uint8_t  networkingMode;
   uint8_t  dynamicRouting;
   char     dataLinkAddr[4];
   char     dataLinkNetmask[4];
   uint8_t  DHCP_ServerOnLinkInterface;
} InterfacesType;

#endif // _CATEGORY20_H_
