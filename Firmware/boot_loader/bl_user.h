// File: bl_user.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 26 Aug 2020
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
#define BL_VERSION                  "0.0.1"
#define BL_RELEASEDATE              "26 Aug 2020"



// ******************************************************************
// Function prototypes.
// ******************************************************************
void DelayUs(uint32_t ui32DelayUs);
void UARTprint(uint32_t ui32UartBase, const char* pcStr);
void UARTprintBlInfo(uint32_t ui32UartBase);



#endif  // __BL_USER_H__

