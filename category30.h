#ifndef _CATEGORY30_H_
#define _CATEGORY30_H_

#define TransmitEnableID    (0x01)
#define FreqValueID         (0x10)
#define DataRateBpsID       (0x20)
#define WaveformCatID       (0x21)
#define NetworkFramingID    (0x22)
#define SpreadSpectrumID    (0x23)
#define ModulationID        (0x24)
#define FECID               (0x25)
#define InterleaverID       (0x26)
#define ChannelMuxID        (0x27)
#define TmSubChanBpsID      (0x30)
#define TmSubChanFreqEncID  (0x31)
#define TmSubChanFreqDevID  (0x32)
#define TmSubChanFreqOsetID (0x33)
#define GuardIntervalID     (0x50)
#define ChannelBandwidthID  (0x51)
#define BandwidthTotalID    (0x52)
#define SpectrumInversionID (0x54)
#define TransmitPowerID     (0x80)
#define TransmitPowerUpID   (0x82)
#define TransmitEnableUpID  (0x84)
#define TransmitCardTempID  (0x90)

typedef struct
{
   uint8_t  txEnable;
   uint32_t freqValue;
   uint32_t dataRate;
   uint8_t  waveformCategory;
   uint8_t  networkFraming;
   uint8_t  spreadSpectrum;
   uint8_t  modulation;
   uint8_t  FEC;
   uint8_t  interleaver;
   uint8_t  channelMUX;
   uint32_t tlmSubChanBaudRate;
   uint8_t  tlmSubChanEncoding;
   uint32_t tlmSubChanFreqDeviation;
   uint32_t tlmSubChanFreqOffset;
   uint8_t  guardInterval;
   uint8_t  channelBandwidth;
   uint32_t bandwidthTotal;
   uint8_t  spectrumInversion;
   uint8_t  txPower;
   uint8_t  txPowerOnPowerUp;
   uint8_t  txEnableOnPowerUp;
   int8_t   txCardTemperature;
} TransmitterType;

#endif // _CATEGORY30_H_
