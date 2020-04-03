// File: gpio.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 19 Feb 2020
//
// GPIO functions for the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
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



// Initialize a GPIO pin.
void GpioInit(tGPIO *gpio)
{
    // Enable the GPIO peripheral for the input.
    SysCtlPeripheralEnable(gpio->ui32Peripheral);

    // Set the direction as input.
    if (gpio->bInput) GPIOPinTypeGPIOInput(gpio->ui32Port, gpio->ui8Pins);
    // Set the direction as output.
    else GPIOPinTypeGPIOOutput(gpio->ui32Port, gpio->ui8Pins);

    // End enable the GPIO pins for digital function.
    GPIOPadConfigSet(gpio->ui32Port, gpio->ui8Pins, gpio->ui32Strength, gpio->ui32PinType);
}



// Initialize a GPIO input with an interrupt. Initialize
void GpioInitIntr(tGPIO *gpio, void (*pfnIntHandler)(void))
{
    // Initialize the GPIO input.
    GpioInit(gpio);

    // Register and enable interrupt function.
    GPIOIntTypeSet(gpio->ui32Port, gpio->ui8Pins, gpio->ui32IntType);
    GPIOIntRegister(gpio->ui32Port, pfnIntHandler);
    GPIOIntEnable(gpio->ui32Port, gpio->ui8Pins);
}



// Read a GPIO input.
int32_t GpioInputGet(tGPIO *gpio)
{
    return GPIOPinRead(gpio->ui32Port, gpio->ui8Pins);
}



// Set a GPIO output.
void GpioOutputSet(tGPIO *gpio, uint8_t ui8Val)
{
    GPIOPinWrite(gpio->ui32Port, gpio->ui8Pins, ui8Val);
}



// Read back a GPIO output.
int32_t GpioOutputGet(tGPIO *gpio)
{
    return GPIOPinRead(gpio->ui32Port, gpio->ui8Pins);
}

