// File: bl_user.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 28 Aug 2020
//
// Header file of the user functions of the boot loader running on the TI Tiva
// TM4C1294 Connected LaunchPad Evaluation Kit.
//



#ifndef __BL_USER_H__
#define __BL_USER_H__



// ******************************************************************
// Boot loader parameters.
// ******************************************************************

#define BL_NAME                     "boot loader"
#define BL_VERSION                  "0.0.3"
#define BL_RELEASEDATE              "28 Aug 2020"
// Timeout in seconds to enter the boot loader at startup.
#define BL_ACTIVATION_TIMEOUT       5
// Command prompt of the boot loader.
#define BL_COMMAND_PROMPT           "> "



// ******************************************************************
// System clock settings.
// ******************************************************************

// Use the internal 16-MHz precision oscillator of the MCU.
//#define SYSTEM_CLOCK_SETTINGS       (SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480)
// Use an external 25 MHz crystal or oscillator.
#define SYSTEM_CLOCK_SETTINGS       (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480)
#define SYSTEM_CLOCK_FREQ           120000000



// ******************************************************************
// Global variables.
// ******************************************************************

extern uint32_t g_ui32SysClock;
extern uint8_t g_ui8Led;



// ******************************************************************
// Function prototypes.
// ******************************************************************

void DelayUs(uint32_t ui32DelayUs);
void UARTprint(uint32_t ui32UartBase, const char* pcStr);
void UARTprintBlInfo(uint32_t ui32UartBase);
int UserHwInit(void);
int BL_UserMenu(uint32_t ui32UartBase);
int BL_UserMenuHelp(uint32_t ui32UartBase);



#endif  // __BL_USER_H__

