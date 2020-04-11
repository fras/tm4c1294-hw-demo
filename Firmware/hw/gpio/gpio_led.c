// File: gpio_led.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 11 Apr 2020
//
// GPIO LED control for the TI Tiva TM4C1294 Connected LaunchPad Evaluation
// Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "gpio.h"
#include "gpio_led.h"



// GPIO definitions.
// D1: PN1
tGPIO sGpioLed1 = {
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};
// D1: PN0
tGPIO sGpioLed2 = {
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_0,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};
// D3: PF4
tGPIO sGpioLed3 = {
    SYSCTL_PERIPH_GPIOF,
    GPIO_PORTF_BASE,
    GPIO_PIN_4,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};
// D4: PF0
tGPIO sGpioLed4 = {
    SYSCTL_PERIPH_GPIOF,
    GPIO_PORTF_BASE,
    GPIO_PIN_0,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};



// Initialize the GPIO LEDs.
void GpioLedInit(void)
{
    // D1: PN1
    GpioInit(&sGpioLed1);
    // D2: PN0
    GpioInit(&sGpioLed2);
    // D3: PF4
    GpioInit(&sGpioLed3);
    // D4: PF0
    GpioInit(&sGpioLed4);
}



// Set the GPIO LEDs.
void GpioLedSet(uint32_t ui32Val)
{
    // D1: PN1
    GpioOutputSet(&sGpioLed1, (ui32Val & 0x01) << 1);
    // D2: PN0
    GpioOutputSet(&sGpioLed2, (ui32Val & 0x02) >> 1);
    // D3: PF4
    GpioOutputSet(&sGpioLed3, (ui32Val & 0x04) << 2);
    // D4: PF0
    GpioOutputSet(&sGpioLed4, (ui32Val & 0x08) >> 3);
}



// Read back the GPIO LEDs.
int32_t GpioLedGet(void)
{
    int ui32Val = 0;

    // D1: PN1
    ui32Val |= (GpioOutputGet(&sGpioLed1) & 0x02) >> 1;
    // D2: PN0
    ui32Val |= (GpioOutputGet(&sGpioLed2) & 0x01) << 1;
    // D3: PF4
    ui32Val |= (GpioOutputGet(&sGpioLed3) & 0x10) >> 2;
    // D4: PF0
    ui32Val |= (GpioOutputGet(&sGpioLed4) & 0x01) << 3;

    return ui32Val;
}

