// File: gpio.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 04 Apr 2020
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
void GpioInit(tGPIO *psGpio)
{
    // Enable the GPIO peripheral for the input.
    SysCtlPeripheralEnable(psGpio->ui32Peripheral);

    // Set the direction as input.
    if (psGpio->bInput) GPIOPinTypeGPIOInput(psGpio->ui32Port, psGpio->ui8Pins);
    // Set the direction as output.
    else GPIOPinTypeGPIOOutput(psGpio->ui32Port, psGpio->ui8Pins);

    // End enable the GPIO pins for digital function.
    GPIOPadConfigSet(psGpio->ui32Port, psGpio->ui8Pins, psGpio->ui32Strength, psGpio->ui32PinType);
}



// Initialize a GPIO input with an interrupt. Initialize
void GpioInitIntr(tGPIO *psGpio, void (*pfnIntHandler)(void))
{
    // Initialize the GPIO input.
    GpioInit(psGpio);

    // Register and enable interrupt function.
    GPIOIntTypeSet(psGpio->ui32Port, psGpio->ui8Pins, psGpio->ui32IntType);
    GPIOIntRegister(psGpio->ui32Port, pfnIntHandler);
    GPIOIntEnable(psGpio->ui32Port, psGpio->ui8Pins);
}



// Read a GPIO input.
int32_t GpioInputGet(tGPIO *psGpio)
{
    return GPIOPinRead(psGpio->ui32Port, psGpio->ui8Pins);
}



// Set a GPIO output.
void GpioOutputSet(tGPIO *psGpio, uint8_t ui8Val)
{
    GPIOPinWrite(psGpio->ui32Port, psGpio->ui8Pins, ui8Val);
}



// Read back a GPIO output.
int32_t GpioOutputGet(tGPIO *psGpio)
{
    return GPIOPinRead(psGpio->ui32Port, psGpio->ui8Pins);
}

