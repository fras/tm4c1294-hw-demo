// File: uart.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 18 Feb 2020
// Rev.: 17 Apr 2020
//
// UART functions on the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uart.h"



// Initialize an UART.
void UartInit(tUART *psUart)
{
    // Set up the IO pins for the UART.
    SysCtlPeripheralEnable(psUart->ui32PeripheralGpio);
    GPIOPinConfigure(psUart->ui32PinConfigRx);
    GPIOPinConfigure(psUart->ui32PinConfigTx);
    GPIOPinTypeUART(psUart->ui32PortGpioBase, psUart->ui8PinGpioRx | psUart->ui8PinGpioTx);

    // Set up the UART.
    SysCtlPeripheralDisable(psUart->ui32PeripheralUart);
    SysCtlPeripheralReset(psUart->ui32PeripheralUart);
    SysCtlPeripheralEnable(psUart->ui32PeripheralUart);
    UARTConfigSetExpClk(psUart->ui32BaseUart, psUart->ui32UartClk, psUart->ui32Baud, psUart->ui32Config);
    UARTFIFOEnable(psUart->ui32BaseUart);
    if (psUart->bLoopback) UARTLoopbackEnable(psUart->ui32BaseUart);
    UARTEnable(psUart->ui32BaseUart);
}



// Write data to an UART.
uint32_t UartWrite(tUART *psUart, uint8_t *pui8Data, uint8_t ui8Length)
{
    for (int i = 0; i < ui8Length; i++) {
        UARTCharPut(psUart->ui32BaseUart, pui8Data[i]);
    }

    return 0;
}



// Read data from an UART (non-blocking).
uint32_t UartRead(tUART *psUart, uint8_t *pui8Data, uint8_t ui8Length)
{
    int cnt = 0;

    for (cnt = 0; cnt < ui8Length; cnt++) {
        if (UARTCharsAvail(psUart->ui32BaseUart)) {
            pui8Data[cnt] = (uint8_t) UARTCharGet(psUart->ui32BaseUart) & 0xff;
        } else {
            break;
        }
    }

    return cnt;
}



// Read data from an UART (blocking).
uint32_t UartReadBlocking(tUART *psUart, uint8_t *pui8Data, uint8_t ui8Length)
{
    for (int i = 0; i < ui8Length; i++) {
        pui8Data[i] = (uint8_t) UARTCharGet(psUart->ui32BaseUart) & 0xff;
    }

    return 0;
}

