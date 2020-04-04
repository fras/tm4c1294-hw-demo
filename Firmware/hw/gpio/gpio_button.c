// File: gpio_button.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 04 Apr 2020
//
// GPIO button functions for the TI Tiva TM4C1294 Connected LaunchPad
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
#include "gpio.h"
#include "gpio_button.h"



// GPIO definitions.
// USR_SW1: PJ0
tGPIO sGpioUsrSw1 = {
    SYSCTL_PERIPH_GPIOJ,
    GPIO_PORTJ_BASE,
    GPIO_PIN_0,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};
// USR_SW2: PJ1
tGPIO sGpioUsrSw2 = {
    SYSCTL_PERIPH_GPIOJ,
    GPIO_PORTJ_BASE,
    GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};
// Educational BoosterPack MK II S1: PH1
tGPIO sGpioEduBoosterS1 = {
    SYSCTL_PERIPH_GPIOH,
    GPIO_PORTH_BASE,
    GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};
// Educational BoosterPack MK II S2: PK6
tGPIO sGpioEduBoosterS2 = {
    SYSCTL_PERIPH_GPIOK,
    GPIO_PORTK_BASE,
    GPIO_PIN_6,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};



// Initialize the GPIO buttons.
void GpioButtonInit(void)
{
    // USR_SW1: PJ0
    GpioInitIntr(&sGpioUsrSw1, GpioButtonIntHandler);
    // USR_SW2: PJ1
    GpioInitIntr(&sGpioUsrSw2, GpioButtonIntHandler);
    // Educational BoosterPack MK II S1: PH1
    GpioInitIntr(&sGpioEduBoosterS1, GpioButtonIntHandler);
    // Educational BoosterPack MK II S2: PK6
    GpioInitIntr(&sGpioEduBoosterS2, GpioButtonIntHandler);
}



// Get the GPIO button status.
int32_t GpioButtonGet(void)
{
    return 0;
}



// Interrupt handler for buttons.
void GpioButtonIntHandler(void)
{
    uint32_t intStatusPortUsrSw1 = 0;
    uint32_t intStatusPortUsrSw2 = 0;
    uint32_t intStatusPortEduBoosterS1 = 0;
    uint32_t intStatusPortEduBoosterS2 = 0;

    intStatusPortUsrSw1 = GPIOIntStatus(sGpioUsrSw1.ui32Port, true);
    GPIOIntClear(sGpioUsrSw1.ui32Port, intStatusPortUsrSw1);
//    intStatusPortUsrSw2 = GPIOIntStatus(sGpioUsrSw2.ui32Port, true);
//    GPIOIntClear(sGpioUsrSw2.ui32Port, intStatusPortUsrSw2);
    intStatusPortUsrSw2 = intStatusPortUsrSw1;  // Both buttons are on port J.
    intStatusPortEduBoosterS1 = GPIOIntStatus(sGpioEduBoosterS1.ui32Port, true);
    GPIOIntClear(sGpioEduBoosterS1.ui32Port, intStatusPortEduBoosterS1);
    intStatusPortEduBoosterS2 = GPIOIntStatus(sGpioEduBoosterS2.ui32Port, true);
    GPIOIntClear(sGpioEduBoosterS2.ui32Port, intStatusPortEduBoosterS2);

    // USR_SW1: PJ0
    if ((intStatusPortUsrSw1 & sGpioUsrSw1.ui8Pins) == sGpioUsrSw1.ui8Pins) {
        if (GpioInputGet(&sGpioUsrSw1) == sGpioUsrSw1.ui8Pins)
            UARTprintf("USR_SW1 released.\n");
        else
            UARTprintf("USR_SW1 pressed.\n");
    }
    // USR_SW2: PJ1
    if ((intStatusPortUsrSw2 & sGpioUsrSw2.ui8Pins) == sGpioUsrSw2.ui8Pins) {
        if (GpioInputGet(&sGpioUsrSw2) == sGpioUsrSw2.ui8Pins)
            UARTprintf("USR_SW2 released.\n");
        else
            UARTprintf("USR_SW2 pressed.\n");
    }
    // Educational BoosterPack MK II S1: PH1
    if ((intStatusPortEduBoosterS1 & sGpioEduBoosterS1.ui8Pins) == sGpioEduBoosterS1.ui8Pins) {
        if (GpioInputGet(&sGpioEduBoosterS1) == sGpioEduBoosterS1.ui8Pins)
            UARTprintf("Educational BoosterPack MK II S1 released.\n");
        else
            UARTprintf("Educational BoosterPack MK II S1 pressed.\n");
    }
    // Educational BoosterPack MK II S2: PK6
    if ((intStatusPortEduBoosterS2 & sGpioEduBoosterS2.ui8Pins) == sGpioEduBoosterS2.ui8Pins) {
        if (GpioInputGet(&sGpioEduBoosterS2) == sGpioEduBoosterS2.ui8Pins)
            UARTprintf("Educational BoosterPack MK II S2 released.\n");
        else
            UARTprintf("Educational BoosterPack MK II S2 pressed.\n");
    }
}

