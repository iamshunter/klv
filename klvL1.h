#ifndef _KLV_L1_H
#define _KLV_L1_H
#include "category10.h"
#include "category20.h"
#include "category30.h"
#include "category38.h"
#include "category40.h"
#include "category61.h"
#include "category64.h"
#include "category67.h"
#include "category80.h"
#include "category88.h"


static const unsigned char UniversalKey[]         ={ 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00};
static const unsigned char UniversalKeyExternal[] ={ 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x4C, 0x33, 0x44, 0x45, 0x56, 0x01, 0x00};

// Routine to encode BER length.  The len is input as a long, the BER encoded length is stored to the output buffer
// 0-127 in first byte means that's the lengh
// If the MSbit is set, then the rest of the byte is a byte-count for the length, 0x10000010 would mean the next 2 bytes
// are the real length.
int  encodeBER(int len, void *out, int outLen);
void decodeBER(void *in, unsigned int *nBytesMsg, unsigned int *nBytesLen);

// Base keys
static const unsigned char Checksum      = 0xC0;
static const unsigned char Timestamp     = 0xC1;
static const unsigned char Type          = 0xC2;

// Level1 Keys
static const unsigned char Configuration = 0x10;
static const unsigned char Interfaces    = 0x20;
static const unsigned char Transmitter   = 0x30;
static const unsigned char Receiver      = 0x38;
static const unsigned char MediaEncoder  = 0x40;
static const unsigned char MediaDecoder  = 0x48;
static const unsigned char Routing       = 0x50;
static const unsigned char PacketMux     = 0x58;
static const unsigned char PowerOnBIT    = 0x61;
static const unsigned char ContinuousBIT = 0x64;
static const unsigned char InitiatedBIT  = 0x67;
static const unsigned char Encryption    = 0x78;
static const unsigned char CSICommand    = 0x80;
static const unsigned char CSIStatus     = 0x88;
static const unsigned char KLVTelemetry  = 0xA0;
static const unsigned char NavData       = 0xA8;

bool procKLV(void *bfr, int bfrSize);

#endif // _KLV_L1_H
