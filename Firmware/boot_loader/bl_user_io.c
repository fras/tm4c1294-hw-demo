// File: bl_user_io.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 27 Aug 2020
// Rev.: 27 Aug 2020
//
// IO peripheral definitions of the boot loader running on the TI Tiva TM4C1294
// Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "bl_user_io.h"



// ******************************************************************
// UARTs.
// ******************************************************************

// UART on BoosterPack 1.
tUART g_sUart7 = {
    SYSCTL_PERIPH_UART7,
    SYSCTL_PERIPH_GPIOC,
    GPIO_PORTC_BASE,
    GPIO_PIN_4,             // RX
    GPIO_PIN_5,             // TX
    GPIO_PC4_U7RX,          // RX
    GPIO_PC5_U7TX,          // TX
    UART7_BASE,
    0,                      // ui32UartClk
    115200,                 // ui32Baud
    UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE,
    false                   // bLoopback
};

