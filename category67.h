#ifndef _CATEGORY67_H_
#define _CATEGORY67_H_

#define IBIT_StatusID           (0x01)
#define IBIT_CommandID          (0x02)
#define IBIT_RAM_TestID         (0x20)
#define IBIT_NVM_TestID         (0x21)
#define IBIT_FPGA1_CommsTestID  (0x30)
#define IBIT_FPGA2_CommsTestID  (0x31)
#define IBIT_RX_CommsTestID     (0x40)
#define IBIT_TX_CommsTestID     (0x41)
#define IBIT_ModemTempID        (0x50)
#define IBIT_ModemDigLoopTestID (0x54)
#define IBIT_RF_CalFileTestID   (0x55)
#define IBIT_ModemRfTxPllID     (0x60)
#define IBIT_ModemRfRxPllID     (0x61)

typedef struct
{
   uint8_t ibitStatus;
   uint8_t ibitCommand;
   uint8_t ramTest;
   uint8_t nvmTest;
   uint8_t fpga1CommsTest;
   uint8_t fpga2CommsTest;
   uint8_t rXCommsTest;
   uint8_t tXCommsTest;
   uint8_t modemTemp;
   uint8_t modemDigLoopTest;
   uint8_t rfCalFileTest;
   uint8_t modemRfTxPll;
   uint8_t modemRfRxPll;
}InitiatedBitType;

#endif // _CATEGORY67_H_
