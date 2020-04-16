// File: hw_demo.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 16 Apr 2020
//
// Hardware demo for the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "inc/hw_memmap.h"
#include "driverlib/i2c.h"
#include "driverlib/rom_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "hw/adc/adc.h"
#include "hw/gpio/gpio.h"
#include "hw/gpio/gpio_button.h"
#include "hw/gpio/gpio_led.h"
#include "hw/i2c/i2c.h"
#include "hw/i2c/i2c_tmp006.h"
#include "hw/i2c/i2c_opt3001.h"
#include "hw/lcd/Crystalfontz128x128_ST7735.h"
#include "hw/lcd/images.h"
#include "hw/lcd/lcd.h"
#include "hw/pwm/pwm_rgb_led.h"
#include "hw/ssi/ssi.h"
#include "hw/system/system.h"
#include "hw/uart/uart.h"
#include "uart_ui.h"
#include "hw_demo.h"
#include "hw_demo_io.h"



// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("%s: Unhandled error occured in file `%s', line %d.\n", UI_STR_FATAL, pcFilename, ui32Line);
}
#endif



// Function prototypes.
void Help(void);
void Info(void);
int AdcRead(char *pcCmd, char *pcParam);
int LcdCmd(tLcdFwInfo *psLcdFwInfo, char *pcCmd, char *pcParam);
int LcdCheckParamCnt(char *pcLcdCmd, int iLcdParamCntActual, int iLcdParamCntTarget);
void LcdHelp(void);
int LedSet(char *pcCmd, char *pcParam);
int RgbLedSet(char *pcCmd, char *pcParam);
int I2CAccess(char *pcCmd, char *pcParam);
void I2CAccessHelp(void);
int I2CPortCheck(uint8_t ui8I2CPort, tI2C **psI2C);
int I2CDetect(char *pcCmd, char *pcParam);
int TemperatureRead(char *pcCmd, char *pcParam);
int IlluminanceRead(char *pcCmd, char *pcParam);
int SsiAccess(char *pcCmd, char *pcParam);
int SsiPortCheck(uint8_t ui8SsiPort, tSSI **psSsi);
int SsiSetup(char *pcCmd, char *pcParam);
void SsiSetupHelp(void);
int UartAccess(char *pcCmd, char *pcParam);
int UartPortCheck(uint8_t ui8UartPort, tUART **psUart);
int UartSetup(char *pcCmd, char *pcParam);
void UartSetupHelp(void);



// Initialize hardware, get and process commands.
int main(void)
{
    uint32_t ui32SysClock;
    char pcUartStr[UI_STR_BUF_SIZE];
    char *pcUartCmd;
    char *pcUartParam;

    // Setup the system clock.
    ui32SysClock = MAP_SysCtlClockFreqSet(SYSTEM_CLOCK_SETTINGS, SYSTEM_CLOCK_FREQ);

    // Initialize the UART for the user interface.
    g_sUartUi.ui32SrcClock = ui32SysClock;
    UartUiInit(&g_sUartUi);

    // Initialize the ADCs.
    AdcReset(&g_sAdcJoystickX);
    AdcInit(&g_sAdcJoystickX);
    AdcInit(&g_sAdcJoystickY);
    AdcReset(&g_sAdcAccelX);
    AdcInit(&g_sAdcAccelX);
    AdcInit(&g_sAdcAccelY);
    AdcInit(&g_sAdcAccelZ);

    // Initialize the user buttons.
    GpioButtonInit();

    // Initialize the Leds.
    GpioLedInit();

    // Initialize the RGB LED on the Educational BoosterPack MK II.
    PwmRgbLedInit();

    // Initialize the I2C master for the BoosterPack 1 socket.
    g_sI2C0.ui32I2CClk = ui32SysClock;
    I2CMasterInit(&g_sI2C0);

    // Initialize the I2C master for the Educational BoosterPack MK II
    // (BoosterPack 2 socket).
    g_sI2C2.ui32I2CClk = ui32SysClock;
    I2CMasterInit(&g_sI2C2);

    // Initialize the I2C devices.
    I2CTmp006Reset(&g_sI2C2, EDUMKII_I2C_TMP006_SLV_ADR);
    I2CTmp006Init(&g_sI2C2, EDUMKII_I2C_TMP006_SLV_ADR);
    I2COpt3001Reset(&g_sI2C2, EDUMKII_I2C_OPT3001_SLV_ADR);
    I2COpt3001Init(&g_sI2C2, EDUMKII_I2C_OPT3001_SLV_ADR);

    // Initialize SSI 2 for BoosterPack 1.
    g_sSsi2.ui32SsiClk = ui32SysClock;
    SsiMasterInit(&g_sSsi2);

    // Initialize SSI 3 for BoosterPack 2.
    g_sSsi3.ui32SsiClk = ui32SysClock;
    SsiMasterInit(&g_sSsi3);

    // Initialize the UART on the Educational BoosterPack MKII.
    g_sUart6.ui32UartClk = ui32SysClock;
//    g_sUart6.bLoopback = true;        // Enable loopback for testing.
    UartInit(&g_sUart6);

    // Initialize the LCD on the Educational BoosterPack MKII.
    tContext sContext;
    LcdInit(&sContext);
    LcdSetOrientation(&sContext, LCD_ORIENTATION_UP);
    tLcdFwInfo sLcdFwInfo;
    sLcdFwInfo.psContext            = &sContext;
    sLcdFwInfo.ui32ColorBackground  = LCD_COLOR_WHITE;
    sLcdFwInfo.ui32ColorFrame       = LCD_COLOR_MPG_GREY_DARK;
    sLcdFwInfo.ui32ColorSeparator   = LCD_COLOR_MPG_GREEN_2;
    sLcdFwInfo.ui32ColorTitle       = LCD_COLOR_MPG_GREEN_1;
    sLcdFwInfo.ui32ColorText        = LCD_COLOR_BLACK;
    sLcdFwInfo.pcTitle              = "TIVA TM4C1294";
    sLcdFwInfo.pcFwName             = FW_NAME;
    sLcdFwInfo.pcFwVersion          = FW_VERSION;
    sLcdFwInfo.pcFwDate             = FW_RELEASEDATE;
    sLcdFwInfo.pu8ImageData         = g_pui8LogoMpp;
//    sLcdFwInfo.i32ImageX            = LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING;
    sLcdFwInfo.i32ImageX            = -1;           // -1 = Auto center image.
    sLcdFwInfo.i32ImageY            = 72;
    LcdFwInfo(&sLcdFwInfo);

    // Send initial information to UART.
    UARTprintf("\n\n*******************************************************************************\n");
    UARTprintf("TIVA TM4C1294 `%s' firmware version %s, release date: %s\n", FW_NAME, FW_VERSION, FW_RELEASEDATE);
    UARTprintf("*******************************************************************************\n\n");
    UARTprintf("Type `help' to get an overview of available commands.\n");

    while(1)
    {
        UARTprintf("%s", UI_COMMAND_PROMPT);
        UARTgets(pcUartStr, UI_STR_BUF_SIZE);
        pcUartCmd = strtok(pcUartStr, UI_STR_DELIMITER);
        pcUartParam = strtok(NULL, UI_STR_DELIMITER);
        if (pcUartCmd == NULL) {
            continue;
        } else if (!strcasecmp(pcUartCmd, "help")) {
            Help();
        } else if (!strcasecmp(pcUartCmd, "info")) {
            Info();
        // ADC based functions.
        } else if (!strcasecmp(pcUartCmd, "adc")) {
            AdcRead(pcUartCmd, pcUartParam);
        // I2C based functions.
        } else if (!strcasecmp(pcUartCmd, "i2c")) {
            I2CAccess(pcUartCmd, pcUartParam);
        } else if (!strcasecmp(pcUartCmd, "i2c-det")) {
            I2CDetect(pcUartCmd, pcUartParam);
        } else if (!strcasecmp(pcUartCmd, "temp")) {
            TemperatureRead(pcUartCmd, pcUartParam);
        } else if (!strcasecmp(pcUartCmd, "illum")) {
            IlluminanceRead(pcUartCmd, pcUartParam);
        // LCD based functions.
        } else if (!strcasecmp(pcUartCmd, "lcd")) {
            LcdCmd(&sLcdFwInfo, pcUartCmd, pcUartParam);
        // GPIO based functions.
        } else if (!strcasecmp(pcUartCmd, "led")) {
            LedSet(pcUartCmd, pcUartParam);
        // PWM based functions.
        } else if (!strcasecmp(pcUartCmd, "rgb")) {
            RgbLedSet(pcUartCmd, pcUartParam);
        // SSI based functions.
        } else if (!strcasecmp(pcUartCmd, "ssi")) {
            SsiAccess(pcUartCmd, pcUartParam);
        } else if (!strcasecmp(pcUartCmd, "ssi-set")) {
            SsiSetup(pcUartCmd, pcUartParam);
        // UART based functions.
        } else if (!strcasecmp(pcUartCmd, "uart")) {
            UartAccess(pcUartCmd, pcUartParam);
        } else if (!strcasecmp(pcUartCmd, "uart-s")) {
            UartSetup(pcUartCmd, pcUartParam);
//        } else if (!strcasecmp(pcUartCmd, "sysclk")) {
//            pcUartParam = strtok(NULL, UI_STR_DELIMITER);
//            if (pcUartParam == NULL) {
//                UARTprintf("ERROR: Parameter required after command `%s'.", pcUartCmd);
//            } else {
//                ui32User0 = strtoul(pcUartParam, (char **) NULL, 0);
//                ui32User0 &= 0xff;
//                // Disable or enable the system clock output on the DIVSCLK pin (PQ4).
//                if (ui32User0 == 0) ClockOutputDisable();
//                else ClockOutputEnable(ui32User0);
//                UARTprintf("%s.", UI_STR_OK);
//            }
        } else {
            UARTprintf("ERROR: Unknown command `%s'.", pcUartCmd);
        }
        UARTprintf("\n");
    }
}



// Show help.
void Help(void)
{
    UARTprintf("Available commands:\n");
    UARTprintf("  help                                Show this help text.\n");
    UARTprintf("  adc     [COUNT]                     Read ADC values.\n");
    UARTprintf("  i2c     PORT SLV-ADR ACC NUM|DATA   I2C access (ACC bits: R/W, Sr, nP, Q).\n");
    UARTprintf("  i2c-det PORT [MODE]                 I2C detect devices (MODE: 0 = auto,\n");
    UARTprintf("                                          1 = quick command, 2 = read).\n");
    UARTprintf("  illum   [COUNT]                     Read ambient light sensor info.\n");
    UARTprintf("  info                                Show information about this firmware.\n");
    UARTprintf("  lcd     CMD PARAMS                  LCD commands.\n");
    UARTprintf("  led     VALUE                       Set the Leds.\n");
    UARTprintf("  rgb     VALUE                       Set the RGB LED.\n");
    UARTprintf("  ssi     PORT R/W NUM|DATA           SSI/SPI access (R/W: 0 = write, 1 = read).\n");
    UARTprintf("  ssi-set PORT FREQ [MODE] [WIDTH]    Setup the SSI port.\n");
    UARTprintf("  temp    [COUNT]                     Read temperature sensor info.\n");
    UARTprintf("  uart    PORT R/W NUM|DATA           UART access (R/W: 0 = write, 1 = read).\n");
    UARTprintf("  uart-s  PORT BAUD [LOOP] [PARITY]   Setup the UART port.");
}



// Show information.
void Info(void)
{
    UARTprintf("TIVA TM4C1294 `%s' firmware version %s, release date: %s\n", FW_NAME, FW_VERSION, FW_RELEASEDATE);
    UARTprintf("It was compiled using gcc %s at %s on %s.", __VERSION__, __TIME__, __DATE__);
}



// Read values from the ADCs.
int AdcRead(char *pcCmd, char *pcParam)
{
    uint32_t ui32Adc;
    int iCnt;

    if (pcParam == NULL) {
        iCnt = 1;
    } else {
        iCnt = strtoul(pcParam, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < iCnt; i++) {
        UARTprintf("%s: ", UI_STR_OK);
        #ifdef ADC_VALUES_HEX
        // Joystick.
        ui32Adc = AdcConvert(&sAdcJoystickX);
        UARTprintf("Joystick: X = 0x%03x ", ui32Adc);
        ui32Adc = AdcConvert(&sAdcJoystickY);
        UARTprintf("Y = 0x%03x", ui32Adc);
        // Accelerometer.
        ui32Adc = AdcConvert(&sAdcAccelX);
        UARTprintf(" ; Accelerometer: X = 0x%03x", ui32Adc);
        ui32Adc = AdcConvert(&sAdcAccelY);
        UARTprintf(" Y = 0x%03x", ui32Adc);
        ui32Adc = AdcConvert(&sAdcAccelZ);
        UARTprintf(" Z = 0x%03x", ui32Adc);
        #else
        // Joystick.
        ui32Adc = AdcConvert(&g_sAdcJoystickX);
        UARTprintf("Joystick: X = %4d ", ui32Adc);
        ui32Adc = AdcConvert(&g_sAdcJoystickY);
        UARTprintf("Y = %4d", ui32Adc);
        // Accelerometer.
        ui32Adc = AdcConvert(&g_sAdcAccelX);
        UARTprintf(" ; Accelerometer: X = %4d", ui32Adc);
        ui32Adc = AdcConvert(&g_sAdcAccelY);
        UARTprintf(" Y = %4d", ui32Adc);
        ui32Adc = AdcConvert(&g_sAdcAccelZ);
        UARTprintf(" Z = %4d", ui32Adc);
        #endif
        if (i < iCnt - 1) {
            SysCtlDelay(1000000);
            UARTprintf("\n");
        }
    }

    return 0;
}



// LCD commands.
int LcdCmd(tLcdFwInfo *psLcdFwInfo, char *pcCmd, char *pcParam)
{
    char *pcLcdCmd = pcParam;
    #define LCD_CMD_PARAM_MAX 64
    char *pcLcdParam[LCD_CMD_PARAM_MAX];
    int iLcdParamCnt = 0;
    int iRet = 0;

    // String to store the LCD text parameter.
    char pcLcdText[UI_STR_BUF_SIZE] = "";

    if (pcLcdCmd == NULL) {
        UARTprintf("%s: LCD command required after command `%s'.", UI_STR_ERROR, pcCmd);
        return -1;
    }
    // Parse parameters of the LCD command.
    for (int i = 0; i < LCD_CMD_PARAM_MAX; i++) {
        iLcdParamCnt = i;
        pcLcdParam[i] = strtok(NULL, UI_STR_DELIMITER);
        if (pcLcdParam[i] == NULL) break;
        // Stop splitting the string for the text command, as the last
        // parameter is of the type string a may contain the deliminator
        // string.
        if (!strcasecmp(pcLcdCmd, "text") && (i == 3)) {
            // Pointer to string past terminating 0 of parameter 4;
            char *pcTmp = pcLcdParam[i] + strlen(pcLcdParam[i]) + 1;
            // Cut off deliminators at the beginning.
            pcTmp += strspn(pcTmp, UI_STR_DELIMITER);
            // Copy text parameter without leading deliminator to text parameter string.
            strncpy(pcLcdText, pcTmp, UI_STR_BUF_SIZE - 1);
            if (strtok(NULL, UI_STR_DELIMITER) != 0) {
                iLcdParamCnt = 5;
            } else {
                iLcdParamCnt = 4;
            }
            break;
        }
    }
    // Show help on LCD commands.
    if (!strcasecmp(pcLcdCmd, "help")) {
        LcdHelp();
    // Show info.
    } else if (!strcasecmp(pcLcdCmd, "info")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 0)) < 0) return iRet;
        LcdFwInfo(psLcdFwInfo);
    // Draw circle.
    } else if (!strcasecmp(pcLcdCmd, "circle")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 5)) < 0) return iRet;
        LcdDrawCircle(psLcdFwInfo->psContext,
                      strtoul(pcLcdParam[0], (char **) NULL, 0),
                      strtoul(pcLcdParam[1], (char **) NULL, 0),
                      strtoul(pcLcdParam[2], (char **) NULL, 0),
                      strtoul(pcLcdParam[3], (char **) NULL, 0),
                      strtoul(pcLcdParam[4], (char **) NULL, 0));
    // Fill the display with a color.
    } else if (!strcasecmp(pcLcdCmd, "clear")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 1)) < 0) return iRet;
        LcdClear(psLcdFwInfo->psContext, strtoul(pcLcdParam[0], (char **) NULL, 0));
    // Draw a line.
    } else if (!strcasecmp(pcLcdCmd, "line")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 5)) < 0) return iRet;
        LcdDrawLine(psLcdFwInfo->psContext,
                    strtoul(pcLcdParam[0], (char **) NULL, 0),
                    strtoul(pcLcdParam[1], (char **) NULL, 0),
                    strtoul(pcLcdParam[2], (char **) NULL, 0),
                    strtoul(pcLcdParam[3], (char **) NULL, 0),
                    strtoul(pcLcdParam[4], (char **) NULL, 0));
    // Change the orientation of the display.
    } else if (!strcasecmp(pcLcdCmd, "orient")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 1)) < 0) return iRet;
        LcdSetOrientation(psLcdFwInfo->psContext, strtoul(pcLcdParam[0], (char **) NULL, 0));
    // Draw a pixel.
    } else if (!strcasecmp(pcLcdCmd, "pixel")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 3)) < 0) return iRet;
        LcdDrawPixel(psLcdFwInfo->psContext,
                     strtoul(pcLcdParam[0], (char **) NULL, 0),
                     strtoul(pcLcdParam[1], (char **) NULL, 0),
                     strtoul(pcLcdParam[3], (char **) NULL, 0));
    // Draw a rectangle.
    } else if (!strcasecmp(pcLcdCmd, "rect")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 6)) < 0) return iRet;
        LcdDrawRect(psLcdFwInfo->psContext,
                    strtoul(pcLcdParam[0], (char **) NULL, 0),
                    strtoul(pcLcdParam[1], (char **) NULL, 0),
                    strtoul(pcLcdParam[2], (char **) NULL, 0),
                    strtoul(pcLcdParam[3], (char **) NULL, 0),
                    strtoul(pcLcdParam[4], (char **) NULL, 0),
                    strtoul(pcLcdParam[5], (char **) NULL, 0));
    // Draw a text.
    } else if (!strcasecmp(pcLcdCmd, "text")) {
        if ((iRet = LcdCheckParamCnt(pcLcdCmd, iLcdParamCnt, 5)) < 0) return iRet;
        LcdDrawText(psLcdFwInfo->psContext,
                    pcLcdText,       // The text message.
                    strtoul(pcLcdParam[0], (char **) NULL, 0),
                    strtoul(pcLcdParam[1], (char **) NULL, 0),
                    strtoul(pcLcdParam[2], (char **) NULL, 0),
                    strtoul(pcLcdParam[3], (char **) NULL, 0));
    } else {
        UARTprintf("%s: Unknown LCD command `%s'!\n", UI_STR_ERROR, pcLcdCmd);
        LcdHelp();
        return -1;
    }
    if (iRet == 0) UARTprintf("%s.", UI_STR_OK);
    return iRet;
}



// Check the number of parameters given for an LCD command.
int LcdCheckParamCnt(char *pcLcdCmd, int iLcdParamCntActual, int iLcdParamCntTarget)
{
    if (iLcdParamCntActual < iLcdParamCntTarget) {
        UARTprintf("%s: The LCD command `%s' requires %d parameter%s, but only %d %s given!",
                   UI_STR_WARNING, pcLcdCmd, iLcdParamCntTarget, (iLcdParamCntTarget == 1) ? "" : "s",
                   iLcdParamCntActual, (iLcdParamCntActual == 1) ? "was" : "were");
        return -1;
    } else if (iLcdParamCntActual > iLcdParamCntTarget) {
        UARTprintf("%s: The LCD command `%s' requires %d parameter%s, but %d %s given! Ignoring the last %d parameter%s.",
                   UI_STR_WARNING, pcLcdCmd, iLcdParamCntTarget, (iLcdParamCntTarget == 1) ? "" : "s",
                   iLcdParamCntActual, (iLcdParamCntActual == 1) ? "was" : "were",
                   iLcdParamCntActual - iLcdParamCntTarget,
                   (iLcdParamCntActual - iLcdParamCntTarget == 1) ? "" : "s");
        return 1;
    }
    return 0;
}



// Show help on LCD commands.
void LcdHelp(void)
{
    UARTprintf("Available LCD commands:\n");
    UARTprintf("  help                                Show this help text.\n");
    UARTprintf("  info                                Show firmware info.\n");
    UARTprintf("  circle  X Y RADIUS COLOR FILL       Draw a circle.\n");
    UARTprintf("  clear   COLOR                       Fill the LCD with COLOR.\n");
    UARTprintf("  line    X1 Y1 X2 Y2 COLOR           Draw a line.\n");
    UARTprintf("  orient  ORIENT                      Set the LCD orientation.\n");
    UARTprintf("  pixel   X Y COLOR                   Draw a pixel.\n");
    UARTprintf("  rect    X1 Y1 X2 Y2 COLOR FILL      Draw a rectangle.\n");
    UARTprintf("  text    X Y COLOR CENTER TEXT       Draw a text message.");
}



// Set the user Leds.
int LedSet(char *pcCmd, char *pcParam)
{
    uint32_t ui32LedSet, ui32LedGet;

    if (pcParam == NULL) {
        UARTprintf("%s: Parameter required after command `%s'.", UI_STR_ERROR, pcCmd);
        return -1;
    }
    ui32LedSet = strtoul(pcParam, (char **) NULL, 0);
    GpioLedSet(ui32LedSet);
    ui32LedGet = GpioLedGet();
    if (ui32LedSet != ui32LedGet) {
        UARTprintf("%s: Setting the Leds to 0x%02x failed!", UI_STR_ERROR, ui32LedSet);
        UARTprintf(" The Leds were set to 0x%02x instead.", ui32LedGet);
        return -1;
    } else {
        UARTprintf("%s: Leds set to 0x%02x.", UI_STR_OK, GpioLedGet());
        return 0;
    }
}



// Set the RGB LED on the Educational BoosterPack MK II.
int RgbLedSet(char *pcCmd, char *pcParam)
{
    uint32_t ui32RgbLedSet;

    if (pcParam == NULL) {
        UARTprintf("%s: Parameter required after command `%s'.", UI_STR_ERROR, pcCmd);
        return -1;
    } else {
        ui32RgbLedSet = strtoul(pcParam, (char **) NULL, 0) & 0xffffff;
        PwmRgbLedSet((ui32RgbLedSet & 0xff0000) >> 16, (ui32RgbLedSet & 0x00ff00) >> 8, (ui32RgbLedSet & 0x0000ff) >> 0);
        UARTprintf("%s: RGB LED set to 0x%06x.", UI_STR_OK, ui32RgbLedSet);
    }

    return 0;
}



// I2C access.
int I2CAccess(char *pcCmd, char *pcParam)
{
    int i;
    tI2C *psI2C;
    uint8_t ui8I2CPort = 0;
    uint8_t ui8I2CSlaveAddr = 0;
    uint8_t ui8I2CAccMode = 0;
    uint8_t ui8I2CRw = 0;           // 0 = write; 1 = read
    bool bI2CRepeatedStart = false; // Repeated start.
    bool bI2CStop = true;           // Generate stop condition.
    bool bI2CQuickCmd = false;      // Quick command.
    uint8_t ui8I2CDataNum = 0;
    uint8_t pui8I2CData[32];
    uint32_t ui32I2CMasterStatus;
    // Parse parameters.
    for (i = 0; i < sizeof(pui8I2CData) / sizeof(pui8I2CData[0]); i++) {
        if (i != 0) pcParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (pcParam == NULL) {
                UARTprintf("%s: I2C port number required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                I2CAccessHelp();
                return -1;
            } else {
                ui8I2CPort = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (pcParam == NULL) {
                UARTprintf("%s: I2C slave address required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                I2CAccessHelp();
                return -1;
            } else {
                ui8I2CSlaveAddr = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 2) {
            if (pcParam == NULL) {
                UARTprintf("%s: I2C access mode required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                I2CAccessHelp();
                return -1;
            } else {
                ui8I2CAccMode = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0x0f;
                ui8I2CRw = ui8I2CAccMode & 0x1;
                bI2CRepeatedStart = (ui8I2CAccMode & 0x2) ? true : false;
                bI2CStop = (ui8I2CAccMode & 0x4) ? false : true;
                bI2CQuickCmd = (ui8I2CAccMode & 0x8) ? true : false;
            }
        } else {
            if (i == 3 && ui8I2CRw == 0 && !bI2CQuickCmd && pcParam == NULL) {
                UARTprintf("%s: At least one data byte required after I2C write command `%s'.\n", UI_STR_ERROR, pcCmd);
                I2CAccessHelp();
                return -1;
            }
            if (pcParam == NULL) break;
            else pui8I2CData[i-3] = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
        }
    }
    if (i < 3) return -1;
    // Check if the I2C port number is valid. If so, set the psI2C pointer to the selected I2C port struct.
    if (I2CPortCheck(ui8I2CPort, &psI2C)) return -1;
    // I2C quick command.
    if (bI2CQuickCmd) {
        ui32I2CMasterStatus = I2CMasterQuickCmdAdv(psI2C, ui8I2CSlaveAddr, ui8I2CRw, bI2CRepeatedStart);
    // Standard I2C access.
    } else {
        // I2C write.
        if (ui8I2CRw == 0) {
            ui32I2CMasterStatus = I2CMasterWriteAdv(psI2C, ui8I2CSlaveAddr, pui8I2CData, i - 3, bI2CRepeatedStart, bI2CStop);
        // I2C read.
        } else {
            if (i == 3) ui8I2CDataNum = 1;
            else ui8I2CDataNum = pui8I2CData[0];
            if (ui8I2CDataNum > sizeof(pui8I2CData) / sizeof(pui8I2CData[0])) {
                ui8I2CDataNum = sizeof(pui8I2CData) / sizeof(pui8I2CData[0]);
            }
            ui32I2CMasterStatus = I2CMasterReadAdv(psI2C, ui8I2CSlaveAddr, pui8I2CData, ui8I2CDataNum, bI2CRepeatedStart, bI2CStop);
        }
    }
    // Check the I2C status.
    if (ui32I2CMasterStatus) {
        UARTprintf("%s: Error flags from I2C the master %d: 0x%08x", UI_STR_ERROR, ui8I2CPort, ui32I2CMasterStatus);
        if (ui32I2CMasterStatus & I2C_MASTER_INT_TIMEOUT) UARTprintf("\n%s: I2C timeout.", UI_STR_ERROR);
        if (ui32I2CMasterStatus & I2C_MASTER_INT_NACK) UARTprintf("\n%s: NACK received.", UI_STR_ERROR);
        if (ui32I2CMasterStatus & I2C_MASTER_INT_ARB_LOST) UARTprintf("\n%s: I2C bus arbitration lost.", UI_STR_ERROR);
        if (ui32I2CMasterStatus & 0x1) UARTprintf("\n%s: Unknown error.", UI_STR_ERROR);
    } else {
        UARTprintf("%s.", UI_STR_OK);
        if (ui8I2CRw && !bI2CQuickCmd) {
            UARTprintf(" Data:");
            for (i = 0; i < ui8I2CDataNum; i++) UARTprintf(" 0x%02x", pui8I2CData[i]);
        }
    }

    return 0;
}



// Show help on I2C access command.
void I2CAccessHelp(void)
{
    UARTprintf("I2C access command:\n");
    UARTprintf("  i2c     PORT SLV-ADR ACC NUM|DATA   I2C access (ACC bits: R/W, Sr, nP, Q).\n");
    UARTprintf("I2C access mode (ACC) bits:\n");
    UARTprintf("  0: Read/write (R/W)                 0 = write, 1 = read\n");
    UARTprintf("  1: Repeated start (Sr)              0 = no Sr, 1 = Sr\n");
    UARTprintf("  2: No stop condition (nP)           0 = generate stop cond. P, 1 = omit P\n");
    UARTprintf("  3: Quick command (Q)                0 = no Q, 1 = Q");
}



// Check if the I2C port number is valid. If so, set the psI2C pointer to the
// selected I2C port struct.
int I2CPortCheck(uint8_t ui8I2CPort, tI2C **psI2C)
{
    switch (ui8I2CPort) {
        case 0: *psI2C = &g_sI2C0; break;
        case 2: *psI2C = &g_sI2C2; break;
        default:
            *psI2C = NULL;
            UARTprintf("%s: Only I2C port numbers 0 and 2 are supported!", UI_STR_ERROR);
            return -1;
    }
    return 0;
}



// Detect I2C devices.
int I2CDetect(char *pcCmd, char *pcParam)
{
    int i;
    tI2C *psI2C;
    uint8_t ui8I2CPort = 0;
    uint8_t ui8I2CSlaveAddr = 0;
    uint8_t ui8I2CDetectMode = 0;   // 0 = auto; 1 = quick command; 2 = read
    uint8_t pui8I2CData[1];
    uint32_t ui32I2CMasterStatus;
    // Parse parameters.
    for (i = 0; i < 4; i++) {
        if (i != 0) pcParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (pcParam == NULL) {
                UARTprintf("%s: I2C port number required after command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            } else {
                ui8I2CPort = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1 && pcParam != NULL) {
            ui8I2CDetectMode = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0x0f;
        } else {
            break;
        }
    }
    if (i < 1) return -1;
    // Check if the I2C port number is valid. If so, set the psI2C pointer to the selected I2C port struct.
    if (I2CPortCheck(ui8I2CPort, &psI2C)) return -1;
    // Detect I2C devices based on the i2cdetect program of the i2c-tools.
    // Please see: https://github.com/mozilla-b2g/i2c-tools/blob/master/tools/i2cdetect.c
    UARTprintf("%s. I2C device(s) found at slave address:", UI_STR_OK);
    for (ui8I2CSlaveAddr = 1; ui8I2CSlaveAddr <= 0x7f; ui8I2CSlaveAddr++) {
        switch (ui8I2CDetectMode) {
            // Detection mode using I2C quick command.
            case 1:
                ui32I2CMasterStatus = I2CMasterQuickCmd(psI2C, ui8I2CSlaveAddr, false);   // false = write; true = read
            // Detection mode using I2C read command.
            case 2:
                ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8I2CSlaveAddr, pui8I2CData, 1);
            // Automatic mode. Use I2C quick command or I2C read based on the slave address.
            default:
                if ((ui8I2CSlaveAddr >= 0x30 && ui8I2CSlaveAddr <= 0x37)
                    || (ui8I2CSlaveAddr >= 0x50 && ui8I2CSlaveAddr <= 0x5F)) {
                    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8I2CSlaveAddr, pui8I2CData, 1);
                } else {
                    ui32I2CMasterStatus = I2CMasterQuickCmd(psI2C, ui8I2CSlaveAddr, false);   // false = write; true = read
                }
        }
        if (!ui32I2CMasterStatus) UARTprintf(" 0x%02x", ui8I2CSlaveAddr);
    }

    return 0;
}



// Read values from the TMP006 temperature sensor on the Educational
// BoosterPack MK II.
int TemperatureRead(char *pcCmd, char *pcParam)
{
    float fTemp;
    uint32_t ui32Tmp006ManufacturerId;
    uint32_t ui32Tmp006DeviceId;
    int iCnt;

    if (pcParam == NULL) {
        iCnt = 1;
    } else {
        iCnt = strtoul(pcParam, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < iCnt; i++) {
        fTemp = I2CTmp006ReadTemp(&g_sI2C2, EDUMKII_I2C_TMP006_SLV_ADR);
        ui32Tmp006ManufacturerId = I2CTmp006ReadManufacturerId(&g_sI2C2, EDUMKII_I2C_TMP006_SLV_ADR);
        ui32Tmp006DeviceId = I2CTmp006ReadDeviceId(&g_sI2C2, EDUMKII_I2C_TMP006_SLV_ADR);
        if ((ui32Tmp006ManufacturerId == ~0) || (ui32Tmp006DeviceId == ~0)) {
            UARTprintf("%s: Cannot read from the TMP006 IC.", UI_STR_ERROR);
            return -1;
        } else {
            UARTprintf("%s: Temperature = %s%d.%05d *C ; Manufacturer ID = 0x%04x ; Device ID = 0x%04x",
                        UI_STR_OK,
                        fTemp < 0 ? "-" : "", (int) abs(fTemp), (int) abs((fTemp - (int) fTemp) * 100000),
                        ui32Tmp006ManufacturerId, ui32Tmp006DeviceId);
        }
        if (i < iCnt - 1) {
            SysCtlDelay(1000000);
            UARTprintf("\n");
        }
    }

    return 0;
}



// Read values from the OPT3001 ambient light sensor on the Educational
// BoosterPack MK II.
int IlluminanceRead(char *pcCmd, char *pcParam)
{
    float fIlluminance;
    uint32_t ui32Opt3001ManufacturerId;
    uint32_t ui32Opt3001DeviceId;
    int iCnt;

    if (pcParam == NULL) {
        iCnt = 1;
    } else {
        iCnt = strtoul(pcParam, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < iCnt; i++) {
        fIlluminance = I2COpt3001ReadIlluminance(&g_sI2C2, EDUMKII_I2C_OPT3001_SLV_ADR);
        ui32Opt3001ManufacturerId = I2COpt3001ReadManufacturerId(&g_sI2C2, EDUMKII_I2C_OPT3001_SLV_ADR);
        ui32Opt3001DeviceId = I2COpt3001ReadDeviceId(&g_sI2C2, EDUMKII_I2C_OPT3001_SLV_ADR);
        if ((ui32Opt3001ManufacturerId == ~0) || (ui32Opt3001DeviceId == ~0)) {
            UARTprintf("%s: Cannot read from the OPT3001 IC.", UI_STR_ERROR);
            return -1;
        } else {
            UARTprintf("%s: Illuminance = %d.%02d lux ; Manufacturer ID = 0x%04x ; Device ID = 0x%04x",
                        UI_STR_OK,
                        (int) abs(fIlluminance), (int) abs((fIlluminance - (int) fIlluminance) * 100),
                        ui32Opt3001ManufacturerId, ui32Opt3001DeviceId);
        }
        if (i < iCnt - 1) {
            SysCtlDelay(1000000);
            UARTprintf("\n");
        }
    }

    return 0;
}



// SSI access.
int SsiAccess(char *pcCmd, char *pcParam)
{
    int i;
    uint8_t ui8SsiPort = 0;
    uint8_t ui8SsiRw = 0;   // 0 = write; 1 = read
    uint8_t ui8SsiDataNum = 0;
    uint32_t pui32SsiData[32];
    tSSI *psSsi;
    int32_t i32SsiStatus;
    // Parse parameters.
    for (i = 0; i < sizeof(pui32SsiData) / sizeof(pui32SsiData[0]); i++) {
        if (i != 0) pcParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (pcParam == NULL) {
                UARTprintf("%s: SSI port number required after command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            } else {
                ui8SsiPort = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (pcParam == NULL) {
                UARTprintf("%s: SSI read/write required after command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            } else {
                ui8SsiRw = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0x01;
            }
        } else {
            if (i == 2 && ui8SsiRw == 0 && pcParam == NULL) {
                UARTprintf("%s: At least one data byte required after SSI write command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            }
            if (pcParam == NULL) break;
            else pui32SsiData[i-2] = (uint32_t) strtoul(pcParam, (char **) NULL, 0) & 0xffff;
        }
    }
    if (i < 2) return -1;
    // Check if the SSI port number is valid. If so, set the psSsi pointer to the selected SSI port struct.
    if (SsiPortCheck(ui8SsiPort, &psSsi)) return -1;
    // SSI write.
    if (ui8SsiRw == 0) {
        i32SsiStatus = SsiMasterWrite(psSsi, pui32SsiData, i - 2);
        // Check the SSI status.
        if (i32SsiStatus) {
            UARTprintf("%s: Error status from the SSI master %d: %d", UI_STR_ERROR, ui8SsiPort, i32SsiStatus);
        } else {
            UARTprintf("%s.", UI_STR_OK);
        }
    // SSI read.
    } else {
        // Read all available data.
        if (i == 2) {
            for (int iCnt = 0; ; iCnt++) {
                i32SsiStatus = SsiMasterRead(psSsi, pui32SsiData, 1);
                if (i32SsiStatus != 1) {
                    if (iCnt == 0) UARTprintf("%s: No data available.", UI_STR_WARNING);
                    break;
                } else {
                    if (iCnt == 0) UARTprintf("%s. Data:", UI_STR_OK);
                    UARTprintf(" 0x%02x", pui32SsiData[0]);
                }
            }
        // Read given number of data.
        } else {
             ui8SsiDataNum = pui32SsiData[0];
            if (ui8SsiDataNum > sizeof(pui32SsiData) / sizeof(pui32SsiData[0])) {
                ui8SsiDataNum = sizeof(pui32SsiData) / sizeof(pui32SsiData[0]);
            }
            i32SsiStatus = SsiMasterRead(psSsi, pui32SsiData, ui8SsiDataNum);
            // Check the SSI status.
            if (i32SsiStatus != ui8SsiDataNum) {
                UARTprintf("%s: Could only read %d data bytes from the SSI master %d instead of %d.", UI_STR_WARNING, i32SsiStatus, ui8SsiPort, ui8SsiDataNum);
            } else {
                UARTprintf("%s.", UI_STR_OK);
            }
            if (i32SsiStatus > 0) {
                UARTprintf(" Data:");
                for (i = 0; i < i32SsiStatus; i++) UARTprintf(" 0x%02x", pui32SsiData[i]);
            }
        }
    }

    return 0;
}



// Check if the SSI port number is valid. If so, set the psSsi pointer to the
// selected SSI port struct.
int SsiPortCheck(uint8_t ui8SsiPort, tSSI **psSsi)
{
    switch (ui8SsiPort) {
        case 2: *psSsi = &g_sSsi2; break;
        case 3: *psSsi = &g_sSsi3; break;
        default:
            *psSsi = NULL;
            UARTprintf("%s: Only SSI port numbers 2 and 3 are supported!", UI_STR_ERROR);
            return -1;
    }
    return 0;
}



// Setup the SSI.
int SsiSetup(char *pcCmd, char *pcParam)
{
    int i;
    uint8_t ui8SsiPort = 0;
    uint32_t ui32SsiBitRate;
    uint32_t ui32SsiProtocol;
    uint32_t ui32SsiDataWidth;
    tSSI *psSsi;
    // Parse parameters.
    for (i = 0; i <= 3; i++) {
        if (i != 0) pcParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (pcParam == NULL) {
                UARTprintf("%s: SSI port number required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                SsiSetupHelp();
                return -1;
            } else {
                ui8SsiPort = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (pcParam == NULL) {
                UARTprintf("%s: SSI bit rate required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                SsiSetupHelp();
                return -1;
            } else {
                ui32SsiBitRate = (uint32_t) strtoul(pcParam, (char **) NULL, 0);
                if ((ui32SsiBitRate < SSI_FREQ_MIN) || (ui32SsiBitRate > SSI_FREQ_MAX)) {
                    UARTprintf("%s: SSI bit rate %d outside of valid range %d..%d.", UI_STR_ERROR, ui32SsiBitRate, SSI_FREQ_MIN, SSI_FREQ_MAX);
                    return -1;
                }
            }
        } else if (i == 2) {
            if (pcParam == NULL) {
                ui32SsiProtocol = SSI_FRF_MOTO_MODE_0;
            } else {
                ui32SsiProtocol = strtoul(pcParam, (char **) NULL, 0) & 0x07;
                switch (ui32SsiProtocol) {
                    case 0: ui32SsiProtocol = SSI_FRF_MOTO_MODE_0; break;
                    case 1: ui32SsiProtocol = SSI_FRF_MOTO_MODE_1; break;
                    case 2: ui32SsiProtocol = SSI_FRF_MOTO_MODE_2; break;
                    case 3: ui32SsiProtocol = SSI_FRF_MOTO_MODE_3; break;
                    case 4: ui32SsiProtocol = SSI_FRF_TI; break;
                    case 5: ui32SsiProtocol = SSI_FRF_NMW; break;
                    default:
                        UARTprintf("%s: Invalid SSI mode setting %d.", UI_STR_ERROR, ui32SsiProtocol);
                        return -1;
                }
            }
        } else if (i == 3) {
            if (pcParam == NULL) {
                ui32SsiDataWidth = 8;
            } else {
                ui32SsiDataWidth = strtoul(pcParam, (char **) NULL, 0);
                if ((ui32SsiDataWidth < SSI_DATAWIDTH_MIN) || (ui32SsiDataWidth > SSI_DATAWIDTH_MAX)) {
                    UARTprintf("%s: SSI data width %d outside of valid range %d..%d.", UI_STR_ERROR, ui32SsiDataWidth, SSI_DATAWIDTH_MIN, SSI_DATAWIDTH_MAX);
                    return -1;
                }
            }
        }
    }
    if (i < 1) return -1;
    // Check if the SSI port number is valid. If so, set the psSsi pointer to the selected SSI port struct.
    if (SsiPortCheck(ui8SsiPort, &psSsi)) return -1;
    // Setup the SSI port.
    psSsi->ui32BitRate = ui32SsiBitRate;
    psSsi->ui32Protocol = ui32SsiProtocol;
    psSsi->ui32DataWidth = ui32SsiDataWidth;
    SsiMasterInit(psSsi);
    UARTprintf("%s.", UI_STR_OK);

    return 0;
}



// Show help on SSI setup command.
void SsiSetupHelp(void)
{
    UARTprintf("SSI setup command:\n");
    UARTprintf("  ssi-set PORT FREQ [MODE] [WIDTH]    Setup the SSI port.\n");
    UARTprintf("SSI bit rate: %d..%d\n", SSI_FREQ_MIN, SSI_FREQ_MAX);
    UARTprintf("SSI modes:\n");
    UARTprintf("  0: SPI frame format, polarity = 0, phase = 0.\n");
    UARTprintf("  1: SPI frame format, polarity = 0, phase = 1.\n");
    UARTprintf("  2: SPI frame format, polarity = 1, phase = 0.\n");
    UARTprintf("  3: SPI frame format, polarity = 1, phase = 1.\n");
    UARTprintf("  4: TI frame format.\n");
    UARTprintf("  5: National MicroWire frame format.\n");
    UARTprintf("SSI data width: %d..%d", SSI_DATAWIDTH_MIN, SSI_DATAWIDTH_MAX);
}



// UART access.
int UartAccess(char *pcCmd, char *pcParam)
{
    int i;
    uint8_t ui8UartPort = 0;
    uint8_t ui8UartRw = 0;  // 0 = write; 1 = read
    uint8_t ui8UartDataNum = 0;
    uint8_t pui8UartData[32];
    tUART *psUart;
    int32_t i32UartStatus;
    // Parse parameters.
    for (i = 0; i < sizeof(pui8UartData) / sizeof(pui8UartData[0]); i++) {
        if (i != 0) pcParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (pcParam == NULL) {
                UARTprintf("%s: UART port number required after command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            } else {
                ui8UartPort = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (pcParam == NULL) {
                UARTprintf("%s: UART read/write required after command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            } else {
                ui8UartRw = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0x01;
            }
        } else {
            if (i == 2 && ui8UartRw == 0 && pcParam == NULL) {
                UARTprintf("%s: At least one data byte required after UART write command `%s'.", UI_STR_ERROR, pcCmd);
                return -1;
            }
            if (pcParam == NULL) break;
            else pui8UartData[i-2] = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
        }
    }
    if (i < 2) return -1;
    // Check if the UART port number is valid. If so, set the psUart pointer to the selected UART port struct.
    if (UartPortCheck(ui8UartPort, &psUart)) return -1;
    // UART write.
    if (ui8UartRw == 0) {
        i32UartStatus = UartWrite(psUart, pui8UartData, i - 2);
        // Check the UART status.
        if (i32UartStatus) {
            UARTprintf("%s: Error status from the UART %d: %d", UI_STR_ERROR, ui8UartPort, i32UartStatus);
        } else {
            UARTprintf("%s.", UI_STR_OK);
        }
    // UART read.
    } else {
        if (i == 2) ui8UartDataNum = 1;
        // Read all available data.
        if (i == 2) {
            for (int iCnt = 0; ; iCnt++) {
                i32UartStatus = UartRead(psUart, pui8UartData, 1);
                if (i32UartStatus != 1) {
                    if (iCnt == 0) UARTprintf("%s: No data available.", UI_STR_WARNING);
                    break;
                } else {
                    if (iCnt == 0) UARTprintf("%s. Data:", UI_STR_OK);
                    UARTprintf(" 0x%02x", pui8UartData[0]);
                }
            }
        // Read given number of data.
        } else {
            ui8UartDataNum = pui8UartData[0];
            if (ui8UartDataNum > sizeof(pui8UartData) / sizeof(pui8UartData[0])) {
                ui8UartDataNum = sizeof(pui8UartData) / sizeof(pui8UartData[0]);
            }
            i32UartStatus = UartRead(psUart, pui8UartData, ui8UartDataNum);
            // Check the UART status.
            if (i32UartStatus != ui8UartDataNum) {
                UARTprintf("%s: Could only read %d data bytes from the UART %d instead of %d.", UI_STR_WARNING, i32UartStatus, ui8UartPort, ui8UartDataNum);
            } else {
                UARTprintf("%s.", UI_STR_OK);
            }
            if (i32UartStatus > 0) {
                UARTprintf(" Data:");
                for (i = 0; i < i32UartStatus; i++) UARTprintf(" 0x%02x", pui8UartData[i]);
            }
        }
    }

    return 0;
}



// Check if the UART port number is valid. If so, set the psUart pointer to the
// selected UART port struct.
int UartPortCheck(uint8_t ui8UartPort, tUART **psUart)
{
    switch (ui8UartPort) {
        case 6: *psUart = &g_sUart6; break;
        default:
            *psUart = NULL;
            UARTprintf("%s: Only UART port number 6 is supported!", UI_STR_ERROR);
            return -1;
    }
    return 0;
}



// Setup the UART.
int UartSetup(char *pcCmd, char *pcParam)
{
    int i;
    uint8_t ui8UartPort = 0;
    uint32_t ui32UartBaud;
    uint32_t ui32UartParity;
    bool bUartLoopback;
    tUART *psUart;
    // Parse parameters.
    for (i = 0; i <= 3; i++) {
        if (i != 0) pcParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (pcParam == NULL) {
                UARTprintf("%s: UART port number required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                UartSetupHelp();
                return -1;
            } else {
                ui8UartPort = (uint8_t) strtoul(pcParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (pcParam == NULL) {
                UARTprintf("%s: UART baud rate required after command `%s'.\n", UI_STR_ERROR, pcCmd);
                UartSetupHelp();
                return -1;
            } else {
                ui32UartBaud = (uint32_t) strtoul(pcParam, (char **) NULL, 0);
                if ((ui32UartBaud < UART_BAUD_MIN) || (ui32UartBaud > UART_BAUD_MAX)) {
                    UARTprintf("%s: UART baud rate %d outside of valid range %d..%d.", UI_STR_ERROR, ui32UartBaud, UART_BAUD_MIN, UART_BAUD_MAX);
                    return -1;
                }
            }
        } else if (i == 2) {
            if (pcParam == NULL) {
                bUartLoopback = false;
            } else {
                bUartLoopback = (bool) strtoul(pcParam, (char **) NULL, 0) & 0x01;
            }
        } else if (i == 3) {
            if (pcParam == NULL) {
                ui32UartParity = UART_CONFIG_PAR_NONE;
            } else {
                ui32UartParity = strtoul(pcParam, (char **) NULL, 0) & 0x07;
                switch (ui32UartParity) {
                    case 0: ui32UartParity = UART_CONFIG_PAR_NONE; break;
                    case 1: ui32UartParity = UART_CONFIG_PAR_EVEN; break;
                    case 2: ui32UartParity = UART_CONFIG_PAR_ODD; break;
                    case 3: ui32UartParity = UART_CONFIG_PAR_ONE; break;
                    case 4: ui32UartParity = UART_CONFIG_PAR_ZERO; break;
                    default:
                        UARTprintf("%s: Invalid UART parity setting %d.", UI_STR_ERROR, ui32UartParity);
                        return -1;
                }
            }
        }
    }
    if (i < 1) return -1;
    // Check if the UART port number is valid. If so, set the psUart pointer to the selected UART port struct.
    if (UartPortCheck(ui8UartPort, &psUart)) return -1;
    // Setup the UART.
    psUart->ui32Baud = ui32UartBaud;
    psUart->bLoopback = bUartLoopback;
    UartInit(psUart);
    UARTParityModeSet(psUart->ui32BaseUart, ui32UartParity);
    UARTprintf("%s.", UI_STR_OK);

    return 0;
}



// Show help on UART setup command.
void UartSetupHelp(void)
{
    UARTprintf("UART setup command:\n");
    UARTprintf("  uart-s  PORT BAUD [LOOP] [PARITY]   Setup the UART.\n");
    UARTprintf("UART baud rate: %d..%d\n", UART_BAUD_MIN, UART_BAUD_MAX);
    UARTprintf("UART loopback options:\n");
    UARTprintf("  0: No loopback.\n");
    UARTprintf("  1: Enable internal loopback mode.\n");
    UARTprintf("UART partiy options:\n");
    UARTprintf("  0: None.\n");
    UARTprintf("  1: Even.\n");
    UARTprintf("  2: Odd.\n");
    UARTprintf("  3: One.\n");
    UARTprintf("  4: Zero.");
}

