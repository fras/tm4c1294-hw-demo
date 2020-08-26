// File: bl_userhooks.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 26 Aug 2020
//
// Header file of the user hook functions of the boot loader running on the TI
// Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#ifndef __BL_USERHOOK_H__
#define __BL_USERHOOK_H__



// ******************************************************************
// System clock settings.
// ******************************************************************
// Use the internal 16-MHz precision oscillator of the MCU.
//#define SYSTEM_CLOCK_SETTINGS       (SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480)
// Use an external 25 MHz crystal or oscillator.
#define SYSTEM_CLOCK_SETTINGS       (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480)
#define SYSTEM_CLOCK_FREQ           120000000



// ******************************************************************
// Function prototypes.
// ******************************************************************
void BL_UserHwInit(void);
void BL_Reinit(void);
void BL_FwDownloadStart(void);
void BL_FwDownloadProgress(void);
void BL_FwDownloadEnd(void);
unsigned long BL_UserCheckUpdateHook(void);



#endif  // __BL_USERHOOK_H__

