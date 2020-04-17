// File: gpio_button.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 17 Apr 2020
//
// GPIO button functions for the TI Tiva TM4C1294 Connected LaunchPad
// Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "gpio.h"
#include "gpio_button.h"



// Global variables holding the button status.
uint8_t g_ui8GpioButtonStatus[GPIO_BUTTON_NUM];
uint32_t g_ui32GpioButtonPressedCnt[GPIO_BUTTON_NUM];
uint32_t g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_NUM];



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
    g_ui8GpioButtonStatus[GPIO_BUTTON_USR_SW1] = GPIO_BUTTON_RELEASED;
    g_ui32GpioButtonPressedCnt[GPIO_BUTTON_USR_SW1] = 0;
    g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_USR_SW1] = 0;
    // USR_SW2: PJ1
    GpioInitIntr(&sGpioUsrSw2, GpioButtonIntHandler);
    g_ui8GpioButtonStatus[GPIO_BUTTON_USR_SW2] = GPIO_BUTTON_RELEASED;
    g_ui32GpioButtonPressedCnt[GPIO_BUTTON_USR_SW2] = 0;
    g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_USR_SW2] = 0;
    // Educational BoosterPack MK II S1: PH1
    GpioInitIntr(&sGpioEduBoosterS1, GpioButtonIntHandler);
    g_ui8GpioButtonStatus[GPIO_BUTTON_EDU_S1] = GPIO_BUTTON_RELEASED;
    g_ui32GpioButtonPressedCnt[GPIO_BUTTON_EDU_S1] = 0;
    g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_EDU_S1] = 0;
    // Educational BoosterPack MK II S2: PK6
    GpioInitIntr(&sGpioEduBoosterS2, GpioButtonIntHandler);
    g_ui8GpioButtonStatus[GPIO_BUTTON_EDU_S2] = GPIO_BUTTON_RELEASED;
    g_ui32GpioButtonPressedCnt[GPIO_BUTTON_EDU_S2] = 0;
    g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_EDU_S2] = 0;
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
        if (GpioInputGet(&sGpioUsrSw1) == sGpioUsrSw1.ui8Pins) {
            g_ui8GpioButtonStatus[GPIO_BUTTON_USR_SW1] = GPIO_BUTTON_RELEASED;
            g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_USR_SW1]++;
            #ifdef GPIO_BUTTON_MESSAGE_USR_SW1
            UARTprintf("USR_SW1 released.\n");
            #endif
        } else {
            g_ui8GpioButtonStatus[GPIO_BUTTON_USR_SW1] = GPIO_BUTTON_PRESSED;
            g_ui32GpioButtonPressedCnt[GPIO_BUTTON_USR_SW1]++;
            #ifdef GPIO_BUTTON_MESSAGE_USR_SW1
            UARTprintf("USR_SW1 pressed.\n");
            #endif
        }
    }
    // USR_SW2: PJ1
    if ((intStatusPortUsrSw2 & sGpioUsrSw2.ui8Pins) == sGpioUsrSw2.ui8Pins) {
        if (GpioInputGet(&sGpioUsrSw2) == sGpioUsrSw2.ui8Pins) {
            g_ui8GpioButtonStatus[GPIO_BUTTON_USR_SW2] = GPIO_BUTTON_RELEASED;
            g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_USR_SW2]++;
            #ifdef GPIO_BUTTON_MESSAGE_USR_SW2
            UARTprintf("USR_SW2 released.\n");
            #endif
        } else {
            g_ui8GpioButtonStatus[GPIO_BUTTON_USR_SW2] = GPIO_BUTTON_PRESSED;
            g_ui32GpioButtonPressedCnt[GPIO_BUTTON_USR_SW2]++;
            #ifdef GPIO_BUTTON_MESSAGE_USR_SW2
            UARTprintf("USR_SW2 pressed.\n");
            #endif
        }
    }
    // Educational BoosterPack MK II S1: PH1
    if ((intStatusPortEduBoosterS1 & sGpioEduBoosterS1.ui8Pins) == sGpioEduBoosterS1.ui8Pins) {
        if (GpioInputGet(&sGpioEduBoosterS1) == sGpioEduBoosterS1.ui8Pins) {
            g_ui8GpioButtonStatus[GPIO_BUTTON_EDU_S1] = GPIO_BUTTON_RELEASED;
            g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_EDU_S1]++;
            #ifdef GPIO_BUTTON_MESSAGE_EDU_S1
            UARTprintf("Educational BoosterPack MK II S1 released.\n");
            #endif
        } else {
            g_ui8GpioButtonStatus[GPIO_BUTTON_EDU_S1] = GPIO_BUTTON_PRESSED;
            g_ui32GpioButtonPressedCnt[GPIO_BUTTON_EDU_S1]++;
            #ifdef GPIO_BUTTON_MESSAGE_EDU_S1
            UARTprintf("Educational BoosterPack MK II S1 pressed.\n");
            #endif
        }
    }
    // Educational BoosterPack MK II S2: PK6
    if ((intStatusPortEduBoosterS2 & sGpioEduBoosterS2.ui8Pins) == sGpioEduBoosterS2.ui8Pins) {
        if (GpioInputGet(&sGpioEduBoosterS2) == sGpioEduBoosterS2.ui8Pins) {
            g_ui8GpioButtonStatus[GPIO_BUTTON_EDU_S2] = GPIO_BUTTON_RELEASED;
            g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_EDU_S2]++;
            #ifdef GPIO_BUTTON_MESSAGE_EDU_S2
            UARTprintf("Educational BoosterPack MK II S2 released.\n");
            #endif
        } else {
            g_ui8GpioButtonStatus[GPIO_BUTTON_EDU_S2] = GPIO_BUTTON_PRESSED;
            g_ui32GpioButtonPressedCnt[GPIO_BUTTON_EDU_S2]++;
            #ifdef GPIO_BUTTON_MESSAGE_EDU_S2
            UARTprintf("Educational BoosterPack MK II S2 pressed.\n");
            #endif
        }
    }
}

