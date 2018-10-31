#include <stdio.h>
#include <sys/timeb.h>
#include "util.h"

void dump(void *bfr, int count)
{
    unsigned char *cPtr = (unsigned char *)bfr;
    for ( int ii = 0; ii < count; ii++ )
    {
        if ( ii%16 == 0 )
        {
            printf("\n%3d) ", ii);
        }
        printf(" %02X", cPtr[ii]);
    }
    printf("\n");
}

unsigned char bits[65];
char *printBits(unsigned char byte)
{
   printf("******* ");
   int ii = 0;
   for ( ii = 7; ii >= 0; ii-- )
   {
//      printf("(%x:%x) ", 0x1<<ii, byte);
      if ( (0x1<<ii) & byte )
      {
         bits[7-ii] = '1';
//         printf("1:%d", ii);
      }
      else
      {
         bits[7-ii] = '0';
//         printf("0:%d", ii);
      }
   }

   bits[ii] = 0;
   printf("******* ");
   return (char *)&bits[0];
}
char *printBits(char byte)
{
   return printBits((unsigned char)byte);
}

char *printBits(unsigned short bytes)
{
   printf("******* ");
   int ii = 0;
   for ( ii = 15; ii >= 0; ii-- )
   {
//      printf("(%x:%x) ", 0x1<<ii, byte);
      if ( (0x1<<ii) & bytes )
      {
         bits[15-ii] = '1';
//         printf("1:%d", ii);
      }
      else
      {
         bits[15-ii] = '0';
//         printf("0:%d", ii);
      }
   }

   bits[ii] = 0;
   printf("******* ");
   return (char *)&bits[0];
}
char *printBits(short byte)
{
   return printBits((unsigned short)byte);
}

// A general purpose byte swapping routine, buffers can NOT overlap
inline void genSwap(unsigned char *in, unsigned char *out, int byteCount)
{
   unsigned char tmp;
   for (int indx = 0, outdx=byteCount-1; indx < byteCount; indx++, outdx-- )
   {
      out[outdx] = in[indx];
   }
}

void swap2(void *inPtr, void *outPtr)
{
   unsigned char *cIn=(unsigned char *)inPtr;
   unsigned char *cOut=(unsigned char *)outPtr;
   genSwap(cIn, cOut, 2);
}

void swap4(void *inPtr, void *outPtr)
{
   unsigned char *cIn=(unsigned char *)inPtr;
   unsigned char *cOut=(unsigned char *)outPtr;
   genSwap(cIn, cOut, 4);
}

void swap8(void *inPtr, void *outPtr)
{
   unsigned char *cIn=(unsigned char *)inPtr;
   unsigned char *cOut=(unsigned char *)outPtr;
   genSwap(cIn, cOut, 8);
}

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

void decodeBER(void *ptr, unsigned int *nBytesMsg, unsigned int *nBytesLen)
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

double getDoubleTime(void)
{
   struct timeb tval;
   ftime(&tval);
   return (tval.time + tval.millitm/1000.0);
}
