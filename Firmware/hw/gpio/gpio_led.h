// File: gpio_led.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 19 Feb 2020
//
// Header file for the GPIO LED control for the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#ifndef __GPIO_LED_H__
#define __GPIO_LED_H__



// Function prototypes.
void GpioLedInit(void);
void GpioLedSet(uint32_t val);
int32_t GpioLedGet(void);



#endif  // __GPIO_LED_H__

