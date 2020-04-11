// File: pwm_rgb_led.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 10 Feb 2020
// Rev.: 11 Apr 2020
//
// RGB LED control using PWM functions on the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "pwm_rgb_led.h"



// Initialize the PWM pins for the RGB LED on the Educational BoosterPack MK II.
void PwmRgbLedInit(void)
{

    // Red channel: PWM pin M0PWM6 (GPIO PK4).
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    GPIOPinConfigure(GPIO_PK4_M0PWM6);
    GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
    PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_1);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWM_RGB_LED_PERIOD);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 1);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

    // Green channel: PWM pin M0PWM7 (GPIO PK5).
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    GPIOPinConfigure(GPIO_PK5_M0PWM7);
    GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_5);
    PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_1);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWM_RGB_LED_PERIOD);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 1);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);

    // Blue channel: Timer pin T2CCP0 (GPIO PM0).
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    GPIOPinConfigure(GPIO_PM0_T2CCP0);
    GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_0);
    TimerClockSourceSet(GPIO_PORTM_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER2_BASE, TIMER_A, PWM_RGB_LED_PERIOD);
    TimerControlLevel(TIMER2_BASE, TIMER_A, false);
    TimerEnable(TIMER2_BASE, TIMER_A);
    TimerMatchSet(TIMER2_BASE, TIMER_A, PWM_RGB_LED_PERIOD - 1);

    // Set the initial RGB value.
    PwmRgbLedSet(0, 0, 0);
}



// Coerce a RGB value.
int PwmRgbLedCoerce(int iRgb)
{
    int iVal = iRgb;
    
    if (iVal < 1) iVal = 1;
    else if (iVal > 255) iVal = 255;

    return iVal;
}



// Set the RGB LED value.
void PwmRgbLedSet(int iR, int iG, int iB)
{
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PwmRgbLedCoerce(iR));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PwmRgbLedCoerce(iG));
    TimerMatchSet(TIMER2_BASE, TIMER_A, PWM_RGB_LED_PERIOD - PwmRgbLedCoerce(iB));
}

