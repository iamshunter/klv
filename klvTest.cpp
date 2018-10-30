#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <zlib.h>
#include <time.h>
#include <map>
#include "klvL1.h"
#include "l3Sum.h"
#include "util.h"

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

// Master table stores tables for the each page
std::map<char, void *> categories;

//
// There's a lot going on in this file.
//
// First is an exploration of getting the right CRC for use with the L3 KLV interface
//   I used an example from stack overflow [https://stackoverflow.com/questions/40416995/difference-between-crc32-implementations-of-linux-crc32-h-and-zlib-h-in-c]
//   that uses the crc from zLib, the linux kernel and the code included in the L3 documents. 
//   Experimenting with that link I found the zlib implementation a little easier to deal with, 
//   and fastest execution.
//   
//   Install zLib with "sudo yum install zlib".
//
// Next there's a CRC-16 implemented but I don't recall why.
// 
// Then some tests for Basic Encoding Rules (BER). [See the comments in klvL1.cpp]
// 
// Finally we do some testing to decode and encode KLV message samples from the L3 documents.
//
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
    //printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    //printf("unBER  %5d %d\n", nMsg, nLen);

    len = 127;
    ans = encodeBER(len, &out, sizeof(out));
    //printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    //printf("unBER  %5d %d\n", nMsg, nLen);

    len = 128;
    ans = encodeBER(len, &out, sizeof(out));
    //printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    //printf("unBER  %5d %d\n", nMsg, nLen);

    len = 32760;
    ans = encodeBER(len, &out, sizeof(out));
    //printf("BER   %6d %d %04x\n", len, ans, out);
    decodeBER(&out, &nMsg, &nLen);
    //printf("unBER  %5d %d\n", nMsg, nLen);

    printf("***********************************************************************\n");
    printf("***********************************************************************\n");

    // This is the message defined in section c.1.1 in the CMDL manual.
    // "Single Command Example"
    unsigned char msg_1_1[] = { 
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,  // Universal Key
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,  // Universal Key
        0x18,                                            // Len=24 bytes

// Level 2 Value
            0xC1,                                        // Key - Category (0xc1=time)
            0x08,                                        // Len=8 bytes
            0x00, 0x04, 0x42, 0xA7,                      // Value bytes 0-3
            0xE6, 0x71, 0x50, 0x12,                      // Value bytes 4-7

            0x30,                                        // Key - Category (0x30=transmit characteristics)
            0x06,                                        // Len=6 bytes

                0x10,                                    // Key - 0x30->0x10 = Frequency Value 
                0x04,                                    // Len=4 bytes
                0x00, 0x06, 0x90, 0x78,                  // Value bytes 0-3 - kHz
        
            0xC0,                                        // Key for crc
            0x04,                                        // Len=4 bytes
            0x39, 0x5C, 0x6F, 0xAB                       // Value bytes 0-3, CRC
    };
    // Time 1/1/2008 00:00:00 (But I think that's not counting the TZ. I get 04:00 PDT)
    // Category 0x30
    // Command 0x10
    // Value 0x069078
    bool stat = procLevel1(msg_1_1, sizeof(msg_1_1));
    printf("procLevel1 returned ");
    if ( stat )
    {
        printf(" TRUE\n");
    }
    else
    {
        printf(" FALSE\n");
    }

    // This is the message defined in section c.1.2 in the CMDL manual.
    // "Multiple Command Example"
    unsigned char msg_1_2[] = { 
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,  // Universal Key
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,  // Universal Key
        0x2C,                                            // Len=44 bytes
        
            0xC1,                                            // Key - Category (0xC1=time)
            0x08,                                            // Len=8 bytes
                0x00, 0x04, 0x42, 0xA7,                      // Time value bytes 0-3
                0xE6, 0x71, 0x50, 0x12,                      // Time value bytes 4-7

            0x30,                                            // Key - Category (0x30=transmit characteristics)
            0x12,                                            // Len=18 bytes
        
                0x10,                                        // Key - 0x30->0x10 = Frequency Value
                0x04,                                        // Len=4 bytes
                    0x00, 0x06, 0x90, 0x78,                  // Value bytes 0-3
           
                0x20,                                        // Key - 0x30->0x20 = Data Rate
                0x04,                                        // Len=4 bytes
                    0x00, 0x00, 0x4E, 0x20,                  // Value bytes 0-3

                0x24,                                        // Key - 0x30->0x24 = Modulation
                0x01,                                        // Len=1 byte
                    0x00,                                    // Value byte
            
                0x80,                                        // Key - 0x30->0x80 = Tx Power
                0x01,                                        // Len=1 byte
                    0x80,                                    // Value byte

            0x40,                                            // Key - 0x40 = Media Encoder
            0x06,                                            // Len=6 bytes
                0x40,                                        // Key - 0x40->40 = Video Encoder Enable
                0x01,                                        // Len=1 bytes 
                    0x01,                                    // Value byte 
                0x41,                                        // Key - 0x40->41 = Video Encoder Type
                0x01,                                        // Len=1 bytes 
                    0x04,                                    // Value byte
                
            0xC0,                                            // Key - C0 checksum
            0x04,                                            // Len=4 bytes
                0x95, 0x60, 0x56, 0x1E                       // Value CRC
    };

    // This is the message defined in section c.1.3 in the CMDL manual.
    // "Send Status Request Example"
    unsigned char msg_1_3[] = { 
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,  // Universal Key
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,  // Universal Key
        0x20,                                            // Len=32 bytes

            0xC1,                                        // Key - Category (0xC1=time)
            0x08,                                        // Len=8 bytes
                0x00, 0x04, 0x42, 0xA7,                  // Time value bytes 0-3
                0xE6, 0x71, 0x50, 0x12,                  // Time value bytes 4-7
            
            
            0x30,                                        // Key - 0x30
            0x08,                                        // Len=8 bytes
                0x10, 0x00, 0x20, 0x00,                  // Value 0-3
                0x24, 0x00, 0x80, 0x00,                  // Value 4-7
            
            0x40,                                        // Key - 0x40
            0x04,                                        // Len=4 bytes
                0x40,                                    // Key - 0x40->0x40 (Video Encode Enable)
                0x00,                                    // Len=0, No data

                0x41,                                    // Key - 0x40->0x41 (Video Encode Type)
                0x00,                                    // Len=0, No data
                
            0xC0,                                        // Key - 0xC0 checksum
            0x04,                                        // Len=4 btes
                0xAD, 0x41, 0x23, 0xC4                   // Value CRC
    };

    // This is the message defined in section c.1.4 in the CMDL manual.
    // "Receive Status Response Example"
    unsigned char msg_1_4[] = { 
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,  // Universal Key
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,  // Universal Key
        0x2C,                                            // Len=44 bytes
        
            0xC1,                                        // Key - Category (0xC1=time)
            0x08,                                        // Len=8 bytes 
                0x00, 0x04, 0x42, 0xA7,                  // Time value bytes 0-3
                0xE6, 0x71, 0x50, 0x12,                  // Time value bytes 4-7 
                
            0x30,                                        // Key - 0x30 
            0x12,                                        // Len=18 bytes
                0x10,                                    // Key - 0x30->0x10 (Frequency Value)
                0x04,                                    // Len=4 bytes 
                    0x00, 0x06, 0x90, 0x78, 

                0x20,                                    // Key - 0x30->0x20 (Data Rate) 
                0x04,                                    // Len=4 bytes  
                    0x00, 0x00, 0x4E, 0x20, 
                
                0x24,                                    // Key - 0x30->0x24 (Modulation)
                0x01,                                    // Len=4 bytes   
                    0x00, 
                    
                0x80,                                    // Key - 0x30->0x80 (Tx Power)  
                0x01,                                    // Len=4 bytes   
                    0x80, 
                    
            0x40,                                        // Key - 0x40 (Media Encoder) 
            0x06,                                        // Len=6 bytes   
                0x40,                                    // Key - 0x40->0x40 (Video Encoder Enable)
                0x01,                                    // Len=1 bytes    
                    0x01,                                // Value byte (1=Enable)
                
                0x41,                                    // Key - 0x40->0x41 (Video Encoder Type)
                0x01,                                    // Len=1 bytes    
                    0x04,                                // Value byte (4 doesn't exist?) **********************
                    
            0xC0,                                        // Key - 0xC0 checksum
            0x04,                                        // Len=4 bytes   
                0x95, 0x60, 0x56, 0x1E
    };

}
