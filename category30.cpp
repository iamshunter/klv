#include <stdio.h>
#include <time.h>
#include <zlib.h>
#include "util.h"
#include "category30.h"

// This routine generates a KLV message to control the "Transmitter" block for an L3 radio
//
// *********************************************
// @todo - It does NOT check the size nor need to use BER to encode the message size
// *********************************************
//
int genTransmitter(void *ptr, int maxBytes, TransmitterType values, TransmitterType flags)
{
   int            numBytes=0, bytesLeft, masterSave, txSave=0;
   unsigned char *cPtr = (unsigned char *)ptr;
   unsigned char *masterLenPtr, *txLenPtr;

   // Keep track of how much room is left in the output buffer
   bytesLeft = maxBytes;

   // Start with an assumption that we'll have less than 127 bytes in the message.
   // If that isn't true, we'll have to clean it up at the end and make a bigger
   // slot for the extra byte in the BER encoded size.

   // Set the universal key
   bytesLeft -= sizeof(UniversalKey);
   if ( bytesLeft < 0 ){
      return 0;
   }
   memcpy(cPtr, UniversalKey, sizeof(UniversalKey));
   numBytes += sizeof(UniversalKey);

   // Capture the spot where the size will go
   masterLenPtr = &cPtr[numBytes++];
   *masterLenPtr = 0xAB;
   printf("nb %d\n", numBytes);

   // Save off the current byte count
   masterSave = numBytes;

   // Store the timestamp, length and value
   time_t tt;
   cPtr[numBytes++] = Timestamp;
   cPtr[numBytes++] = 8;
   tt = (time_t)(((long)getDoubleTime())*1000000);
   //
   // Test value for debug from L3 document - tt = 0x000442A7E6715012L;
   //
   swap8(&tt, &cPtr[numBytes]);
   numBytes += 8;

   // Store the "Transmitter" category and the length
   cPtr[numBytes++] = Transmitter;

   // Grab the spot where the transmitter sub-key length will go
   txLenPtr = &cPtr[numBytes++];
   txSave = numBytes;

   if ( flags.txEnable != 0 )
   {
      // Set the key
      cPtr[numBytes++] = TransmitEnableID;

      // Set the size
      cPtr[numBytes++] = sizeof(values.txEnable);

      // Set the value
      cPtr[numBytes] = values.txEnable;
      numBytes += sizeof(values.txEnable);
   }

   // Store the "Frequency Value" category and the length
   if ( flags.freqValue != 0 )
   {
      // Set the key
      cPtr[numBytes++] = FreqValueID;

      // Set the size
      cPtr[numBytes++] = sizeof(values.freqValue);

      // Set the value use swap
      swap4(&values.freqValue, &cPtr[numBytes]);
      numBytes += sizeof(values.freqValue);
   }

   // Store the "Data Rate" category and the length
   if ( flags.dataRate != 0 )
   {
      // Set the key
      cPtr[numBytes++] = DataRateBpsID;

      // Set the size
      cPtr[numBytes++] = sizeof(values.dataRate);

      // Set the value
      swap4(&values.dataRate, &cPtr[numBytes]);
      numBytes += sizeof(values.dataRate);
   }

   //if ( flags.waveformCategory != 0 )
   //if ( flags.networkFraming != 0 )
   //if ( flags.spreadSpectrum != 0 )
   if ( flags.modulation != 0 )
   {
      // Set the key
      cPtr[numBytes++] = ModulationID;

      // Set the size
      cPtr[numBytes++] = sizeof(values.modulation);

      // Set the value
      cPtr[numBytes] = values.modulation;
      numBytes += sizeof(values.modulation);
   }

   //if ( flags.FEC != 0 )
   //if ( flags.interleaver != 0 )
   //if ( flags.channelMUX != 0 )
   //if ( flags.tlmSubChanBaudRate != 0 )
   //if ( flags.tlmSubChanEncoding != 0 )
   //if ( flags.tlmSubChanFreqDeviation != 0 )
   //if ( flags.tlmSubChanFreqOffset != 0 )
   //if ( flags.guardInterval != 0 )
   //if ( flags.channelBandwidth != 0 )
   //if ( flags.bandwidthTotal != 0 )
   //if ( flags.spectrumInversion != 0 )
   if ( flags.txPower != 0 )
   {
      // Set the key
      cPtr[numBytes++] = TransmitPowerID;

      // Set the size
      cPtr[numBytes++] = sizeof(values.txPower);

      // Set the value
      cPtr[numBytes] = values.txPower;
      numBytes += sizeof(values.txPower);
   }
   *txLenPtr     = (char)(numBytes-txSave);

   //if ( flags.txPowerOnPowerUp != 0 )
   //if ( flags.txEnableOnPowerUp != 0 )
   //if ( flags.txCardTemperature != 0 )

   // Append the checksum
   uint32_t masterSum=0;
   printf("pre-sum nb %d\n", numBytes);
   cPtr[numBytes++] = Checksum;
   cPtr[numBytes++] = (char)sizeof(masterSum);
   numBytes += sizeof(masterSum);

   *masterLenPtr = (char)(numBytes-masterSave);
   printf("masterLenPtr %02X\n", *masterLenPtr);
   printf("txLenPtr %02X\n", *txLenPtr);
   printf("pre txLenPtr %02X\n", txLenPtr[-1]);

   int numSum = numBytes-sizeof(masterSum);
   masterSum = crc32(0x00000000, (const unsigned char *)cPtr, numSum);
   swap4(&masterSum, &cPtr[numSum]);
   dump(cPtr, numBytes);
   return numBytes;
}

int procTransmitter(void *ptr, TransmitterType *values, TransmitterType *changed)
{
   int            retVal=0;
   unsigned int   nMsg, nLen, catLen;
   unsigned char *cPtr = (unsigned char *)ptr;

   // Clear out the copy of TransmitterType to record changes
   memset(changed, 0, sizeof(TransmitterType));

   // Verify the key/category
   if ( *cPtr != Transmitter )
   {
      printf("Key error, 0x%02X expected 0x%02X\n", *cPtr, Transmitter);
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
      bool processed=false;
      retVal++;
      decodeBER(&cPtr[retVal], &catLen, &nLen); 
      retVal += nLen;
      if (catLen)
      {
         switch(subCat)
         {
            case TransmitEnableID: 
               changed->txEnable = 1;
               values->txEnable = cPtr[retVal];
               break;
            case FreqValueID: 
               changed->freqValue = 1;
               swap4(&cPtr[retVal], &values->freqValue);
               printf("\n\tFreqValue decoded %d", values->freqValue);
               processed=true;
               break;
            case DataRateBpsID: 
               changed->dataRate = 1;
               swap4(&cPtr[retVal], &values->dataRate);
               printf("\n\tDataRate decoded %d", values->dataRate);
               processed = true;
               break;
            case WaveformCatID: 
               changed->waveformCategory = 1;
               values->waveformCategory = cPtr[retVal];
               break;
            case NetworkFramingID: 
               changed->networkFraming = 1;
               values->networkFraming = cPtr[retVal];
               break;
            case SpreadSpectrumID: 
               changed->spreadSpectrum = 1;
               values->spreadSpectrum = cPtr[retVal];
               break;
            case ModulationID: 
               changed->modulation = 1;
               values->modulation = cPtr[retVal];
               printf("\n\tModulation decoded %d", values->modulation);
               processed = true;
               break;
            case FECID: 
               changed->FEC = 1;
               values->FEC = cPtr[retVal];
               break;
            case InterleaverID: 
               changed->interleaver = 1;
               values->interleaver = cPtr[retVal];
               break;
            case ChannelMuxID: 
               changed->channelMUX = 1;
               values->channelMUX = cPtr[retVal];
               break;
            case TmSubChanBpsID: 
               changed->tlmSubChanBaudRate = 1;
               swap4(&cPtr[retVal], &values->tlmSubChanBaudRate);
               //retVal+= 4;
               break;
            case TmSubChanEncID: 
               changed->tlmSubChanEncoding = 1;
               values->tlmSubChanEncoding = cPtr[retVal];
               break;
            case TmSubChanFreqDevID: 
               changed->tlmSubChanFreqDeviation = 1;
               swap4(&cPtr[retVal], &values->tlmSubChanFreqDeviation);
               //retVal+= 4;
               break;
            case TmSubChanFreqOsetID:
               changed->tlmSubChanFreqOffset = 1;
               swap4(&cPtr[retVal], &values->tlmSubChanFreqOffset);
               //retVal+= 4;
               break;
            case GuardIntervalID: 
               changed->guardInterval = 1;
               values->guardInterval = cPtr[retVal];
               break;
            case ChannelBandwidthID: 
               changed->channelBandwidth = 1;
               values->channelBandwidth = cPtr[retVal];
               break;
            case BandwidthTotalID: 
               changed->bandwidthTotal = 1;
               swap4(&cPtr[retVal], &values->bandwidthTotal);
               //retVal+= 4;
               break;
            case SpectrumInversionID:
               changed->spectrumInversion = 1;
               values->spectrumInversion = cPtr[retVal];
               break;
            case TransmitPowerID: 
               changed->txPower = 1;
               values->txPower = cPtr[retVal];
               printf("\n\tTransmit power decoded %d", values->txPower);
               processed = true;
               break;
            case TransmitPowerUpID: 
               changed->txPowerOnPowerUp = 1;
               values->txPowerOnPowerUp = cPtr[retVal];
               break;
            case TransmitEnableUpID: 
               changed->txEnableOnPowerUp = 1;
               values->txEnableOnPowerUp = cPtr[retVal];
               break;
            case TransmitCardTempID: 
               {
               char *scPtr = (char *)&cPtr[retVal];
               changed->txCardTemperature = 1;
               values->txCardTemperature = *scPtr;
               }
               break;
            default:
               printf("Subkey not found\n");
         }
      }
      if ( !processed )
      {
         printf("\n\tSubkey 0x%02X", subCat);
      }
      retVal += catLen;
   }

   printf("\n\n");

   return retVal;
}
