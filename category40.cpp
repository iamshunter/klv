#include <stdio.h>
#include "util.h"
#include "util.h"

// We aren't quite interested in the media encoder just yet
int procMediaEncoder(void *ptr)
{
   int            retVal=0;
   unsigned int   nMsg, nLen, catLen;
   unsigned char *cPtr = (unsigned char *)ptr;

   // Verify the key/category
   if ( *cPtr != 0x40 )
   {
      printf("Key error, 0x%02X expected 0x%02X\n", *cPtr, 0x40);
      return 0;
   }

   // Get the length
   retVal++;   // Move past the key
   decodeBER(&cPtr[retVal], &catLen, &nLen); 
   retVal += nLen;

   // Loop over the available data
   int lastData=catLen+retVal;
   while ( retVal < lastData )
   {
      unsigned char subCat = cPtr[retVal];
      retVal++;
      decodeBER(&cPtr[retVal], &catLen, &nLen); 
      retVal += nLen;

      switch(subCat)
      {
         default:
            printf("Media Encoder Subkey 0x%02X found\n", subCat);
            dump(&cPtr[retVal], catLen);
      }
      retVal += catLen;
   }

   printf("\n\n");

   return retVal;
}
