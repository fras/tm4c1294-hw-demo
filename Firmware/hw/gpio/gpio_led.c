// File: gpio_led.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 19 Feb 2020
//
// GPIO LED control for the TI Tiva TM4C1294 Connected LaunchPad Evaluation
// Kit.
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
#include "gpio.h"
#include "gpio_led.h"



// GPIO definitions.
// D1: PN1
tGPIO gpioLed1 = {
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_1,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};
// D1: PN0
tGPIO gpioLed2 = {
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_0,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};
// D3: PF4
tGPIO gpioLed3 = {
    SYSCTL_PERIPH_GPIOF,
    GPIO_PORTF_BASE,
    GPIO_PIN_4,
    GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD,
    false,
    0
};
// D4: PF0
tGPIO gpioLed4 = {
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
    GpioInit(&gpioLed1);
    // D2: PN0
    GpioInit(&gpioLed2);
    // D3: PF4
    GpioInit(&gpioLed3);
    // D4: PF0
    GpioInit(&gpioLed4);
}



// Set the GPIO LEDs.
void GpioLedSet(uint32_t val)
{
    // D1: PN1
    GpioOutputSet(&gpioLed1, (val & 0x01) << 1);
    // D2: PN0
    GpioOutputSet(&gpioLed2, (val & 0x02) >> 1);
    // D3: PF4
    GpioOutputSet(&gpioLed3, (val & 0x04) << 2);
    // D4: PF0
    GpioOutputSet(&gpioLed4, (val & 0x08) >> 3);
}



// Read back the GPIO LEDs.
int32_t GpioLedGet(void)
{
    int val = 0;

    // D1: PN1
    val |= (GpioOutputGet(&gpioLed1) & 0x02) >> 1;
    // D2: PN0
    val |= (GpioOutputGet(&gpioLed2) & 0x01) << 1;
    // D3: PF4
    val |= (GpioOutputGet(&gpioLed3) & 0x10) >> 2;
    // D4: PF0
    val |= (GpioOutputGet(&gpioLed4) & 0x01) << 3;

    return val;
}

