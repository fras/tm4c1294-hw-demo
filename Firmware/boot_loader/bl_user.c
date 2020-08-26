// File: bl_user.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 26 Aug 2020
//
// User functions of the boot loader running on the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "bl_config.h"
#include "bl_user.h"



// Global variables.
extern uint32_t g_ui32SysClock;



// A prototype for the function (in the startup code) for a predictable length
// delay.
extern void Delay(uint32_t ui32Count);



// Delay in microseconds.
void DelayUs(uint32_t ui32DelayUs)
{
    // CAUTION: Calling SysCtlDelay(0) will hang the system.
    if (ui32DelayUs > 0)
        // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
        SysCtlDelay((g_ui32SysClock / 3e6) * ui32DelayUs);
}



// Print a string to an UART.
void UARTprint(uint32_t ui32UartBase, const char* pcStr)
{
	int size = strlen(pcStr);
	for (int i = 0; i < size; ++i) {
		// Write the next character to the UART.
		MAP_UARTCharPut(ui32UartBase, pcStr[i]);
	}
}



// Print boot loader information.
void UARTprintBlInfo(uint32_t ui32UartBase)
{
    UARTprint(ui32UartBase, "\r\n\r\n");
    UARTprint(ui32UartBase, "\r\n***** TIVA TM4C1294 " BL_NAME " version " BL_VERSION ", release date: " BL_RELEASEDATE " *****\r\n");
    // Wait some time for UART to send out the message.
    Delay(100000);
}


