// File: bl_userhooks.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 28 Aug 2020
//
// User hook functions of the boot loader running on the TI Tiva TM4C1294
// Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/ustdlib.h"
#include "hw/gpio/gpio.h"
#include "hw/gpio/gpio_led.h"
#include "bl_config.h"
#include "bl_user.h"
#include "bl_userhooks.h"



// A prototype for the function (in the startup code) for a predictable length
// delay.
extern void Delay(uint32_t ui32Count);



// Performs application-specific low level hardware initialization on system
// reset.
void BL_UserHwInit(void)
{
    // Initialize the hardware peripherals.
    UserHwInit();
}



// Performs application-specific reinitialization on boot loader entry via SVC.
void BL_Reinit(void)
{
    // Re-initialize the hardware peripherals.
    UserHwInit();

    // Show boot loader info.
    UARTprintBlInfo(UARTx_BASE);

    UARTprint(UARTx_BASE, "\r\nWaiting for firmware data...\r\n");
}



// Informs an application that a download is starting.
void BL_FwDownloadStart(void)
{
    // Switch on LED 0 to indicate activity.
    GpioLedSet(g_ui8Led = 0x1);
}



// Informs an application of download progress.
void BL_FwDownloadProgress(void)
{
    // Counting LEDs to indicate activity.
    g_ui8Led &= 0xe;
    g_ui8Led += 2;
    GpioLedSet(g_ui8Led = 0x1 | g_ui8Led);
}



// Informs an application that a download has completed.
void BL_FwDownloadEnd(void)
{
    // Blink all LEDs to indicate the end of the firmware download.
    for (int i = 0; i < 4; i++) {
        GpioLedSet(g_ui8Led = 0xf);
        DelayUs(5e5);
        GpioLedSet(g_ui8Led = 0x0);
        DelayUs(5e5);
    }
}



// Enable a new firmware download at system start up.
unsigned long BL_UserCheckUpdateHook(void)
{
    // Show boot loader info.
    UARTprintBlInfo(UARTx_BASE);

    // Clear all pending characters from the UART to avoid false activation of
    // the boot loader menu.
    while (UARTCharsAvail(UARTx_BASE)) {
        UARTCharGetNonBlocking(UARTx_BASE);
    }
    // Wait for any charater to enter the boot loader menu.
    UARTprint(UARTx_BASE, "\r\nPress any key to enter the boot loader menu.\r\n");
    for (int i = BL_ACTIVATION_TIMEOUT; i >= 0; i--) {
        char pcTimeoutStr[10];
        usprintf(pcTimeoutStr, "%d ", i);
        UARTprint(UARTx_BASE, pcTimeoutStr);
        // Blink the LED 0 with 1 second period.
        DelayUs(5e5);
        GpioLedSet(g_ui8Led &= ~0x1);
        // Character received on the UART.
        if (UARTCharsAvail(UARTx_BASE)) break;
        DelayUs(5e5);
        GpioLedSet(g_ui8Led |= 0x1);
        // Character received on the UART.
        if (UARTCharsAvail(UARTx_BASE)) break;
    }
    // Enter the boot loader menu.
    if (UARTCharsAvail(UARTx_BASE)) {
        // A return value of 1 can freeze the boot loader. The reason is unknown.
        //return BL_UserMenu(UARTx_BASE);

        // Work-around: Use code copied from the EK-TM4C1294XL boot_demo1 example.
        if (BL_UserMenu(UARTx_BASE)) {
            // We must make sure we turn off SysTick and its interrupt before entering 
            // the boot loader!
            MAP_SysTickIntDisable(); 
            MAP_SysTickDisable(); 

            // Disable all processor interrupts.  Instead of disabling them
            // one at a time, a direct write to NVIC is done to disable all
            // peripheral interrupts.
            HWREG(NVIC_DIS0) = 0xffffffff;
            HWREG(NVIC_DIS1) = 0xffffffff;
            HWREG(NVIC_DIS2) = 0xffffffff;
            HWREG(NVIC_DIS3) = 0xffffffff;

            // Return control to the boot loader.  This is a call to the SVC
            // handler in the boot loader.
            (*((void (*)(void))(*(uint32_t *)0x2c)))();
        }
    }

    // Turn off all LEDs.
    GpioLedSet(g_ui8Led = 0x0);

    return 0;
}

