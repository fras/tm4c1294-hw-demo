// File: pwm_rgb_led.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 04 Apr 2020
//
// Header file for the RGB LED control using PWD functions on the TI Tiva
// TM4C1294 Connected LaunchPad Evaluation Kit.
//



#ifndef __PWM_RGB_LED_H__
#define __PWM_RGB_LED_H__



// Dim down the overall brightness of the RGB LED by increasing this number.
#define PWM_RGB_LED_PERIOD 5000



// Function prototypes.
void PwmRgbLedInit(void);
void PwmRgbLedSet(int iR, int iG, int iB);



#endif  // __PWM_RGB_LED_H__

