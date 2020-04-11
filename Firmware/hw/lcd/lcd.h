// File: lcd.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 03 Apr 2020
// Rev.: 11 Apr 2020
//
// Header file for the LCD on the Educational BoosterPack MKII connected to the
// BoosterPack 2 socket of the TI Tiva TM4C1294 Connected LaunchPad Evaluation
// Kit.
//



#ifndef __LCD_H__
#define __LCD_H__



#include <stdint.h>
#include "grlib/grlib.h"



// Constants.
#define LCD_FRAME_PADDING           3
#define LCD_FRAME_WIDTH             3
#define LCD_CONTENT_PADDING         5



// Types.
typedef struct {
    tContext        *psContext;
    uint32_t        ui32ColorBackground;
    uint32_t        ui32ColorFrame;
    uint32_t        ui32ColorSeparator;
    uint32_t        ui32ColorTitle;
    uint32_t        ui32ColorText;
    char            *pcTitle;
    char            *pcFwName;
    char            *pcFwVersion;
    char            *pcFwDate;
    const uint8_t   *pu8ImageData;
    int32_t         i32ImageX;
    int32_t         i32ImageY;
} tLcdFwInfo;



// Function prototypes.
void LcdInit(tContext *psContext);
void LcdSetOrientation(tContext *psContext, uint8_t ui8Orientation);
void LcdClear(tContext *psContext, uint32_t ui32Color);
void LcdDrawPixel(tContext *psContext, int32_t i32X, int32_t i32Y,
                  uint32_t ui32Color);
void LcdDrawLine(tContext *psContext, int32_t i32X1, int32_t i32Y1,
                 int32_t i32X2, int32_t i32Y2, uint32_t ui32Color);
void LcdDrawRect(tContext *psContext, int16_t i16X1, int16_t i16Y1,
                 int16_t i16X2, int16_t i16Y2, uint32_t ui32Color, bool bFill);
void LcdDrawCircle(tContext *psContext, int32_t i32X, int32_t i32Y,
                   int32_t iRadius, uint32_t ui32Color, bool bFill);
void LcdDrawText(tContext *psContext, const char *pcText, int32_t i32X,
                 int32_t i32Y, uint32_t ui32Color, bool bCenter);
void LcdFontSet(tContext *psContext, const tFont *pFnt);
void LcdFwInfo(tLcdFwInfo *psLcdFwInfo);



#endif  // __LCD_H__

