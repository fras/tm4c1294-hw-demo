// File: bl_userhooks.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 26 Aug 2020
//
// User hook functions of the boot loader running on the TI Tiva TM4C1294
// Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "bl_config.h"
#include "bl_user.h"
#include "bl_userhooks.h"



// Global variables.
uint32_t g_ui32SysClock;



// A prototype for the function (in the startup code) for a predictable length
// delay.
extern void Delay(uint32_t ui32Count);



// Performs application-specific low level hardware initialization on system
// reset.
void BL_UserHwInit(void)
{
    // Set up the system clock.
    g_ui32SysClock = MAP_SysCtlClockFreqSet(SYSTEM_CLOCK_SETTINGS, SYSTEM_CLOCK_FREQ);

    // Set up the UART.
    // Enable the GPIO peripheral used by the UART.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Set up the UART7.
    MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_UART7);
    MAP_SysCtlPeripheralReset(SYSCTL_PERIPH_UART7);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    while (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_UART7));

    // Configure GPIO pins for UART mode.
    MAP_GPIOPinConfigure(GPIO_PC4_U7RX);
    MAP_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);
    MAP_GPIOPinConfigure(GPIO_PC5_U7TX);
    MAP_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);

    // Configure the UART for 115,200, 8-N-1 operation.
    MAP_UARTConfigSetExpClk(UART7_BASE, g_ui32SysClock, 115200,
                           (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                            UART_CONFIG_PAR_NONE));

    // Initialize the UART for console I/O.
//    UARTStdioConfig(7, 115200, g_ui32SysClock);
}



// Performs application-specific reinitialization on boot loader entry via SVC.
void BL_Reinit(void)
{
    // Re-initialize the hardware peripherals.
    BL_UserHwInit();

    // Show boot loader info.
    UARTprintBlInfo(UARTx_BASE);

    UARTprint(UARTx_BASE, "\r\nWaiting for firmware data...\r\n");
}



// Informs an application that a download is starting.
void BL_FwDownloadStart(void)
{
}



// Informs an application of download progress.
void BL_FwDownloadProgress(void)
{
}



// Informs an application that a download has completed.
void BL_FwDownloadEnd(void)
{
}



// Enable a new firmware download at system start up.
unsigned long BL_UserCheckUpdateHook(void)
{
    // Show boot loader info.
    UARTprintBlInfo(UARTx_BASE);

    return 0;
}

