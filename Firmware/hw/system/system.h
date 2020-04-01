// File: system.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 11 Feb 2020
// Rev.: 11 Feb 2020
//
// Header file for the system functions on the TI Tiva TM4C1294 connected
// LaunchPad evaluation kit.
//



#ifndef __SYSTEM_H__
#define __SYSTEM_H__



// Function prototypes.
void ClockOutputEnable(uint32_t ui32Div);
void ClockOutputDisable(void);



#endif  // __SYSTEM_H__

