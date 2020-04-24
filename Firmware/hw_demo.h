// File: hw_demo.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 24 Apr 2020
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
#define FW_VERSION                  "0.3.3"
#define FW_RELEASEDATE              "24 Apr 2020"



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

//#define ADC_VALUES_HEX

// I2C parameters.
#define EDUMKII_I2C_TMP006_SLV_ADR  0x40
#define EDUMKII_I2C_OPT3001_SLV_ADR 0x44

// SSI parameters.
#define SSI_FREQ_MIN                2000
#define SSI_FREQ_MAX                50000000
#define SSI_DATAWIDTH_MIN           4
#define SSI_DATAWIDTH_MAX           16

// UART parameters.
#define UART_BAUD_MIN               150
#define UART_BAUD_MAX               15000000

// LCD settings.
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

