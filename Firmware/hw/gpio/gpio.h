// File: gpio.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 19 Feb 2020
//
// Header file for the GPIO functions for the TI Tiva TM4C1294 connected
// LaunchPad evaluation kit.
//



#ifndef __GPIO_H__
#define __GPIO_H__



// Types.
typedef struct {
    uint32_t ui32Peripheral;
    uint32_t ui32Port;
    uint8_t  ui8Pins;
    uint32_t ui32Strength;
    uint32_t ui32PinType;
    bool     bInput;            // false = output, true = input
    uint32_t ui32IntType;
} tGPIO;



// Function prototypes.
void GpioInit(tGPIO *gpio);
void GpioInitIntr(tGPIO *gpio, void (*pfnIntHandler)(void));
int32_t GpioInputGet(tGPIO *gpio);
void GpioOutputSet(tGPIO *gpio, uint8_t ui8Val);
int32_t GpioOutputGet(tGPIO *gpio);



#endif  // __GPIO_H__

