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
// HAL_EK_TM4C1294XL_Crystalfontz128x128_ST7735.c -
//           Hardware abstraction layer for using the Educational Boosterpack's
//           Crystalfontz128x128 LCD with EK-TM4C1294XL LaunchPad
//
// Ported by Gene Bogdanov 8/30/2017
//
//*****************************************************************************


#include <stdint.h>
#include <stdbool.h>
#include "HAL_EK_TM4C1294XL_Crystalfontz128x128_ST7735.h"
#include "grlib/grlib.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

void HAL_LCD_PortInit(void)
{
    // LCD_SCK
    SysCtlPeripheralEnable(LCD_SCK_PERIPH);
    GPIOPinTypeSSI(LCD_SCK_BASE, LCD_SCK_PIN);
    GPIOPinConfigure(LCD_SCK_CONFIG);

    // LCD_MOSI
    SysCtlPeripheralEnable(LCD_MOSI_PERIPH);
    GPIOPinTypeSSI(LCD_MOSI_BASE, LCD_MOSI_PIN);
    GPIOPinConfigure(LCD_MOSI_CONFIG);

    // LCD_RST
    SysCtlPeripheralEnable(LCD_RST_PERIPH);
    GPIOPinTypeGPIOOutput(LCD_RST_BASE, LCD_RST_PIN);

    // LCD_RS (DC)
    SysCtlPeripheralEnable(LCD_DC_PERIPH);
    GPIOPinTypeGPIOOutput(LCD_DC_BASE, LCD_DC_PIN);

    // LCD_CS
    SysCtlPeripheralEnable(LCD_CS_PERIPH);
    GPIOPinTypeGPIOOutput(LCD_CS_BASE, LCD_CS_PIN);
}

void HAL_LCD_SpiInit(void)
{
    // configure SSI as SPI
    SysCtlPeripheralEnable(LCD_SSI_PERIPH);
    SSIConfigSetExpClk(LCD_SSI_BASE, LCD_SYSTEM_CLOCK, LCD_SSI_PROTOCOL, SSI_MODE_MASTER, LCD_SSI_CLOCK, LCD_SSI_DATA_WIDTH);
    SSIEnable(LCD_SSI_BASE);

    // set outputs to default state
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0);
    GPIOPinWrite(LCD_DC_BASE, LCD_DC_PIN, LCD_DC_PIN);

    // activate reset
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, 0);

    HAL_LCD_delay(20);

    // deactivate reset
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);

    // delay more than 120 ms after reset
    HAL_LCD_delay(120000);
}


//*****************************************************************************
//
// Writes a command to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t command)
{
    // Set to command mode
    while (SSIBusy(LCD_SSI_BASE)); // finish any transmission
    GPIOPinWrite(LCD_DC_BASE, LCD_DC_PIN, 0);

    // Transmit data
    SSIDataPut(LCD_SSI_BASE, command);
    while (SSIBusy(LCD_SSI_BASE)); // finish transmission

    // Set back to data mode
    GPIOPinWrite(LCD_DC_BASE, LCD_DC_PIN, LCD_DC_PIN);
}


//*****************************************************************************
//
// Writes a data to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeData(uint8_t data)
{
    // Transmit data
    SSIDataPut(LCD_SSI_BASE, data); // returns before data finishes transmitting
}
