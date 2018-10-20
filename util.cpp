#include <stdio.h>
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
