// File: uart.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 18 Feb 2020
// Rev.: 18 Feb 2020
//
// UART functions on the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "uart.h"



// Initialize an UART.
void UartInit(tUART *uart)
{
    // Setup the IO pins for the UART.
    SysCtlPeripheralEnable(uart->ui32PeripheralGpio);
    GPIOPinConfigure(uart->ui32PinConfigRx);
    GPIOPinConfigure(uart->ui32PinConfigTx);
    GPIOPinTypeUART(uart->ui32PortGpio, uart->ui8PinGpioRx | uart->ui8PinGpioTx);

    // Setup the UART.
    SysCtlPeripheralDisable(uart->ui32PeripheralUart);
    SysCtlPeripheralReset(uart->ui32PeripheralUart);
    SysCtlPeripheralEnable(uart->ui32PeripheralUart);
    UARTConfigSetExpClk(uart->ui32BaseUart, uart->ui32SysClock, uart->ui32Baud, uart->ui32Config);
    UARTFIFOEnable(uart->ui32BaseUart);
    if (uart->bLoopback) UARTLoopbackEnable(uart->ui32BaseUart);
    UARTEnable(uart->ui32BaseUart);
}



// Write data to an UART.
uint32_t UartWrite(tUART *uart, uint8_t *ui8Data, uint8_t ui8Length)
{
    for (int i = 0; i < ui8Length; i++) {
        UARTCharPut(uart->ui32BaseUart, ui8Data[i]);
    }

    return 0;
}



// Read data from an UART (non-blocking).
uint32_t UartRead(tUART *uart, uint8_t *ui8Data, uint8_t ui8Length)
{
    int cnt = 0;

    for (cnt = 0; cnt < ui8Length; cnt++) {
        if (UARTCharsAvail(uart->ui32BaseUart)) {
            ui8Data[cnt] = (uint8_t) UARTCharGet(uart->ui32BaseUart) & 0xff;
        } else {
            break;
        }
    }

    return cnt;
}



// Read data from an UART (blocking).
uint32_t UartReadBlocking(tUART *uart, uint8_t *ui8Data, uint8_t ui8Length)
{
    for (int i = 0; i < ui8Length; i++) {
        ui8Data[i] = (uint8_t) UARTCharGet(uart->ui32BaseUart) & 0xff;
    }

    return 0;
}

