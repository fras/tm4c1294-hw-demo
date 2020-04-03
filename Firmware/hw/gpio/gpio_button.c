// File: gpio_button.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 19 Feb 2020
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
tGPIO gpioUsrSw1 = {
    SYSCTL_PERIPH_GPIOJ,
    GPIO_PORTJ_BASE,
    GPIO_PIN_0,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};
// USR_SW2: PJ1
tGPIO gpioUsrSw2 = {
    SYSCTL_PERIPH_GPIOJ,
    GPIO_PORTJ_BASE,
    GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};
// Educational BoosterPack MK II S1: PH1
tGPIO gpioEduBoosterS1 = {
    SYSCTL_PERIPH_GPIOH,
    GPIO_PORTH_BASE,
    GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU,
    true,
    GPIO_BOTH_EDGES
};
// Educational BoosterPack MK II S2: PK6
tGPIO gpioEduBoosterS2 = {
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
    GpioInitIntr(&gpioUsrSw1, GpioButtonIntHandler);
    // USR_SW2: PJ1
    GpioInitIntr(&gpioUsrSw2, GpioButtonIntHandler);
    // Educational BoosterPack MK II S1: PH1
    GpioInitIntr(&gpioEduBoosterS1, GpioButtonIntHandler);
    // Educational BoosterPack MK II S2: PK6
    GpioInitIntr(&gpioEduBoosterS2, GpioButtonIntHandler);
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

    intStatusPortUsrSw1 = GPIOIntStatus(gpioUsrSw1.ui32Port, true);
    GPIOIntClear(gpioUsrSw1.ui32Port, intStatusPortUsrSw1);
//    intStatusPortUsrSw2 = GPIOIntStatus(gpioUsrSw2.ui32Port, true);
//    GPIOIntClear(gpioUsrSw2.ui32Port, intStatusPortUsrSw2);
    intStatusPortUsrSw2 = intStatusPortUsrSw1;  // Both buttons are on port J.
    intStatusPortEduBoosterS1 = GPIOIntStatus(gpioEduBoosterS1.ui32Port, true);
    GPIOIntClear(gpioEduBoosterS1.ui32Port, intStatusPortEduBoosterS1);
    intStatusPortEduBoosterS2 = GPIOIntStatus(gpioEduBoosterS2.ui32Port, true);
    GPIOIntClear(gpioEduBoosterS2.ui32Port, intStatusPortEduBoosterS2);

    // USR_SW1: PJ0
    if ((intStatusPortUsrSw1 & gpioUsrSw1.ui8Pins) == gpioUsrSw1.ui8Pins) {
        if (GpioInputGet(&gpioUsrSw1) == gpioUsrSw1.ui8Pins)
            UARTprintf("USR_SW1 released.\n");
        else
            UARTprintf("USR_SW1 pressed.\n");
    }
    // USR_SW2: PJ1
    if ((intStatusPortUsrSw2 & gpioUsrSw2.ui8Pins) == gpioUsrSw2.ui8Pins) {
        if (GpioInputGet(&gpioUsrSw2) == gpioUsrSw2.ui8Pins)
            UARTprintf("USR_SW2 released.\n");
        else
            UARTprintf("USR_SW2 pressed.\n");
    }
    // Educational BoosterPack MK II S1: PH1
    if ((intStatusPortEduBoosterS1 & gpioEduBoosterS1.ui8Pins) == gpioEduBoosterS1.ui8Pins) {
        if (GpioInputGet(&gpioEduBoosterS1) == gpioEduBoosterS1.ui8Pins)
            UARTprintf("Educational BoosterPack MK II S1 released.\n");
        else
            UARTprintf("Educational BoosterPack MK II S1 pressed.\n");
    }
    // Educational BoosterPack MK II S2: PK6
    if ((intStatusPortEduBoosterS2 & gpioEduBoosterS2.ui8Pins) == gpioEduBoosterS2.ui8Pins) {
        if (GpioInputGet(&gpioEduBoosterS2) == gpioEduBoosterS2.ui8Pins)
            UARTprintf("Educational BoosterPack MK II S2 released.\n");
        else
            UARTprintf("Educational BoosterPack MK II S2 pressed.\n");
    }
}

