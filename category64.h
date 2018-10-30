#ifndef _CATEGORY64_H_
#define _CATEGORY64_H_

#define CBIT_StatusID          (0x01)
#define CBIT_FPGA1_CommsTestID (0x30)
#define CBIT_FPGA2_CommsTestID (0x31)
#define CBIT_RX_CommsTestID    (0x40)
#define CBIT_TX_CommsTestID    (0x41)
#define CBIT_ModemTempID       (0x50)
#define CBIT_RF_CalFileTestID  (0x55)
#define CBIT_ModemRfTxPllID    (0x60)
#define CBIT_ModemRfRxPllID    (0x61)

typedef struct
{
   uint8_t pbitStatus;
   uint8_t fpga1CommsTest;
   uint8_t fpga2CommsTest;
   uint8_t rxCommsTest;
   uint8_t txCommsTest;
   uint8_t rfCalFileTest;
   uint8_t modemTempID;
   uint8_t rfCalFileTestID;
   uint8_t modemRfTxPllID;
   uint8_t modemRfRxPllID;
}ContinuousBitType;

#endif // _CATEGORY64_H_
