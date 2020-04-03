/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// HAL_EK_TM4C1294XL_Crystalfontz128x128_ST7735.h -
//           Hardware abstraction layer for using the Educational Boosterpack's
//           Crystalfontz128x128 LCD with EK-TM4C1294XL LaunchPad
//
// Ported by Gene Bogdanov 8/30/2017
//
//*****************************************************************************

#ifndef __HAL_EK_TM4C1294XL_CRYSTALFONTZLCD_H_
#define __HAL_EK_TM4C1294XL_CRYSTALFONTZLCD_H_

#include <stdint.h>
#include "driverlib/sysctl.h"

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// System clock speed (in Hz)
#define LCD_SYSTEM_CLOCK    120000000 // EK-TM4C1294XL at full speed
// SPI clock speed (in Hz)
#define LCD_SSI_CLOCK       15000000

// Ports and pins from MCU connected to LCD (BOOSTXL-EDUMKII as BoosterPack 2 of EK-TM4C1294XL)
// SCK
#define LCD_SCK_PERIPH      SYSCTL_PERIPH_GPIOQ
#define LCD_SCK_BASE        GPIO_PORTQ_BASE
#define LCD_SCK_PIN         GPIO_PIN_0
#define LCD_SCK_CONFIG      GPIO_PQ0_SSI3CLK
// MOSI
#define LCD_MOSI_PERIPH     SYSCTL_PERIPH_GPIOQ
#define LCD_MOSI_BASE       GPIO_PORTQ_BASE
#define LCD_MOSI_PIN        GPIO_PIN_2
#define LCD_MOSI_CONFIG     GPIO_PQ2_SSI3XDAT0
// RST
#define LCD_RST_PERIPH      SYSCTL_PERIPH_GPIOA
#define LCD_RST_BASE        GPIO_PORTA_BASE
#define LCD_RST_PIN         GPIO_PIN_7
// CS
#define LCD_CS_PERIPH       SYSCTL_PERIPH_GPIOP
#define LCD_CS_BASE         GPIO_PORTP_BASE
#define LCD_CS_PIN          GPIO_PIN_3
// DC (RS)
#define LCD_DC_PERIPH       SYSCTL_PERIPH_GPIOK
#define LCD_DC_BASE         GPIO_PORTK_BASE
#define LCD_DC_PIN          GPIO_PIN_7

// SPI (SSI) configuration
#define LCD_SSI_PERIPH      SYSCTL_PERIPH_SSI3
#define LCD_SSI_BASE        SSI3_BASE
#define LCD_SSI_PROTOCOL    SSI_FRF_MOTO_MODE_0
#define LCD_SSI_DATA_WIDTH  8 // bits

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void HAL_LCD_writeCommand(uint8_t command);
extern void HAL_LCD_writeData(uint8_t data);
extern void HAL_LCD_PortInit(void);
extern void HAL_LCD_SpiInit(void);

#define HAL_LCD_delay(x)    SysCtlDelay((x) * 40) // delay in us

#endif /* __HAL_EK_TM4C1294XL_CRYSTALFONTZLCD_H_ */
