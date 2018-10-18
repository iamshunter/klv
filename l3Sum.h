#ifndef _L3_SUM
#define _L3_SUM

unsigned short crc16L3(const void *buffer, int nbytes );
unsigned short crc16L3(const void *buffer, int nbytes, unsigned short crc);

unsigned long  crc32L3(const void *buffer, int nbytes);
unsigned long  crc32L3(const void *buffer, int nbytes, unsigned int crcIn );

#endif // _L3_SUM

