// File: uart_ui.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 18 Feb 2020
//
// Header file for the UART user interface for the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#ifndef __UART_UI_H__
#define __UART_UI_H__



// Hardware parameters of the UART for the user interface.
#define UART_UI_NUM                 7
#define UART_UI_BAUD                115200
#define UART_UI_SYSCTL_PERIPH_GPIO  SYSCTL_PERIPH_GPIOC
#define UART_UI_SYSCTL_PERIPH_UART  SYSCTL_PERIPH_UART7
#define UART_UI_GPIO_PORT_BASE      GPIO_PORTC_BASE
#define UART_UI_GPIO_RX             GPIO_PC4_U7RX
#define UART_UI_GPIO_TX             GPIO_PC5_U7TX
#define UART_UI_GPIO_PIN_RX         GPIO_PIN_4
#define UART_UI_GPIO_PIN_TX         GPIO_PIN_5



// Function prototypes.
void UartUiInit(uint32_t ui32SysClock);



#endif  // __UART_UI_H__

