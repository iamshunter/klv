#ifndef _CATEGORY38_H_
#define _CATEGORY38_H_

#define ReceiverEnableID            (0x01)
#define FrequencyValueID            (0x10)
#define AutoConnectEnableID         (0x11)
#define DataRateID                  (0x20)
#define WaveformCategoryID          (0x21)
#define NetworkFramingID            (0x22)
#define SpreadSpectrumID            (0x23)
#define InterleaverID               (0x26)
#define ChanDEMUXID                 (0x27)
#define TransmitReceiverConfigID    (0x2D)
#define TlmSubchanBaudRateID        (0x30)
#define LNAVoltageID                (0x40)
#define SearchModeID                (0x50)
#define SearchTypeID                (0x60)
#define SearchStartFreqID           (0x61)
#define SearchEndFreqID             (0x62)
#define SearchStepSizeID            (0x63)
#define SearchDirectionID           (0x64)
#define ConfigSearchFileActiveID    (0x66)
#define ReceivedSignalStrengthIndID (0x80)
#define LinkEstablishedStatusID     (0x81)
#define CDLLinkStatusDetailsID      (0x83)
#define ReceiverDataLockID          (0x84)

typedef struct
{
   uint8_t  receiverEnable;
   uint32_t frequencyValue;
   uint8_t  autoConnectEnable;
   uint32_t dataRate;
   uint8_t  waveformCategory;
   uint8_t  networkFraming;
   uint8_t  spreadSpectrum;
   uint8_t  interleaver;
   uint8_t  chanDEMUX;
   uint8_t  transmitReceiverConfig;
   uint8_t  tlmSubchanBaudRate;
   uint8_t  lnaVoltage;
   uint8_t  searchMode;
   uint8_t  searchType;
   uint32_t searchStartFreq;
   uint32_t searchEndFreq;
   uint32_t searchStepSize;
   uint8_t  searchDirection;
   uint8_t  configSearchFileActive;
   uint8_t  receivedSignalStrengthInd;
   uint8_t  linkEstablishedStatus;
   uint8_t  cdlLinkStatusDetails;
   uint8_t  receiverDataLock;
}ReceiverType;

#endif // _CATEGORY38_H_
