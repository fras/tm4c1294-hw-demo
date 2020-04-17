// File: gpio_button.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 17 Apr 2020
//
// Header file for the GPIO button functions for the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#ifndef __GPIO_BUTTON_H__
#define __GPIO_BUTTON_H__



// Button parameters.
#define GPIO_BUTTON_USR_SW1     0
#define GPIO_BUTTON_USR_SW2     1
#define GPIO_BUTTON_EDU_S1      2
#define GPIO_BUTTON_EDU_S2      3
#define GPIO_BUTTON_NUM         4
#define GPIO_BUTTON_RELEASED    0
#define GPIO_BUTTON_PRESSED     1
// Print message, when a button is pressed or released.
//#define GPIO_BUTTON_MESSAGE_USR_SW1
//#define GPIO_BUTTON_MESSAGE_USR_SW2
//#define GPIO_BUTTON_MESSAGE_EDU_S1
//#define GPIO_BUTTON_MESSAGE_EDU_S2



// Global variables holding the button status.
extern uint8_t g_ui8GpioButtonStatus[GPIO_BUTTON_NUM];
extern uint32_t g_ui32GpioButtonPressedCnt[GPIO_BUTTON_NUM];
extern uint32_t g_ui32GpioButtonReleasedCnt[GPIO_BUTTON_NUM];



// Function prototypes.
void GpioButtonInit(void);
int32_t GpioButtonGet(void);
void GpioButtonIntHandler(void);



#endif  // __GPIO_BUTTON_H__

