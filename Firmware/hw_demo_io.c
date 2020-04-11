// File: hw_demo_io.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 09 Apr 2020
// Rev.: 11 Apr 2020
//
// IO peripheral definitions of the hardware demo for the TI Tiva TM4C1294
// Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "hw_demo_io.h"



// ******************************************************************
// UART user inferface (UI).
// ******************************************************************

tUartUi g_sUartUi = {
    SYSCTL_PERIPH_UART7,
    SYSCTL_PERIPH_GPIOC,
    GPIO_PORTC_BASE,
    GPIO_PIN_4,             // RX
    GPIO_PIN_5,             // TX
    GPIO_PC4_U7RX,          // RX
    GPIO_PC5_U7TX,          // TX
    UART7_BASE,             // ui32Base
    0,                      // ui32SrcClock
    115200,                 // ui32Baud
    7                       // ui32Port
};



// ******************************************************************
// ADC.
// ******************************************************************

// Joystick X.
tADC g_sAdcJoystickX = {
    SYSCTL_PERIPH_ADC0,
    SYSCTL_PERIPH_GPIOD,
    GPIO_PORTD_BASE,
    GPIO_PIN_2,
    ADC0_BASE,
    0,                      // ui32SequenceNum
    0,                      // ui32Step
    ADC_CTL_CH13 | ADC_CTL_IE | ADC_CTL_END
};
// Joystick Y.
tADC g_sAdcJoystickY = {
    SYSCTL_PERIPH_ADC0,
    SYSCTL_PERIPH_GPIOK,
    GPIO_PORTK_BASE,
    GPIO_PIN_1,
    ADC0_BASE,
    1,                      // ui32SequenceNum
    0,                      // ui32Step
    ADC_CTL_CH17 | ADC_CTL_IE | ADC_CTL_END
};
// Accelerometer X.
tADC g_sAdcAccelX = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOB,
    GPIO_PORTB_BASE,
    GPIO_PIN_4,
    ADC1_BASE,
    0,                      // ui32SequenceNum
    0,                      // ui32Step
    ADC_CTL_CH10 | ADC_CTL_IE | ADC_CTL_END
};
// Accelerometer Y.
tADC g_sAdcAccelY = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOB,
    GPIO_PORTB_BASE,
    GPIO_PIN_5,
    ADC1_BASE,
    1,                      // ui32SequenceNum
    0,                      // ui32Step
    ADC_CTL_CH11 | ADC_CTL_IE | ADC_CTL_END
};
// Accelerometer Z.
tADC g_sAdcAccelZ = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOK,
    GPIO_PORTK_BASE,
    GPIO_PIN_0,
    ADC1_BASE,
    2,                      // ui32SequenceNum
    0,                      // ui32Step
    ADC_CTL_CH16 | ADC_CTL_IE | ADC_CTL_END
};



// ******************************************************************
// I2C masters.
// ******************************************************************

// I2C master No. 0 (BoosterPack 1).
tI2C g_sI2C0 = {
    SYSCTL_PERIPH_I2C0,
    SYSCTL_PERIPH_GPIOB,
    GPIO_PORTB_BASE,
    GPIO_PIN_2,             // SCL
    GPIO_PIN_3,             // SDA
    GPIO_PB2_I2C0SCL,       // SCL
    GPIO_PB3_I2C0SDA,       // SDA
    I2C0_BASE,
    0,                      // ui32I2CClk
    false,                  // false = 100 kbps; true = 400 kbps
    I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_STOP | I2C_MASTER_INT_START |
        I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT | I2C_MASTER_INT_DATA,
    100                     // ui32Timeout
};

// I2C master No. 2 (BoosterPack 2).
tI2C g_sI2C2 = {
    SYSCTL_PERIPH_I2C2,
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_5,             // SCL
    GPIO_PIN_4,             // SDA
    GPIO_PN5_I2C2SCL,       // SCL
    GPIO_PN4_I2C2SDA,       // SDA
    I2C2_BASE,
    0,                      // ui32I2CClk
    false,                  // false = 100 kbps; true = 400 kbps
    I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_STOP | I2C_MASTER_INT_START |
        I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT | I2C_MASTER_INT_DATA,
    100                     // ui32Timeout
};



// ******************************************************************
// Synchronous Serial Interface (SSI). This can also be configured to use the
// Motorola SPI frame format.
// ******************************************************************

// SSI 2 for BoosterPack 1.
tSSI g_sSsi2 = {
    SYSCTL_PERIPH_SSI2,
    SYSCTL_PERIPH_GPIOD,
    GPIO_PORTD_BASE,
    GPIO_PIN_3,             // CLK
    // CAUTION: Pin PD2 is used as analog input on BoosterPack 2 (joystick X on
    //          the Educational BoosterPack MK II)! So it is disabled here.
//    GPIO_PIN_2,             // FSS
    0,                      // FSS
    GPIO_PIN_0,             // RX
    GPIO_PIN_1,             // TX
    GPIO_PD3_SSI2CLK,       // CLK
    GPIO_PD2_SSI2FSS,       // FSS
    GPIO_PD0_SSI2XDAT1,     // RX
    GPIO_PD1_SSI2XDAT0,     // TX
    SSI2_BASE,
    0,                      // ui32SsiClk
    SSI_FRF_MOTO_MODE_0,    // ui32Protocol
    SSI_MODE_MASTER,        // ui32Mode
    15000000,               // ui32BitRate
    8,                      // ui32DataWidth
    100                     // ui32Timeout
};

// SSI 3 for BoosterPack 2.
tSSI g_sSsi3 = {
    SYSCTL_PERIPH_SSI3,
    SYSCTL_PERIPH_GPIOQ,
    GPIO_PORTQ_BASE,
    GPIO_PIN_0,             // CLK
    GPIO_PIN_1,             // FSS
    GPIO_PIN_3,             // RX
    GPIO_PIN_2,             // TX
    GPIO_PQ0_SSI3CLK,       // CLK
    GPIO_PQ1_SSI3FSS,       // FSS
    GPIO_PQ3_SSI3XDAT1,     // RX
    GPIO_PQ2_SSI3XDAT0,     // TX
    SSI3_BASE,
    0,                      // ui32SsiClk
    SSI_FRF_MOTO_MODE_0,    // ui32Protocol
    SSI_MODE_MASTER,        // ui32Mode
    15000000,               // ui32BitRate
    8,                      // ui32DataWidth
    100                     // ui32Timeout
};



// ******************************************************************
// UARTs.
// ******************************************************************

// UART on BoosterPack 2.
tUART g_sUart6 = {
    SYSCTL_PERIPH_UART6,
    SYSCTL_PERIPH_GPIOP,
    GPIO_PORTP_BASE,
    GPIO_PIN_0,             // RX
    GPIO_PIN_1,             // TX
    GPIO_PP0_U6RX,          // RX
    GPIO_PP1_U6TX,          // TX
    UART6_BASE,
    0,                      // ui32UartClk
    115200,                 // ui32Baud
    UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE,
    false                   // bLoopback
};

