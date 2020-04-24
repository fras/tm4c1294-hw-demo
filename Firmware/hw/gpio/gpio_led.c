// File: gpio_led.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 24 Apr 2020
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
tGPIO g_sGpioLed1 = {
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_1,             // ui8Pins
    GPIO_STRENGTH_2MA,      // ui32Strength
    GPIO_PIN_TYPE_STD,      // ui32PinType
    false,                  // bInput: false = output, true = input
    0                       // ui32IntType
};
// D1: PN0
tGPIO g_sGpioLed2 = {
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_0,             // ui8Pins
    GPIO_STRENGTH_2MA,      // ui32Strength
    GPIO_PIN_TYPE_STD,      // ui32PinType
    false,                  // bInput: false = output, true = input
    0                       // ui32IntType
};
// D3: PF4
tGPIO g_sGpioLed3 = {
    SYSCTL_PERIPH_GPIOF,
    GPIO_PORTF_BASE,
    GPIO_PIN_4,             // ui8Pins
    GPIO_STRENGTH_2MA,      // ui32Strength
    GPIO_PIN_TYPE_STD,      // ui32PinType
    false,                  // bInput: false = output, true = input
    0                       // ui32IntType
};
// D4: PF0
tGPIO g_sGpioLed4 = {
    SYSCTL_PERIPH_GPIOF,
    GPIO_PORTF_BASE,
    GPIO_PIN_0,             // ui8Pins
    GPIO_STRENGTH_2MA,      // ui32Strength
    GPIO_PIN_TYPE_STD,      // ui32PinType
    false,                  // bInput: false = output, true = input
    0                       // ui32IntType
};



// Initialize the GPIO LEDs.
void GpioLedInit(void)
{
    GpioInit(&g_sGpioLed1);
    GpioInit(&g_sGpioLed2);
    GpioInit(&g_sGpioLed3);
    GpioInit(&g_sGpioLed4);
}



// Set the GPIO LEDs.
void GpioLedSet(uint32_t ui32Val)
{
    GpioOutputSetBool(&g_sGpioLed1, (bool) (ui32Val & 0x01));
    GpioOutputSetBool(&g_sGpioLed2, (bool) (ui32Val & 0x02));
    GpioOutputSetBool(&g_sGpioLed3, (bool) (ui32Val & 0x04));
    GpioOutputSetBool(&g_sGpioLed4, (bool) (ui32Val & 0x08));
}



// Read back the GPIO LEDs.
uint32_t GpioLedGet(void)
{
    uint32_t ui32Val = 0;

    ui32Val |= (GpioOutputGetBool(&g_sGpioLed1) & 0x1) << 0;
    ui32Val |= (GpioOutputGetBool(&g_sGpioLed2) & 0x1) << 1;
    ui32Val |= (GpioOutputGetBool(&g_sGpioLed3) & 0x1) << 2;
    ui32Val |= (GpioOutputGetBool(&g_sGpioLed4) & 0x1) << 3;

    return ui32Val;
}

