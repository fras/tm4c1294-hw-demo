// File: uart_ui.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 10 Apr 2020
//
// UART user interface (UI) for the TI Tiva TM4C1294 Connected LaunchPad
// Evaluation Kit.
//



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "uart_ui.h"



// Initialize the UART user interface and its pins.
void UartUiInit(tUartUi *psUartUi)
{
    // Enable the GPIO Peripheral used by the UART.
    SysCtlPeripheralEnable(psUartUi->ui32PeripheralGpio);

    // Setup the UART.
    SysCtlPeripheralDisable(psUartUi->ui32PeripheralUart);
    SysCtlPeripheralReset(psUartUi->ui32PeripheralUart);
    SysCtlPeripheralEnable(psUartUi->ui32PeripheralUart);

    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(psUartUi->ui32PinConfigRx);
    GPIOPinConfigure(psUartUi->ui32PinConfigTx);
    GPIOPinTypeUART(psUartUi->ui32PortGpioBase, psUartUi->ui8PinGpioRx | psUartUi->ui8PinGpioTx);

    // Initialize the UART for console I/O.
    UARTStdioConfig(psUartUi->ui32Port, psUartUi->ui32Baud, psUartUi->ui32SrcClock);
}

