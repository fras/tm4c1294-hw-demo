// File: gpio_button.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 19 Feb 2020
//
// Header file for the GPIO button functions for the TI Tiva TM4C1294 connected
// LaunchPad evaluation kit.
//



#ifndef __GPIO_BUTTON_H__
#define __GPIO_BUTTON_H__



// Function prototypes.
void GpioButtonInit(void);
int32_t GpioButtonGet(void);
void GpioButtonIntHandler(void);



#endif  // __GPIO_BUTTON_H__

