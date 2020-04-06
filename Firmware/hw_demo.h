// File: hw_demo.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 06 Apr 2020
//
// Header file of the hardware demo for the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#ifndef __HW_DEMO_H__
#define __HW_DEMO_H__



// ******************************************************************
// Firmware parameters.
// ******************************************************************
#define FW_NAME                     "hw_demo"
#define FW_VERSION                  "0.1.3"
#define FW_RELEASEDATE              "06 Apr 2020"



// ******************************************************************
// System clock settings.
// ******************************************************************
#define SYSTEM_CLOCK_SETTINGS       (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480)
#define SYSTEM_CLOCK_FREQ           120000000


// ******************************************************************
// User interface.
// ******************************************************************
#define UI_COMMAND_PROMPT           "> "
#define UI_STR_BUF_SIZE             256
#define UI_STR_DELIMITER            " \t"
#define UI_STR_OK                   "OK"
#define UI_STR_WARNING              "WARNING"
#define UI_STR_ERROR                "ERROR"
#define UI_STR_FATAL                "FATAL"



// ******************************************************************
// Hardware settings.
// ******************************************************************

// ADCs.
//#define ADC_VALUES_HEX
// Joystick X.
tADC sAdcJoystickX = {
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
tADC sAdcJoystickY = {
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
tADC sAdcAccelX = {
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
tADC sAdcAccelY = {
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
tADC sAdcAccelZ = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOK,
    GPIO_PORTK_BASE,
    GPIO_PIN_0,
    ADC1_BASE,
    2,                      // ui32SequenceNum
    0,                      // ui32Step
    ADC_CTL_CH16 | ADC_CTL_IE | ADC_CTL_END
};



// I2C master No. 0 (BoosterPack 1).
tI2C sI2C0 = {
    SYSCTL_PERIPH_I2C0,
    SYSCTL_PERIPH_GPIOB,
    GPIO_PORTB_BASE,
    GPIO_PIN_2,             // SCL
    GPIO_PIN_3,             // SDA
    GPIO_PB2_I2C0SCL,
    GPIO_PB3_I2C0SDA,
    I2C0_BASE,
    0,                      // ui32SysClock
    false,                  // false = 100 kbps; true = 400 kbps
    I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_STOP | I2C_MASTER_INT_START |
        I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT | I2C_MASTER_INT_DATA,
    100                     // ui32Timeout
};

// I2C master No. 2 (BoosterPack 2).
tI2C sI2C2 = {
    SYSCTL_PERIPH_I2C2,
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_5,             // SCL
    GPIO_PIN_4,             // SDA
    GPIO_PN5_I2C2SCL,
    GPIO_PN4_I2C2SDA,
    I2C2_BASE,
    0,                      // ui32SysClock
    false,                  // false = 100 kbps; true = 400 kbps
    I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_STOP | I2C_MASTER_INT_START |
        I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT | I2C_MASTER_INT_DATA,
    100                     // ui32Timeout
};

// I2C parameters.
#define EDUMKII_I2C_TMP006_SLV_ADR  0x40
#define EDUMKII_I2C_OPT3001_SLV_ADR 0x44



// UARTs.
tUART sUartBoosterPack2 = {
    SYSCTL_PERIPH_UART6,
    SYSCTL_PERIPH_GPIOP,
    GPIO_PORTP_BASE,
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PP0_U6RX,
    GPIO_PP1_U6TX,
    UART6_BASE,
    0,                      // ui32SysClock
    115200,                 // ui32Baud
    UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE,
    false                   // bLoopback
};



// Synchronous Serial Interface (SSI). This can also be configured to use the
// Motorola SPI frame format.

// SSI 2 for BoosterPack 1.
tSSI sSsi2 = {
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
    0,                      // ui32SysClock
    SSI_FRF_MOTO_MODE_0,    // ui32Protocol
    SSI_MODE_MASTER,        // ui32Mode
    15000000,               // ui32BitRate
    8,                      // ui32DataWidth
    100                     // ui32Timeout
};

// SSI 3 for BoosterPack 2.
tSSI sSsi3 = {
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
    0,                      // ui32SysClock
    SSI_FRF_MOTO_MODE_0,    // ui32Protocol
    SSI_MODE_MASTER,        // ui32Mode
    15000000,               // ui32BitRate
    8,                      // ui32DataWidth
    100                     // ui32Timeout
};



// ******************************************************************
// LCD settings.
// ******************************************************************
#define LCD_COLOR_BLACK             0x000000
#define LCD_COLOR_WHITE             0xffffff
#define LCD_COLOR_MPG_GREEN_1       0x006c66
#define LCD_COLOR_MPG_GREEN_2       0x005555
#define LCD_COLOR_MPG_GREY_DARK     0x777777
#define LCD_COLOR_MPG_GREY          0xa7a7a8
#define LCD_COLOR_MPG_GREY_LIGHT    0xeeeeee
#define LCD_COLOR_MPG_GREEN_LIGHT   0xc6d325
#define LCD_COLOR_MPG_BLUE_DARK     0x29485d
#define LCD_COLOR_MPG_BLUE_LIGHT    0x00b1ea
#define LCD_COLOR_MPG_ORANGE        0xef7c00



#endif  // __HW_DEMO_H__

