// File: uart_ui.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 18 Feb 2020
//
// UART user interface for the TI Tiva TM4C1294 connected LaunchPad evaluation
// kit.
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
void UartUiInit(uint32_t ui32SysClock)
{
    // Enable the GPIO Peripheral used by the UART.
    SysCtlPeripheralEnable(UART_UI_SYSCTL_PERIPH_GPIO);

    // Enable UART7
    SysCtlPeripheralEnable(UART_UI_SYSCTL_PERIPH_UART);

    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(UART_UI_GPIO_RX);
    GPIOPinConfigure(UART_UI_GPIO_TX);
    GPIOPinTypeUART(UART_UI_GPIO_PORT_BASE, UART_UI_GPIO_PIN_RX | UART_UI_GPIO_PIN_TX);

    // Initialize the UART for console I/O.
    UARTStdioConfig(UART_UI_NUM, UART_UI_BAUD, ui32SysClock);
}

