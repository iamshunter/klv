#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <zlib.h>
#include "klvL1.h"
#include "l3Sum.h"

#include <stdio.h>

void crc16Test(void)
{
    char buffer[] = "123456789";
    printf("crc16-3 = %x\n", crc16L3(buffer, 9, 0));
    printf("crc16-2 = %x\n", crc16L3(buffer, 9));
}

double getDoubleTime(void)
{
    struct timeb tval;
    ftime(&tval);
    return (tval.time + tval.millitm / 1000.0);
}

int main(int argc, char **argv)
{
    const char buf1[] = { 'p', 'r', 'a', 't', 'i', 'k' };
    int len = 37;
    unsigned char buff[] = {
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,

        0x18, 0xC1, 0x08, 0x00, 0x04, 0x42, 0xA7, 0xE6,
        0x71, 0x50, 0x12, 0x30, 0x06, 0x10, 0x04, 0x00,
        0x06, 0x90, 0x78, 0xC0, 0x04
    };

    printf("size %2d", sizeof(UniversalKey));
    for (int ii = 0; ii < sizeof(UniversalKey); ii++)
    {
        printf(" %02X", UniversalKey[ii]);
    }
    printf("\n");
    printf("size %2d", sizeof(UniversalKeyExternal));
    for (int ii = 0; ii < sizeof(UniversalKeyExternal); ii++)
    {
        printf(" %02X", UniversalKeyExternal[ii]);
    }
    printf("\n");

    /* CRC = 39 5C 6F AB */
    printf("zlib pratik %x\n", crc32(0x80000000, (const unsigned char *)buf1, sizeof(buf1)));
    printf("zlib        %x\n\n", crc32(0x00000000, (const unsigned char *)buf1, sizeof(buf1)));

    printf("zlib        %x\n", crc32(0x00000000, (const unsigned char *)buff, sizeof(buff)));
    printf("l3 Sum      %x\n", crc32L3((char *)buff, sizeof(buff), 0xFFFFFFFF));
    printf("l3 Sum      %x\n", crc32L3((char *)buff, sizeof(buff)));

    printf("CRC = 39 5C 6F AB\n");

    unsigned int sum;
    unsigned short sum2;
    int iter = 1024 * 1024, ii;
    double t1, t2;
    t1 = getDoubleTime();
    for (ii = 0; ii < iter; ii++)
    {
        sum = crc32(0x00000000, (const unsigned char *)buff, sizeof(buff));
    }
    t2 = getDoubleTime();
    printf("%d iter, zlib   %.4f %x\n", ii, (t2 - t1), sum);

    t1 = getDoubleTime();
    for (ii = 0; ii < iter; ii++)
    {
        sum = crc32L3((char *)buff, sizeof(buff));
    }
    t2 = getDoubleTime();
    printf("%d iter, local2 %.4f %x\n", ii, (t2 - t1), sum);

    t1 = getDoubleTime();
    for (ii = 0; ii < iter; ii++)
    {
        sum = crc32L3((char *)buff, sizeof(buff), 0xFFFFFFFF);
    }
    t2 = getDoubleTime();
    printf("%d iter, local3 %.4f %x\n", ii, (t2 - t1), sum);

    t1 = getDoubleTime();
    for (ii = 0; ii < iter; ii++)
    {
        sum2 = crc16L3((char *)buff, sizeof(buff));
    }
    t2 = getDoubleTime();
    printf("%d iter, crc162 %.4f %x\n", ii, (t2 - t1), sum2);

    t1 = getDoubleTime();
    for (ii = 0; ii < iter; ii++)
    {
        sum2 = crc16L3((char *)buff, sizeof(buff), 0);
    }
    t2 = getDoubleTime();
    printf("%d iter, crc163 %.4f %x\n", ii, (t2 - t1), sum2);

    crc16Test();

    // Test the BER encode
    unsigned int out;
    unsigned int nMsg;
    unsigned int nLen;
    int ans;

    len = 64;
    ans = encodeBER(len, &out, sizeof(out));
    printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    printf("unBER  %5d %d\n", nMsg, nLen);

    len = 127;
    ans = encodeBER(len, &out, sizeof(out));
    printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    printf("unBER  %5d %d\n", nMsg, nLen);

    len = 128;
    ans = encodeBER(len, &out, sizeof(out));
    printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    printf("unBER  %5d %d\n", nMsg, nLen);

    len = 32760;
    ans = encodeBER(len, &out, sizeof(out));
    printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    printf("unBER  %5d %d\n", nMsg, nLen);

    unsigned char msg[] = { 
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,  // Universal Key
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,  // Universal Key
        0x18,                                            // Len=24 bytes

// Level 2 Value
        0xC1,                                 // Key - Category (0xc1=time)
        0x08,                                 // Len=8 bytes
        0x00, 0x04, 0x42, 0xA7,               // Value bytes 0-3
        0xE6, 0x71, 0x50, 0x12,               // Value bytes 4-7

        0x30,                                 // Key - Category (0x30=transmit characteristics)
        0x06,                                 // Len=6 bytes

            0x10,                             // Key - 0x10 = Frequency Value 
            0x04,                             // Len=4 bytes
            0x00, 0x06, 0x90, 0x78,           // Value bytes 0-3 - kHz
        
        0xC0,                                 // Key for crc
        0x04,                                 // Len=4 bytes
        0x39, 0x5C, 0x6F, 0xAB                // Value bytes 0-3, CRC
    };

    // Time 1/1/2008
    // Category 0x30
    // Command 0x10
    // Value 0x069078
/*
    if (memcmp(msg, UniversalKey, sizeof(UniversalKey)) == 0)
    {
        printf("Found Universal Key\n");
        decodeBER(&msg[sizeof(UniversalKey)], &nMsg, &nLen);
        printf("unBER  %5d %d\n", nMsg, nLen);
    } 
    else if (memcmp(msg, UniversalKeyExternal, sizeof(UniversalKey)) == 0)
    {
        printf("Found Universal Key External\n");
    }

*/

    bool stat = procLevel1(msg, sizeof(msg));
    printf("procLevel1 returned ");
    if ( stat )
    {
        printf(" TRUE\n");
    }
    else
    {
        printf(" FALSE\n");
    }
}
