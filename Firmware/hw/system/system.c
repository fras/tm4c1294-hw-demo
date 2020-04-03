// File: system.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 11 Feb 2020
// Rev.: 11 Feb 2020
//
// System functions on the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
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
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "pwm_rgb_led.h"



// Setup the DIVSCLK pin (PQ4) to drive the system clock.
// CAUTION: When enabled at boot up, this will cause further flashing to fail!
//          When enabling it at run time, it will cause the MCU to reset.
//          For details see here:
//          https://e2e.ti.com/support/microcontrollers/other/f/908/t/345762
// REMEDY:  Press the RESET button on the Tiva TM4C1294 Connected LaunchPad
//          Evaluation Kit while flashing. Then power cycle the board and flash
//          again.
void ClockOutputEnable(uint32_t ui32Div)
{
    // Setup the DIVSCLK pin (PQ4) to drive the system clock.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    GPIOPadConfigSet(GPIO_PORTQ_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PQ4_DIVSCLK);
    GPIODirModeSet(GPIO_PORTQ_BASE, GPIO_PIN_4, GPIO_DIR_MODE_HW);
    SysCtlClockOutConfig(SYSCTL_CLKOUT_EN | SYSCTL_CLKOUT_SYSCLK, ui32Div);
}



// Disable the system clock output on the DIVSCLK pin (PQ4).
void ClockOutputDisable(void)
{
    SysCtlClockOutConfig(SYSCTL_CLKOUT_DIS, 1);
}

