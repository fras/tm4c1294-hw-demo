// File: ssi.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 06 Apr 2020
// Rev.: 06 Apr 2020
//
// Header file for the Synchronous Serial Interface (SSI) functions on the TI
// Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#ifndef __SSI_H__
#define __SSI_H__



// Types.
typedef struct {
    uint32_t ui32PeripheralSsi;
    uint32_t ui32PeripheralGpio;
    uint32_t ui32PortGpioBase;
    uint8_t  ui8PinGpioClk;
    uint8_t  ui8PinGpioFss;
    uint8_t  ui8PinGpioRx;
    uint8_t  ui8PinGpioTx;
    uint32_t ui32PinConfigClk;
    uint32_t ui32PinConfigFss;
    uint32_t ui32PinConfigRx;
    uint32_t ui32PinConfigTx;
    uint32_t ui32BaseSsi;
    uint32_t ui32SysClock;
    uint32_t ui32Protocol;
    uint32_t ui32Mode;
    uint32_t ui32BitRate;
    uint32_t ui32DataWidth;
    uint32_t ui32Timeout;
} tSSI;


// Function prototypes.
void SsiMasterInit(tSSI *psSsi);
uint32_t SsiMasterWrite(tSSI *psSsi, uint32_t *pui32Data, uint8_t ui8Length);
int32_t SsiMasterRead(tSSI *psSsi, uint32_t *pui32Data, uint8_t ui8Length);



#endif  // __SSI_H__

