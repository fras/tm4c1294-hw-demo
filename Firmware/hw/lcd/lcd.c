// File: lcd.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 03 Apr 2020
// Rev.: 04 Apr 2020
//
// Functions for the LCD on the Educational BoosterPack MKII connected to the
// BoosterPack 2 socket of the TI Tiva TM4C1294 Connected LaunchPad Evaluation
// Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "Crystalfontz128x128_ST7735.h"
#include "HAL_EK_TM4C1294XL_Crystalfontz128x128_ST7735.h"
#include "images.h"
#include "lcd.h"



// Initialize the LCD on the Educational BoosterPack MKII.
void LcdInit(tContext *psContext)
{
    // Initialize the LCD display driver.
    Crystalfontz128x128_Init();

    // Set screen orientation.
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize the grlib graphics context.
    GrContextInit(psContext, &g_sCrystalfontz128x128);
    GrContextFontSet(psContext, &g_sFontFixed6x8);

    // Fill screen with black.
    tRectangle rectFullScreen = {0, 0, GrContextDpyWidthGet(psContext)-1, GrContextDpyHeightGet(psContext)-1};
    GrContextForegroundSet(psContext, ClrBlack);
    GrRectFill(psContext, &rectFullScreen);

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Set the screen orientation of the LCD.
void LcdSetOrientation(tContext *psContext, uint8_t ui8Orientation)
{
    // Set screen orientation.
    Crystalfontz128x128_SetOrientation(ui8Orientation & 0x3);

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Clear the LCD.
void LcdClear(tContext *psContext, uint32_t ui32Color)
{
    tRectangle sRect;

    // Fill background.
    GrContextForegroundSet(psContext, ui32Color);
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(psContext) - 1;
    sRect.i16YMax = GrContextDpyWidthGet(psContext) - 1;
    GrRectFill(psContext, &sRect);

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Draw a pixel the LCD.
void LcdDrawPixel(tContext *psContext, int32_t i32X, int32_t i32Y,
                  uint32_t ui32Color)
{
    // Set foreground color.
    GrContextForegroundSet(psContext, ui32Color);

    // Draw a line with the same start and end point.
    GrLineDraw(psContext, i32X, i32Y, i32X, i32Y);

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Draw a line on the LCD.
void LcdDrawLine(tContext *psContext, int32_t i32X1, int32_t i32Y1,
                 int32_t i32X2, int32_t i32Y2, uint32_t ui32Color)
{
    // Set foreground color.
    GrContextForegroundSet(psContext, ui32Color);

    // Draw a line.
    GrLineDraw(psContext, i32X1, i32Y1, i32X2, i32Y2);

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Draw a rectangle on the LCD.
void LcdDrawRect(tContext *psContext, int16_t i16X1, int16_t i16Y1,
                 int16_t i16X2, int16_t i16Y2, uint32_t ui32Color, bool bFill)
{
    tRectangle sRect;

    // Set foreground color.
    GrContextForegroundSet(psContext, ui32Color);

    // Draw a rectangle.
    sRect.i16XMin = i16X1;
    sRect.i16YMin = i16Y1;
    sRect.i16XMax = i16X2;
    sRect.i16YMax = i16Y2;
    if (bFill) {
        GrRectFill(psContext, &sRect);
    } else{
        GrRectDraw(psContext, &sRect);
    }

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Draw a circle on the LCD.
void LcdDrawCircle(tContext *psContext, int32_t i32X, int32_t i32Y,
                   int32_t i32Radius, uint32_t ui32Color, bool bFill)
{
    // Set foreground color.
    GrContextForegroundSet(psContext, ui32Color);

    // Draw a circle.
    if (bFill) {
        GrCircleFill(psContext, i32X, i32Y, i32Radius);
    } else{
        GrCircleDraw(psContext, i32X, i32Y, i32Radius);
    }

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Draw a text on the LCD.
void LcdDrawText(tContext *psContext, const char *pcText, int32_t i32X,
                 int32_t i32Y, uint32_t ui32Color, bool bCenter)
{
    // Set foreground color.
    GrContextForegroundSet(psContext, ui32Color);

    // Draw a string.
    if (bCenter) {
        GrStringDrawCentered(psContext, pcText, -1, i32X, i32Y, false);
    } else {
        GrStringDraw(psContext, pcText, -1, i32X, i32Y, false);
    }

    // Flush any cached drawing operations.
    GrFlush(psContext);
}



// Set a font for text on the LCD.
void LcdFontSet(tContext *psContext, const tFont *pFnt)
{
    GrContextFontSet(psContext, pFnt);
}



// Show firmware info on the LCD.
void LcdFwInfo(tLcdFwInfo *psLcdFwInfo)
{
    tContext *psContext = psLcdFwInfo->psContext;
    tRectangle sRect;
    int i32ImageX;
    char str[64];

    // Fill background.
    GrContextForegroundSet(psContext, psLcdFwInfo->ui32ColorBackground);
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(psContext) - 1;
    sRect.i16YMax = GrContextDpyWidthGet(psContext) - 1;
    GrRectFill(psContext, &sRect);

    // Draw frame.
    GrContextForegroundSet(psContext, psLcdFwInfo->ui32ColorFrame);
    sRect.i16XMin = LCD_FRAME_PADDING;
    sRect.i16YMin = LCD_FRAME_PADDING;
    sRect.i16XMax = GrContextDpyWidthGet(psContext) - 1 - LCD_FRAME_PADDING;
    sRect.i16YMax = GrContextDpyHeightGet(psContext) - 1 - LCD_FRAME_PADDING;
    for (int i = 0; i < LCD_FRAME_WIDTH; i++) {
        GrRectDraw(psContext, &sRect);
        sRect.i16XMin += 1;
        sRect.i16YMin += 1;
        sRect.i16XMax -= 1;
        sRect.i16YMax -= 1;
    }

    // Draw title.
    GrContextForegroundSet(psContext, psLcdFwInfo->ui32ColorTitle);
    GrContextFontSet(psContext, &g_sFontCmss14b);
    GrStringDrawCentered(psContext, psLcdFwInfo->pcTitle, -1,
                         GrContextDpyWidthGet(psContext) / 2,
                         LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING + 3, false);

    // Draw firmware info.
    GrContextForegroundSet(psContext, psLcdFwInfo->ui32ColorText);
    GrContextFontSet(psContext, &g_sFontFixed6x8);
    usnprintf(str, sizeof(str), "Firmware Info");
    GrStringDraw(psContext, str, -1, LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING, 24, false);
    usnprintf(str, sizeof(str), "Name: %s", psLcdFwInfo->pcFwName);
    GrStringDraw(psContext, str, -1, LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING, 38, false);
    usnprintf(str, sizeof(str), "Ver.: %s", psLcdFwInfo->pcFwVersion);
    GrStringDraw(psContext, str, -1, LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING, 48, false);
    usnprintf(str, sizeof(str), "Date: %s", psLcdFwInfo->pcFwDate);
    GrStringDraw(psContext, str, -1, LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING, 58, false);

    // Draw horizontal lines as separator.
    GrContextForegroundSet(psContext, psLcdFwInfo->ui32ColorSeparator);
    GrLineDrawH(psContext, LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING,
                GrContextDpyWidthGet(psContext) - (LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING) - 1, 34);
    GrLineDrawH(psContext, LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING,
                GrContextDpyWidthGet(psContext) - (LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING) - 1, 68);

    // Draw logo.
    // Auto center the image.
    if (psLcdFwInfo->i32ImageX < 0) {
        i32ImageX = (GrContextDpyWidthGet(psContext) -
                 (psLcdFwInfo->pu8ImageData[1] + (psLcdFwInfo->pu8ImageData[2] << 8)))  / 2;
    } else {
        i32ImageX = psLcdFwInfo->i32ImageX;
    }
    GrImageDraw(psContext, psLcdFwInfo->pu8ImageData, i32ImageX, psLcdFwInfo->i32ImageY);

    // Flush any cached drawing operations.
    GrFlush(psContext);
}

