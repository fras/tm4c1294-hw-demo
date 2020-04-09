// File: hw_demo_io.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 09 Apr 2020
// Rev.: 09 Apr 2020
//
// Header file for the IO peripheral definitions of the hardware demo for the
// TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//




#ifndef __HW_DEMO_H_IO__
#define __HW_DEMO_H_IO__



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "hw/adc/adc.h"
#include "hw/i2c/i2c.h"
#include "hw/ssi/ssi.h"
#include "hw/uart/uart.h"
#include "hw_demo.h"



// ******************************************************************
// Global variables for the IO peripherals.
// ******************************************************************

// ADC.
extern tADC g_sAdcJoystickX;
extern tADC g_sAdcJoystickY;
extern tADC g_sAdcAccelX;
extern tADC g_sAdcAccelY;
extern tADC g_sAdcAccelZ;

// I2C masters.
extern tI2C g_sI2C0;
extern tI2C g_sI2C2;

// UARTs.
extern tUART g_sUart6;

// Synchronous Serial Interface (SSI).
extern tSSI g_sSsi2;
extern tSSI g_sSsi3;



#endif  // __HW_DEMO_H_IO__

