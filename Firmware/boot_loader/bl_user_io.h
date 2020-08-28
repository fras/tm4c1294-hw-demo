// File: bl_user_io.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 27 Aug 2020
// Rev.: 27 Aug 2020
//
// Header file for the IO peripheral definitions of the boot loader running on
// the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#ifndef __BL_USER_IO_H__
#define __BL_USER_IO_H__



#include "hw/uart/uart.h"



// ******************************************************************
// Global variables for the IO peripherals.
// ******************************************************************

// UARTs.
extern tUART g_sUart7;



#endif  // __BL_USER_IO_H__

