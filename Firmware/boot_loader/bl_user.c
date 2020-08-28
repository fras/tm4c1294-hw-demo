// File: bl_user.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 27 Aug 2020
//
// User functions of the boot loader running on the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/ustdlib.h"
#include "hw/gpio/gpio.h"
#include "hw/gpio/gpio_led.h"
#include "bl_config.h"
#include "bl_user.h"
#include "bl_user_io.h"



// Global variables.
uint32_t g_ui32SysClock;
uint8_t g_ui8Led;



// A prototype for the function (in the startup code) for a predictable length
// delay.
extern void Delay(uint32_t ui32Count);



// Delay in microseconds.
void DelayUs(uint32_t ui32DelayUs)
{
    // CAUTION: Calling SysCtlDelay(0) will hang the system.
    if (ui32DelayUs > 0)
        // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
        ROM_SysCtlDelay((g_ui32SysClock / 3e6) * ui32DelayUs);
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
    // Wait some time for the UART to send out the message.
    DelayUs(1e4);
}



// Initialize the hardware peripherals.
int UserHwInit(void)
{
    // Set up the system clock.
    g_ui32SysClock = MAP_SysCtlClockFreqSet(SYSTEM_CLOCK_SETTINGS, SYSTEM_CLOCK_FREQ);

    // Initialize the LEDs.
    GpioLedInit();
    // Switch on LED 0 to indicate activity.
    GpioLedSet(g_ui8Led = 0x1);

    // Initialize the UART on the BOOSTXL-RS232 BoosterPack on socket 1.
    g_sUart7.ui32UartClk = g_ui32SysClock;
    UartInit(&g_sUart7);

    return 0;
}



// Boot loader user menu.
int BL_UserMenu(uint32_t ui32UartBase)
{
    uint32_t ui32Cmd;
    char pcCmdStr[3];
    // Clear all pending characters from the UART.
    while (UARTCharsAvail(ui32UartBase)) {
        UARTCharGetNonBlocking(ui32UartBase);
    }
    // Wait for correct command.
    UARTprint(ui32UartBase, "\r\n\r\nBoot Loader Menu\r\n");
    UARTprint(ui32UartBase, "================\r\n");
    BL_UserMenuHelp(ui32UartBase);
    UARTprint(ui32UartBase, BL_COMMAND_PROMPT);
    while (1) {
        if ((ui32Cmd = UARTCharGetNonBlocking(ui32UartBase)) == -1) continue;
        usprintf(pcCmdStr, "%c", ui32Cmd & 0xff);
        UARTprint(ui32UartBase, pcCmdStr);
        UARTprint(ui32UartBase, "\r\n");
        switch (ui32Cmd) {
            case 'h':
            case 'H':
                BL_UserMenuHelp(ui32UartBase);
                break;
            case 'b':
            case 'B':
                UARTprint(ui32UartBase, "Booting the MCU main firmware.\r\n\r\n");
                // Wait some time for the UART to send out the message.
                DelayUs(1e4);
                return 0;
            case 'f':
            case 'F':
//                UARTprint(ui32UartBase, "Waiting for firmware data...\r\n");
                // Wait some time for the UART to send out the message.
                DelayUs(1e4);
                return 1;
            case 'r':
            case 'R':
                UARTprint(ui32UartBase, "Rebooting the MCU.\r\n");
                // Wait some time for the UART to send out the message.
                DelayUs(1e4);
                SysCtlReset();
            default:
                UARTprint(ui32UartBase, "Unknown command: ");
                UARTprint(ui32UartBase, pcCmdStr);
                UARTprint(ui32UartBase, "\r\n");
                BL_UserMenuHelp(ui32UartBase);
                break;
        }
        UARTprint(ui32UartBase, BL_COMMAND_PROMPT);
    }
    return 0;
}



// Boot loader user menu help.
int BL_UserMenuHelp(uint32_t ui32UartBase)
{
    UARTprint(ui32UartBase, "\r\nAvailable commands:\r\n");
    UARTprint(ui32UartBase, "h   Show this help text.\r\n");
    UARTprint(ui32UartBase, "b   Start normal boot process.\r\n");
    UARTprint(ui32UartBase, "f   Force MCU firmware download via the serial boot loader.\r\n");
    UARTprint(ui32UartBase, "r   Reboot the MCU.\r\n");
    // Wait some time for the UART to send out the message.
    DelayUs(1e4);

    return 0;
}

