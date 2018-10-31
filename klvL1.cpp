#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include "klvL1.h"
#include "util.h"

int procTimestamp(void *ptr, double *dTime)
{
   int retVal=0;
   unsigned int nMsg, nLen;
   unsigned char *cPtr = (unsigned char *)ptr;
   time_t        tmpTime;

   // Verify the key/category
   if ( *cPtr != Timestamp )
   {
      printf("Key error, 0x%02X expected 0x%02X\n", *cPtr, Timestamp);
      return 0;
   }

   // Get the length
   retVal++;
   decodeBER(&cPtr[retVal], &nMsg, &nLen);

   // Move past the length
   retVal += nLen;

   // Get the time
   swap8(&cPtr[retVal], &tmpTime);
   *dTime = tmpTime/1000000.;

   retVal += 8;
   return retVal;
}

// Code to process frames of KLV data from L3 radios
// Level 1 will come with a 16 byte UniversalKey, we look for that key
// and validate the rest of the buffer with the CRC
//
// If the frame is valid, we'll loop through the data looking for data we're
// interested in.
#define MIN_CRC_SIZE (6)
#define MIN_LEN_SIZE (1)
bool procKLV(void *bfr, int bfrSize)
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

    TransmitterType txValue, txUpdated;

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
           printf("CRC failure calc sum %08X != %08X transmitted sum\n", crcCalc, ntohl(*crcPtr) );
           return false;
        }
        else
        {
           printf("CRC success calc sum %08X == %08X transmitted sum\n", crcCalc, ntohl(*crcPtr) );
        }

        // -- We've got a valid message.
        //
        //   ++ Now, while there's data, pull level 2 data out
        totProc = sizeof(UniversalKey)+nLen;
        totProc = nLen;
        printf("totProc %2d:%2d\n", totProc, totMsg);
        while(totProc < totMsg)
        {
            // Look for a base key or category
            unsigned char category = *cPtr;
            printf("Base-key/category %02x\n", category );

            double dTime;
            bool   processed=true;
            int    numProc;
            processed=false;
            switch(*cPtr)
            {
               case Checksum:
                  printf("Checksum\n");
                  break;
               case Timestamp:
                  printf("Timestamp::");
                  numProc = procTimestamp(cPtr, &dTime);
                  if ( numProc )
                  {
                      cPtr += numProc;
                      processed = true;
                      time_t tt = (time_t)dTime;
                      printf("%s\n", ctime(&tt));
                  }
                  totProc += numProc;
                  break;
               case Type:
                  printf("Type\n");
                  break;
               case Configuration:
                  printf("Configuration\n");
                  break;
               case Interfaces:
                  printf("Interfaces\n");
                  break;
               case Transmitter:
                  printf("Transmitter::");
                  numProc = procTransmitter(cPtr, &txValue, &txUpdated);
                  if ( numProc )
                  {
                      cPtr += numProc;
                      processed = true;
                  }
                  totProc += numProc;
                  break;
               case Receiver:
                  printf("Receiver\n");
                  break;
               case MediaEncoder:
                  printf("MediaEncoder\n");
                  numProc = procMediaEncoder(cPtr);
                  if ( numProc )
                  {
                      cPtr += numProc;
                      processed = true;
                  }
                  totProc += numProc;
                  break;
               case MediaDecoder:
                  printf("MediaDecoder\n");
                  break;
               case Routing:
                  printf("Routing\n");
                  break;
               case PacketMux:
                  printf("PacketMux\n");
                  break;
               case PowerOnBIT:
                  printf("PBIT\n");
                  break;
               case ContinuousBIT:
                  printf("CBIT\n");
                  break;
               case InitiatedBIT:
                  printf("IBIT\n");
                  break;
               case Encryption:
                  printf("Encryption\n");
                  break;
               case CSICommand:
                  printf("CSI Command\n");
                  break;
               case CSIStatus:
                  printf("CSI Status\n");
                  break;
               case KLVTelemetry:
                  printf("KLV Tm\n");
                  break;
               case NavData:
                  printf("Nav Data\n");
                  break;
               default:
                  printf("Default\n");
            }

            if (!processed)
            {
               cPtr++; // skip past the category
               totProc++;
               decodeBER(cPtr, &nMsg, &nLen);
               cPtr += nLen; // Move past the length byte(s)
               totProc += nLen;
               cPtr += nMsg; // Move past the message byte(s)
               totProc += nMsg;
            }

        }

        printf("Total bytes processed %2d\n", totProc);
    } 
    else if (memcmp(cPtr, UniversalKeyExternal, sizeof(UniversalKey)) == 0)
    {
        printf("Found Universal Key External\n");
    }
}
