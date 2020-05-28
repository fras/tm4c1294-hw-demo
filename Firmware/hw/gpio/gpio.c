// File: gpio.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 29 May 2020
//
// GPIO functions for the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "gpio.h"



// Initialize a GPIO pin.
void GpioInit(tGPIO *psGpio)
{
    // Enable the GPIO peripheral for the input.
    SysCtlPeripheralEnable(psGpio->ui32Peripheral);

    // Unlock a GPIO pin which had been previously locked. This is required for
    // pins that by default are locked against inadvertent reconfiguration.
    // These are e.g. pins used by the JTAG/SWD interface and any pin capable
    // of acting as an NMI input.
    GPIOUnlockPin(psGpio->ui32Port, psGpio->ui8Pins);

    // Set the direction as input.
    if (psGpio->bInput) GPIOPinTypeGPIOInput(psGpio->ui32Port, psGpio->ui8Pins);
    // Set the direction as output with open drain.
    else if (psGpio->ui32PinType == GPIO_PIN_TYPE_OD) GPIOPinTypeGPIOOutputOD(psGpio->ui32Port, psGpio->ui8Pins);
    // Set the direction as output.
    else GPIOPinTypeGPIOOutput(psGpio->ui32Port, psGpio->ui8Pins);

    // Enable the GPIO pins for digital function.
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



// Read a GPIO input or an open-drain output.
int32_t GpioInputGet(tGPIO *psGpio)
{
    int32_t i32GpioVal;

    // Temporarily switch the direction of an open-drain output to input.
    if (psGpio->ui32PinType == GPIO_PIN_TYPE_OD) GPIODirModeSet(psGpio->ui32Port, psGpio->ui8Pins, GPIO_DIR_MODE_IN);
    i32GpioVal = GPIOPinRead(psGpio->ui32Port, psGpio->ui8Pins);
    if (psGpio->ui32PinType == GPIO_PIN_TYPE_OD) GPIODirModeSet(psGpio->ui32Port, psGpio->ui8Pins, GPIO_DIR_MODE_OUT);

    return i32GpioVal;
}



// Read a GPIO input (boolean).
bool GpioInputGetBool(tGPIO *psGpio)
{
    if (GpioInputGet(psGpio) == psGpio->ui8Pins) {
        return true;
    } else {
        return false;
    }
}



// Set a GPIO output.
void GpioOutputSet(tGPIO *psGpio, uint8_t ui8Val)
{
    GPIOPinWrite(psGpio->ui32Port, psGpio->ui8Pins, ui8Val);
}



// Set a GPIO output (boolean).
void GpioOutputSetBool(tGPIO *psGpio, bool bVal)
{
    if (bVal) GpioOutputSet(psGpio, psGpio->ui8Pins);
    else GpioOutputSet(psGpio, 0);
}



// Read back a GPIO output.
int32_t GpioOutputGet(tGPIO *psGpio)
{
    return GPIOPinRead(psGpio->ui32Port, psGpio->ui8Pins);
}



// Read back a GPIO output (boolean).
bool GpioOutputGetBool(tGPIO *psGpio)
{
    if (GpioOutputGet(psGpio) == psGpio->ui8Pins) {
        return true;
    } else {
        return false;
    }
}

