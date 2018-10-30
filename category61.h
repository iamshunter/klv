#ifndef _CATEGORY61_H_
#define _CATEGORY61_H_

#define PBIT_StatusID          (0x01)
#define PBIT_RAM_TestID        (0x20)
#define PBIT_NVM_TestID        (0x21)
#define PBIT_FPGA1_CommsTestID (0x30)
#define PBIT_FPGA2_CommsTestID (0x31)
#define PBIT_RX_CommsTestID    (0x40)
#define PBIT_TX_CommsTestID    (0x41)
#define PBIT_RF_CalFileTestID  (0x55)

typedef struct
{
   uint8_t pbitStatus;
   uint8_t ramTest;
   uint8_t nvmTest;
   uint8_t fpga1CommsTest;
   uint8_t fpga2CommsTest;
   uint8_t rxCommsTest;
   uint8_t txCommsTest;
   uint8_t rfCalFileTest;
}PowerOnBitType;

#endif // _CATEGORY61_H_
