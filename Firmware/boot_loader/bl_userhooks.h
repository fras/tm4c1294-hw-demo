// File: bl_userhooks.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 26 Aug 2020
// Rev.: 27 Aug 2020
//
// Header file of the user hook functions of the boot loader running on the TI
// Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#ifndef __BL_USERHOOK_H__
#define __BL_USERHOOK_H__



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

