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
// Crystalfontz128x128.c - Display driver for the Crystalfontz
//                         128x128 display with ST7735 controller.
//
// Modified by Gene Bogdanov 8/30/2017
//
//*****************************************************************************

#include "Crystalfontz128x128_ST7735.h"

#include <stdint.h>
#include <stdbool.h>
#include "grlib/grlib.h"
#include "HAL_EK_TM4C1294XL_Crystalfontz128x128_ST7735.h"

uint8_t Lcd_Orientation;
uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;
uint8_t Lcd_PenSolid, Lcd_FontSolid, Lcd_FlagRead;
uint16_t Lcd_TouchTrim;

uint16_t Lcd_buffer[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX] = {0}; // Gene Bogdanov: LCD frame buffer in RAM

static void Crystalfontz128x128_Flush(void *pvDisplayData);
static uint32_t Crystalfontz128x128_ColorTranslate(void *pvDisplayData, uint32_t ulValue);

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the ST7735 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void Crystalfontz128x128_Init(void)
{
    HAL_LCD_PortInit();
    HAL_LCD_SpiInit();

    HAL_LCD_writeCommand(CM_SLPOUT);
    HAL_LCD_delay(120000);

    HAL_LCD_writeCommand(CM_GAMSET);
    HAL_LCD_writeData(0x04);

    HAL_LCD_writeCommand(CM_SETPWCTR);
    HAL_LCD_writeData(0x0A);
    HAL_LCD_writeData(0x14);

    HAL_LCD_writeCommand(CM_SETSTBA);
    HAL_LCD_writeData(0x0A);
    HAL_LCD_writeData(0x00);

    HAL_LCD_writeCommand(CM_COLMOD);
    HAL_LCD_writeData(0x05);
    HAL_LCD_delay(10);

    HAL_LCD_writeCommand(CM_MADCTL);
    HAL_LCD_writeData(CM_MADCTL_BGR);

    HAL_LCD_writeCommand(CM_NORON);

    Lcd_ScreenWidth  = LCD_HORIZONTAL_MAX;
    Lcd_ScreenHeigth = LCD_VERTICAL_MAX;
    Lcd_PenSolid  = 0;
    Lcd_FontSolid = 1;
    Lcd_FlagRead  = 0;
    Lcd_TouchTrim = 0;

    Crystalfontz128x128_Flush(0); // Gene Bogdanov: flush the RAM buffer instead of filling LCD memory with fixed values

    HAL_LCD_delay(10);
    HAL_LCD_writeCommand(CM_DISPON);
}


void Crystalfontz128x128_SetDrawFrame(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
    switch (Lcd_Orientation) {
        case 0:
            x0 += 2;
            y0 += 3;
            x1 += 2;
            y1 += 3;
            break;
        case 1:
            x0 += 3;
            y0 += 2;
            x1 += 3;
            y1 += 2;
            break;
        case 2:
            x0 += 2;
            y0 += 1;
            x1 += 2;
            y1 += 1;
            break;
        case 3:
            x0 += 1;
            y0 += 2;
            x1 += 1;
            y1 += 2;
            break;
        default:
            break;
    }

    HAL_LCD_writeCommand(CM_CASET);
    HAL_LCD_writeData((uint8_t)(x0 >> 8));
    HAL_LCD_writeData((uint8_t)(x0));
    HAL_LCD_writeData((uint8_t)(x1 >> 8));
    HAL_LCD_writeData((uint8_t)(x1));

    HAL_LCD_writeCommand(CM_RASET);
    HAL_LCD_writeData((uint8_t)(y0 >> 8));
    HAL_LCD_writeData((uint8_t)(y0));
    HAL_LCD_writeData((uint8_t)(y1 >> 8));
    HAL_LCD_writeData((uint8_t)(y1));
}


//*****************************************************************************
//
//! Sets the LCD Orientation.
//!
//! \param orientation is the desired orientation for the LCD. Valid values are:
//!           - \b LCD_ORIENTATION_UP,
//!           - \b LCD_ORIENTATION_LEFT,
//!           - \b LCD_ORIENTATION_DOWN,
//!           - \b LCD_ORIENTATION_RIGHT,
//!
//! This function sets the orientation of the LCD
//!
//! \return None.
//
//*****************************************************************************
void Crystalfontz128x128_SetOrientation(uint8_t orientation)
{
    Lcd_Orientation = orientation;
    HAL_LCD_writeCommand(CM_MADCTL);
    switch (Lcd_Orientation) {
        case LCD_ORIENTATION_UP:
            HAL_LCD_writeData(CM_MADCTL_MX | CM_MADCTL_MY | CM_MADCTL_BGR);
            break;
        case LCD_ORIENTATION_LEFT:
            HAL_LCD_writeData(CM_MADCTL_MY | CM_MADCTL_MV | CM_MADCTL_BGR);
            break;
        case LCD_ORIENTATION_DOWN:
            HAL_LCD_writeData(CM_MADCTL_BGR);
            break;
        case LCD_ORIENTATION_RIGHT:
            HAL_LCD_writeData(CM_MADCTL_MX | CM_MADCTL_MV | CM_MADCTL_BGR);
            break;
    }
}


//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color. The coordinates of
//! the pixel are assumed to be within the extents of the display.
//!
//! Gene Bogdanov: Added local frame buffer.
//!
//! \return None.
//
//*****************************************************************************
static void Crystalfontz128x128_PixelDraw(void *pvDisplayData, int32_t lX, int32_t lY,
                                   uint32_t ulValue)
{
    Lcd_buffer[lY][lX] = ulValue;
}


//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void Crystalfontz128x128_PixelDrawMultiple(void *pvDisplayData, int32_t lX,
                                           int32_t lY, int32_t lX0, int32_t lCount,
                                           int32_t lBPP,
                                           const uint8_t *pucData,
                                           const uint8_t *pucPalette)
{
    uint32_t Data, rgb, native;
    uint16_t *pWrite = &Lcd_buffer[lY][lX]; // pointer to the write location in Lcd_buffer[]

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        // The pixel data is in 1 bit per pixel format
        case 1:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                Data = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
                    *pWrite++ = ((uint32_t *)pucPalette)[(Data >> (7 - lX0)) & 1];
                }

                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn

            break;
        }

        // The pixel data is in 4 bit per pixel format
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch(lX0 & 1)
            {
                case 0:

                    while(lCount)
                    {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the palette
                        Data = (*pucData >> 4);
                        rgb = *(uint32_t *)(pucPalette + 3*Data);
                        native = Crystalfontz128x128_ColorTranslate(pvDisplayData, rgb);
                        // Write to LCD screen
                        *pWrite++ = native;

                        // Decrement the count of pixels to draw
                        lCount--;

                        // See if there is another pixel to draw
                        if(lCount)
                        {
                case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette
                            Data = (*pucData++ & 15);
                            rgb = *(uint32_t *)(pucPalette + 3*Data);
                            native = Crystalfontz128x128_ColorTranslate(pvDisplayData, rgb);
                            // Write to LCD screen
                            *pWrite++ = native;

                            // Decrement the count of pixels to draw
                            lCount--;
                        }
                    }
            }
            // The image data has been drawn.

            break;
        }

        // The pixel data is in 8 bit per pixel format
        case 8:
        {
            // Loop while there are more pixels to draw
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                Data = *pucData++;
                rgb = *(uint32_t *)(pucPalette + 3*Data);
                native = Crystalfontz128x128_ColorTranslate(pvDisplayData, rgb);
                // Write to LCD screen
                *pWrite++ = native;
            }
            // The image data has been drawn
            break;
        }

        //
        // We are being passed data in the display's native format.  Merely
        // write it directly to the display.  This is a special case which is
        // not used by the graphics library but which is helpful to
        // applications which may want to handle, for example, JPEG images.
        //
        case 16:
        {
            uint16_t usData;

            // Loop while there are more pixels to draw.

            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                usData = *((uint16_t *)pucData);
                pucData += 2;

                // Translate this palette entry and write it to the screen
                *pWrite++ = usData;
            }
        }
    }
}


//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void Crystalfontz128x128_LineDrawH(void *pvDisplayData, int32_t lX1, int32_t lX2,
                                   int32_t lY, uint32_t ulValue)
{
    if (lX1 & 1) { // starts at an odd coordinate: fill in first pixel
        Lcd_buffer[lY][lX1] = ulValue;
        lX1++;
    }

    if (!(lX2 & 1)) { // ends at an even coordinate: fill in last pixel
        Lcd_buffer[lY][lX2] = ulValue;
        lX2--;
    }

    // fill in the bulk of the line 2 pixels at a time
    uint32_t *pWrite = (uint32_t*)&Lcd_buffer[lY][lX1]; // pointer to the start of the line
    uint32_t fill = ulValue | (ulValue << 16);
    for (; lX1 < lX2; lX1 += 2) {
        *pWrite++ = fill;
    }
}


//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void Crystalfontz128x128_LineDrawV(void *pvDisplayData, int32_t lX, int32_t lY1,
                                   int32_t lY2, uint32_t ulValue)
{
    // fill the line
    for (; lY1 <= lY2; lY1++) {
        Lcd_buffer[lY1][lX] = ulValue;
    }
}


//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void Crystalfontz128x128_RectFill(void *pvDisplayData, const tRectangle *pRect,
                                  uint32_t ulValue)
{
    int32_t lX1 = pRect->i16XMin;
    int32_t lX2 = pRect->i16XMax;
    int32_t lY1 = pRect->i16YMin;
    int32_t lY2 = pRect->i16YMax;
    uint32_t i;
    if (lX1 & 1) { // starts at an odd coordinate: fill in first column
        for (i = lY1; i <= lY2; i++) {
            Lcd_buffer[i][lX1] = ulValue;
        }
        lX1++;
    }

    if (!(lX2 & 1)) { // ends at an even coordinate: fill in last column
        for (i = lY1; i <= lY2; i++) {
            Lcd_buffer[i][lX2] = ulValue;
        }
        lX2--;
    }

    // fill the bulk of the rectangle
    uint32_t fill = ulValue | (ulValue << 16);
    for (; lY1 <= lY2; lY1++) {
        // fill 2 pixels at a time
        uint32_t *pWrite = (uint32_t*)&Lcd_buffer[lY1][lX1]; // pointer to the start of the line
        for (i = lX1; i < lX2; i += 2) {
            *pWrite++ = fill;
        }
    }
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t Crystalfontz128x128_ColorTranslate(void *pvDisplayData,
                                    uint32_t ulValue)
{
    //
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    //
    uint32_t rgb565 =
           ((((ulValue) & 0x00f80000) >> 8) |
            (((ulValue) & 0x0000fc00) >> 5) |
            (((ulValue) & 0x000000f8) >> 3));
    return (rgb565 >> 8) | ((rgb565 << 8) & 0xff00);  // swap bytes
}


//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.
//!
//! Gene Bogdanov: Added local frame buffer.
//!
//! \return None.
//
//*****************************************************************************
static void
Crystalfontz128x128_Flush(void *pvDisplayData)
{
    Crystalfontz128x128_SetDrawFrame(0, 0, LCD_HORIZONTAL_MAX-1, LCD_VERTICAL_MAX-1);
    HAL_LCD_writeCommand(CM_RAMWR);
    int i;
    uint16_t *pRead = &Lcd_buffer[0][0];
    uint32_t data;
    for (i = 0; i < LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX; i++)
    {
        data = *pRead++;
        HAL_LCD_writeData((uint8_t)data);
        HAL_LCD_writeData((uint8_t)(data >> 8));
    }
}


//*****************************************************************************
//
//! Send command to clear screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This function does a clear screen and the Display Buffer contents
//! are initialized to the current background color.
//!
//! \return None.
//
//*****************************************************************************
//static void
//Crystalfontz128x128_ClearScreen (void *pvDisplayData, uint16_t ulValue)
//{
//    tRectangle rect = { 0, 0, LCD_VERTICAL_MAX-1, LCD_VERTICAL_MAX-1};
//    Crystalfontz128x128_RectFill(&pvDisplayData, &rect, ulValue);
//}


//*****************************************************************************
//
//! The display structure that describes the driver for the Kitronix
//! K350QVG-V1-F TFT panel with an SSD2119 controller.
//
//*****************************************************************************
const tDisplay g_sCrystalfontz128x128 =
{
    sizeof(tDisplay),
    0,
    LCD_VERTICAL_MAX,
    LCD_HORIZONTAL_MAX,
    Crystalfontz128x128_PixelDraw,
    Crystalfontz128x128_PixelDrawMultiple,
    Crystalfontz128x128_LineDrawH,
    Crystalfontz128x128_LineDrawV,
    Crystalfontz128x128_RectFill,
    Crystalfontz128x128_ColorTranslate,
    Crystalfontz128x128_Flush
};

