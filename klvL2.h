#ifndef _KLV_L2_H
#define _KLV_L2_H

// 600731_wor_gh602m44jkff2
//
//  Keys for Configuration key 0x10
//  --------------------------------
// Restore Factory Defaults  0x10 1 uint8   cmd       1 – Restore Factory Defaults
// Software Version ID       0x21 4 uint32  sts       SW Version ID 
//                                                    1-byte major, 1-byte minor, 2-byte build: v1.2.3 = 0x01020003
// System Name               0x30 V char[]  cmd/sts   character text
//                                                    Maximum of 127 Bytes
//                                                    Default is “CMDL”
// Configuration 0           0x40 V char[]  cmd/sts   character text
// Configuration 1           0x41                     Maximum of 127 Bytes
//                            .. .
// Configuration 31          0x5F
// Configuration Restore     0x60 1 uint8  cmd        Value of Configuration to restore
//                                                    (I.e. 0x1 for Configuration 1)
// Networking Mode           0x81 1 uint8  cmd/sts    0 - Contained Networking Mode (Default)
//                                                    1 - Full Networking Mode
// Full Networking Mode Type 0x82 1 uint8  cmd/sts    Exclusive mode type for a link - both ends of a link
//                                                    should be configured to a different mode type to
//                                                    support full networking mode. Valid values are:
//                                                    0 - Default A (Default)
//                                                    1 - Default B
//                                                    2 - Custom
// Net-T Enable              0x83 1 uint8  cmd/sts    0 - off
//                                                    1 - HUB mode
//                                                    2 - reserved
//

// Interfaces    0x20
// Ethernet 1 – IP Address      0x13 4 uint32 cmd/sts    4 byte IP address in network byte order
//                                                       Defaults to 192.168.51.45
// Ethernet 1 – Netmask         0x15 4 uint32 cmd/sts    4-bytes Netmask in network byte order
//                                                       Defaults to 255.255.255.0
// Ethernet 1 – MAC Address     0x16 6 uint48 sts        6 Byte MAC address
// Ethernet 1 – DHCP/DNS Server 0x17 1  uint8 cmd/sts    0 – Disabled (Default)
//                                                       1 – Enabled
// Ethernet 2 - IP Address      0x23 4 byte[] cmd/sts    4 byte IP address in network byte order
//                                                       Defaults to 192.168.80.1 
// Ethernet 2 - Netmask         0x25 4 byte[] Cmd/sts    4-bytes Netmask in network byte order
//                                                       Defaults to 255.255.255.0
// Ethernet 2 - MAC Address     0x26 6 uint48 sts        6 Byte MAC address
// Ethernet 2 – DHCP/DNS Server 0x27 1  uint8 cmd/sts    0 – Disabled
// IP Network – Networking Mode 0x19 1  uint8 sts        ??????
//                                                       2 - Layer 3 (Routing) (Default)
// IP Network - Dynamic Routing 0x1A 1  uint8 cmd/sts    0 - Dynamic Routing Disabled
//                                                       1 - RIP Enabled (Default)
//                                                       2 - OSPF Enabled
//                                                       3 - RIP and OSPF Enabled
//                                                       4 - PIM-SM Enabled
//                                                       5 - RIP and PIM-SM Enabled
//                                                       6 - OSPF and PIM-SM Enabled
//                                                       7 - RIP, OSPF, and PIM-SM Enabled

#ifdef XXX
IP Network-Data Link IP Addr 0x33 4 byte[] cmd/sts 4 byte IP address in network byte order
                                                          Defaults to 192.168.70.1

IP Network-Data Link Netmask 0x35 4 byte[] cmd/sts 4 byte IP address in network byte order
                                                          Defaults to 255.255.255.0

IP Network-DHCP Server on Link interface 0x37 1 uint8 cmd/sts 4 byte IP address in network byte order
                                                          0 – Disabled (Default)
                                                          1 – Enabled


Routing 6 uint48 Cmd/sts 
0 – Disabled (Default)
1 – Enabled
cmd/sts 4 byte IP address in network byte order
Defaults to 192.168.80.1
cmd/sts 4 byte IP address in network byte order
sts
6 Byte MAC address
1 uint8 cmd/sts
1 uint8 sts
1 uint8 
4 byte[] 1 uint8
IP Network – Data Link 0x35
Netmask
IP Network -- DHCP
0x37
Server on Link interface
6 Byte MAC address
0 - Disabled
1 – Enabled (Default)
2 - Layer 3 (Routing) (Default)
cmd/sts 4 byte IP address in network byte order
Defaults to 255.255.255.0
Cmd/sts 0 – Disabled (Default)
1 – Enabled

#endif // XXX




// Transmitter   0x30 *****

// Receiver      0x38

// MediaEncoder  0x40

// MediaDecoder  0x48

// Routing       0x50

// PacketMux     0x58

// PowerOnBIT    0x61

// ContinuousBIT 0x64

// InitiatedBIT  0x67

// Encryption    0x78

// CSICommand    0x80

// CSIStatus     0x88

// KLVTelemetry  0xA0

// NavData       0xA8


#endif // _KLV_L2_H
