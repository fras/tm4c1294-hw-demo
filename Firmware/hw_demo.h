// File: hw_demo.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 30 Mar 2020
//
// Header file of the hardware demo for the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#ifndef __HW_DEMO_H__
#define __HW_DEMO_H__



// ******************************************************************
// Program parameters.
// ******************************************************************
#define PROG_NAME                   "hw_demo"
#define PROG_VERSION                "0.0.7"
#define PROG_RELEASEDATE            "30 Mar 2020"



// ******************************************************************
// System clock settings.
// ******************************************************************
#define SYSTEM_CLOCK_SETTINGS       (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480)
#define SYSTEM_CLOCK_FREQ           120000000


// ******************************************************************
// User interface.
// ******************************************************************
#define UI_COMMAND_PROMPT           "> "
#define UI_STR_BUF_SIZE             128
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
tADC adcJoystickX = {
    SYSCTL_PERIPH_ADC0,
    SYSCTL_PERIPH_GPIOD,
    GPIO_PORTD_BASE,
    GPIO_PIN_2,
    ADC0_BASE,
    0,
    0,
    ADC_CTL_CH13 | ADC_CTL_IE | ADC_CTL_END
};
// Joystick Y.
tADC adcJoystickY = {
    SYSCTL_PERIPH_ADC0,
    SYSCTL_PERIPH_GPIOK,
    GPIO_PORTK_BASE,
    GPIO_PIN_1,
    ADC0_BASE,
    1,
    0,
    ADC_CTL_CH17 | ADC_CTL_IE | ADC_CTL_END
};
// Accelerometer X.
tADC adcAccelX = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOB,
    GPIO_PORTB_BASE,
    GPIO_PIN_4,
    ADC1_BASE,
    0,
    0,
    ADC_CTL_CH10 | ADC_CTL_IE | ADC_CTL_END
};
// Accelerometer Y.
tADC adcAccelY = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOB,
    GPIO_PORTB_BASE,
    GPIO_PIN_5,
    ADC1_BASE,
    1,
    0,
    ADC_CTL_CH11 | ADC_CTL_IE | ADC_CTL_END
};
// Accelerometer Z.
tADC adcAccelZ = {
    SYSCTL_PERIPH_ADC1,
    SYSCTL_PERIPH_GPIOK,
    GPIO_PORTK_BASE,
    GPIO_PIN_0,
    ADC1_BASE,
    2,
    0,
    ADC_CTL_CH16 | ADC_CTL_IE | ADC_CTL_END
};

// I2C master No. 2.
tI2C i2c2 = {
    SYSCTL_PERIPH_I2C2,
    SYSCTL_PERIPH_GPION,
    GPIO_PORTN_BASE,
    GPIO_PIN_5,
    GPIO_PIN_4,
    GPIO_PN5_I2C2SCL,
    GPIO_PN4_I2C2SDA,
    I2C2_BASE,
    0,
    false,
    I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_STOP | I2C_MASTER_INT_START |
        I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT | I2C_MASTER_INT_DATA,
    100
};
// I2C parameters.
#define EDUMKII_I2C_TMP006_SLV_ADR  0x40
#define EDUMKII_I2C_OPT3001_SLV_ADR 0x44



// UARTs.
tUART uartBoosterPack2 = {
    SYSCTL_PERIPH_UART6,
    SYSCTL_PERIPH_GPIOP,
    GPIO_PORTP_BASE,
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PP0_U6RX,
    GPIO_PP1_U6TX,
    UART6_BASE,
    0,
    115200,
    UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE,
    true
};



#endif  // __HW_DEMO_H__

