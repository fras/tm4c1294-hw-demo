// File: ssi.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 06 Apr 2020
// Rev.: 11 Apr 2020
//
// Synchronous Serial Interface (SSI) functions on the TI Tiva TM4C1294
// Connected LaunchPad Evaluation Kit.
//
// The SSI can be configured to use either the Motorola SPI or the Texas
// Instruments synchronous serial interface frame formats.
//



#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "ssi.h"



// Initialize an SSI master.
void SsiMasterInit(tSSI *psSsi)
{
    // Set up the IO pins for the SSI master.
    SysCtlPeripheralEnable(psSsi->ui32PeripheralGpio);
    GPIOPinConfigure(psSsi->ui32PinConfigClk);
    GPIOPinConfigure(psSsi->ui32PinConfigFss);
    GPIOPinConfigure(psSsi->ui32PinConfigRx);       // MISO
    GPIOPinConfigure(psSsi->ui32PinConfigTx);       // MOSI
    GPIOPinTypeSSI(psSsi->ui32PortGpioBase,
                   psSsi->ui8PinGpioClk |
                   psSsi->ui8PinGpioFss |
                   psSsi->ui8PinGpioRx  |
                   psSsi->ui8PinGpioTx);

    // Set up the SSI master.
    SysCtlPeripheralDisable(psSsi->ui32PeripheralSsi);
    SysCtlPeripheralReset(psSsi->ui32PeripheralSsi);
    SysCtlPeripheralEnable(psSsi->ui32PeripheralSsi);
    while(!SysCtlPeripheralReady(psSsi->ui32PeripheralSsi));
    SSIConfigSetExpClk(psSsi->ui32BaseSsi, psSsi->ui32SsiClk,
                       psSsi->ui32Protocol,
                       psSsi->ui32Mode,
                       psSsi->ui32BitRate,
                       psSsi->ui32DataWidth);
    SSIEnable(psSsi->ui32BaseSsi);
}



// Write data to an SSI master.
uint32_t SsiMasterWrite(tSSI *psSsi, uint32_t *pui32Data, uint8_t ui8Length)
{
    uint32_t ui32Timeout = psSsi->ui32Timeout + 10;     // Guarantee some minimum timeout value.

    if (ui8Length < 1) return 1;

    // Send data.
    for (int i = 0; i < ui8Length; i++) {
        SSIDataPut(psSsi->ui32BaseSsi, pui32Data[i]);
        // Wait until the transfer is finished.
        SysCtlDelay(psSsi->ui32SsiClk / 3e5);   // 10 us delay.
                                                // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!SSIBusy(psSsi->ui32BaseSsi)) break;
            SysCtlDelay(psSsi->ui32SsiClk / 3e5);   // 10 us delay.
            // Timeout while waiting for the SSI master to be free.
            if (i == ui32Timeout) {
                return -1;
            }
        }
    }

    return 0;
}



// Read data from an SSI master (blocking).
int32_t SsiMasterRead(tSSI *psSsi, uint32_t *pui32Data, uint8_t ui8Length)
{
    uint32_t ui32Timeout = psSsi->ui32Timeout + 10;     // Guarantee some minimum timeout value.
    int32_t i32Cnt = 0;

    if (ui8Length < 1) return 1;

    // Receive data.
    for (int i = 0; i < ui8Length; i++) {
        i32Cnt += SSIDataGetNonBlocking(psSsi->ui32BaseSsi, &pui32Data[i]);
        // Wait until the transfer is finished.
        SysCtlDelay(psSsi->ui32SsiClk / 3e5);   // 10 us delay.
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!SSIBusy(psSsi->ui32BaseSsi)) break;
            SysCtlDelay(psSsi->ui32SsiClk / 3e5);   // 10 us delay.
            // Timeout while waiting for the I2C bus to be free.
            if (i == ui32Timeout) {
                return -1;
            }
        }
    }

    return i32Cnt;
}

