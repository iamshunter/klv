#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include "klvL1.h"
#include "util.h"

// BER is the acronym for Basic Encoding Rules.  From the L3 ICD:
//
// "If the first byte in the length field does not have the high bit set (0x80), 
// then that single byte represents an integer between 0 and 127 and indicates 
// the number of bytes that immediately follows in the Value field. If the high 
// bit is set, then the lower seven bits indicate how many bytes follow that 
// themselves make up a length field. For example if the first byte of a BER 
// length field is binary 10000010, that would indicate that the next two bytes 
// make up an integer that then indicates how many Value bytes follow. Therefore 
// a total of three bytes were taken up to specify a length."
//
int encodeBER(int len, void *out, int outSize)
{

    int outBytes = 0;

    if (len <= 127)
    {
        unsigned char *ptr = (unsigned char *)out;
        *ptr = len;             // Encode the message length
        outBytes = 1;           // Return that we used 1 byte to encode
    } else
    {
        unsigned char *cptr = (unsigned char *)out;
        if (len <= 0xFFFF)
        {
            if (outSize < 2)    // Not enough room to store the result
            {
                outBytes = -1;
            }
            unsigned short *ptr = (unsigned short *)&cptr[1];
            *ptr = len;
            outBytes = 2;
            cptr[0] = 0x80 | outBytes;
        } else                  // Not going to bother with messages greater than 64K, won't fit in Ethernet anyway
        {
            outBytes = -2;
        }
    }
    return outBytes;
}

bool decodeBER(void *ptr, unsigned int *nBytesMsg, unsigned int *nBytesLen)
{
    unsigned char *in = (unsigned char *)ptr;
    if ((in[0] & 0x80) == 0)
    {
        *nBytesMsg = in[0];
        *nBytesLen = 1;
    } else
    {
        *nBytesLen = (in[0] & 0x7F);
        unsigned short *ptr = (unsigned short *)&in[1];
        *nBytesMsg = *ptr;
    }
}

// Code to process frames of KLV data from L3 radios
// Level 1 will come with a 16 byte UniversalKey, we look for that key
// and validate the rest of the buffer with the CRC
//
// If the frame is valid, we'll loop through the data looking for data we're
// interested in.
#define MIN_CRC_SIZE (6)
#define MIN_LEN_SIZE (1)
bool procLevel1(void *bfr, int bfrSize)
{

    // Is there even enough data to try to process this as an L3 KLV message
    // We expect the UniversalKey/Len pair and a CRC KLV payload as a minimum
    if ( bfrSize < (sizeof(UniversalKey)+MIN_LEN_SIZE + MIN_CRC_SIZE) )
    {
       return false;
    }

    unsigned int nMsg, totMsg;
    unsigned int nLen;
    unsigned char *cPtr=(unsigned char *)bfr;
    unsigned int totProc=0;

    dump(bfr, bfrSize);

    // --------------------------------
    // Look for the Universal Key value
    // --------------------------------
    if (memcmp(cPtr, UniversalKey, sizeof(UniversalKey)) == 0)
    {
        printf("Found Universal Key\n");
        decodeBER(&cPtr[sizeof(UniversalKey)], &totMsg, &nLen);
        //printf("unBER  %5d %d\n", totMsg, nLen);
        //Found the Universal Key and grabbed the length.
        // Check the buffer sizes
        if ((totMsg+sizeof(UniversalKey)) > bfrSize )
        {
           printf("Buffer trouble\n");
           return false;
        }

        // Get the length and figure out where the checksum will be
        cPtr += sizeof(UniversalKey);
        cPtr += nLen;
        unsigned int *crcPtr;
        crcPtr = (unsigned int *)&cPtr[totMsg-sizeof(*crcPtr)];

        // -- Step 1, validate the message.
        int sumLen   = totMsg+sizeof(UniversalKey)+nLen-sizeof(*crcPtr);
        unsigned int crcCalc = crc32(0x00000000, (const unsigned char *)bfr, sumLen);
        if ( crcCalc != ntohl(*crcPtr) )
        {
           printf("CRC failure calc sum %08X != %08X transmitted sum\n");
           return false;
        }
        //else
        //{
        //   printf("CRC success calc sum %08X == %08X transmitted sum\n");
        //}

        // -- We've got a valid chunk of data ...
        //
        //   ++ Now, while there's data, pull level 2 data out

        totProc = sizeof(UniversalKey)+nLen;
        totProc = nLen;
        printf("totProc %2d:%2d\n", totProc, totMsg);
        while(totProc < totMsg)
        {
            // Look for a category
            unsigned char category = *cPtr;
            printf("2nd Level category %02x\n", category );
            cPtr++; // skip past the category
            totProc++;
            decodeBER(cPtr, &nMsg, &nLen);
            //printf("unBER  %5d %d\n", nMsg, nLen);
            cPtr += nLen; // Move past the length byte(s)
            totProc += nLen;
            cPtr += nMsg; // Move past the message byte(s)
            totProc += nMsg;

            printf("totProc %2d\n", totProc);
        }

        return true;

        for ( int ii = 0; ii < 8; ii++ )
           printf(" 0x%02X", cPtr[ii]);
        printf("\n");

        time_t tt;
        char *oPtr = (char *)&tt;
        oPtr[0] = cPtr[7];
        oPtr[1] = cPtr[6];
        oPtr[2] = cPtr[5];
        oPtr[3] = cPtr[4];
        oPtr[4] = cPtr[3];
        oPtr[5] = cPtr[2];
        oPtr[6] = cPtr[1];
        oPtr[7] = cPtr[0];

        printf("time %016lX ", tt );
        tt /= 1000000;
        printf("time %s\n", ctime(&tt));
        cPtr += nMsg; // Move past the Value

        // Look for the next key/category
        printf("Next 2nd Level category %02x\n", *cPtr );
        cPtr++;                                      // skip past the category
        decodeBER(cPtr, &nMsg, &nLen);
        //printf("unBER  %5d %d\n", nMsg, nLen);
        cPtr += nLen;                                // Move past the length byte(s)

        // Look for sub-key
        printf("Next 2nd Level sub-key %02x\n", *cPtr );
        cPtr++;                                      // skip past the category
        decodeBER(cPtr, &nMsg, &nLen);
        //printf("unBER  %5d %d\n", nMsg, nLen);
        cPtr += nLen;                                // Move past the length byte(s)

        // Extract the data
        int *freqPtr;
        freqPtr = (int *)cPtr;
        printf("Freq %016lX %d\n", *freqPtr, htonl(*freqPtr) );
        cPtr += nMsg;

        // Look for the next key/category (checksum0
        printf("Next 2nd Level category %02x\n", *cPtr );
        cPtr++;                                      // skip past the category
        decodeBER(cPtr, &nMsg, &nLen);
        //printf("unBER  %5d %d\n", nMsg, nLen);

        // Validate the sum
        unsigned char *tPtr = (unsigned char *)bfr;
        for (int ii = 0; ii < sumLen; ii++)
        {
            if ( ii%16 == 0 )
               printf("\n%3d) ", ii);
            printf(" %02X", tPtr[ii] );
        }
/*

        unsigned int *crcPtr = (unsigned int *)&tPtr[sumLen];
        printf("\n");
        printf("Calculated sum 0x%08X 0x%08X\n", 
                crc32(0x00000000, (const unsigned char *)bfr, sumLen), ntohl(*crcPtr) );
*/

    } 
    else if (memcmp(cPtr, UniversalKeyExternal, sizeof(UniversalKey)) == 0)
    {
        printf("Found Universal Key External\n");
    }
}
