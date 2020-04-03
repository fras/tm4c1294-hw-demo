// File: hw_demo.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 07 Feb 2020
// Rev.: 03 Apr 2020
//
// Hardware demo for the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "hw/adc/adc.h"
#include "hw/gpio/gpio.h"
#include "hw/gpio/gpio_button.h"
#include "hw/gpio/gpio_led.h"
#include "hw/i2c/i2c.h"
#include "hw/i2c/i2c_tmp006.h"
#include "hw/i2c/i2c_opt3001.h"
#include "hw/lcd/images.h"
#include "hw/lcd/lcd.h"
#include "hw/pwm/pwm_rgb_led.h"
#include "hw/system/system.h"
#include "hw/uart/uart.h"
#include "hw_demo.h"
#include "uart_ui.h"



// The error routine that is called if the driver library eniCnters an error.
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("FATAL: Unhandled error occured in file `%s', line %d.\n", pcFilename, ui32Line);
}
#endif



// Function prototypes.
void Help(void);
void Info(void);
int AdcRead(char *cCmd, char *cParam);
int LcdCmd(tLcdFwInfo *psLcdFwInfo, char *cCmd, char *cParam);
int LcdCheckParamCnt(char *cLcdCmd, int iLcdParamCntActual, int iLcdParamCntTarget);
void LcdHelp(void);
int LedSet(char *cCmd, char *cParam);
int RgbLedSet(char *cCmd, char *cParam);
int I2CAccess(char *cCmd, char *cParam);
int TemperatureRead(char *cCmd, char *cParam);
int IlluminanceRead(char *cCmd, char *cParam);
int UartAccess(char *cCmd, char *cParam);



// Initialize hardware, get and process commands.
int main(void)
{
    uint32_t ui32SysClock;
    char cUartStr[UI_STR_BUF_SIZE];
    char *cUartCmd;
    char *cUartParam;

    // Setup the system clock.
    ui32SysClock = MAP_SysCtlClockFreqSet(SYSTEM_CLOCK_SETTINGS, SYSTEM_CLOCK_FREQ);

    // Initialize the UART for the user interface.
    UartUiInit(ui32SysClock);

    // Initialize the ADCs.
    AdcReset(&adcJoystickX);
    AdcInit(&adcJoystickX);
    AdcInit(&adcJoystickY);
    AdcReset(&adcAccelX);
    AdcInit(&adcAccelX);
    AdcInit(&adcAccelY);
    AdcInit(&adcAccelZ);

    // Initialize the user buttons.
    GpioButtonInit();

    // Initialize the LEDs.
    GpioLedInit();

    // Initialize the RGB LED on the Educational BoosterPack MK II.
    PwmRgbLedInit();

    // Initialize the I2C master for the Educational BoosterPack MK II.
    i2c2.ui32SysClock = ui32SysClock;
    I2C2MasterInit(&i2c2);

    // Initialize the I2C devices.
    I2CTmp006Reset(&i2c2, EDUMKII_I2C_TMP006_SLV_ADR);
    I2CTmp006Init(&i2c2, EDUMKII_I2C_TMP006_SLV_ADR);
    I2COpt3001Reset(&i2c2, EDUMKII_I2C_OPT3001_SLV_ADR);
    I2COpt3001Init(&i2c2, EDUMKII_I2C_OPT3001_SLV_ADR);

    // Initialize the UART.
    uartBoosterPack2.ui32SysClock = ui32SysClock;
    UartInit(&uartBoosterPack2);

    // Initialize the LCD on the Educational BoosterPack MKII.
    tContext sContext;
    LcdInit(&sContext);
    LcdSetOrientation(&sContext, LCD_ORIENTATION_UP);
    tLcdFwInfo sLcdFwInfo;
    sLcdFwInfo.psContext        = &sContext;
    sLcdFwInfo.colorBackground  = LCD_COLOR_WHITE;
    sLcdFwInfo.colorFrame       = LCD_COLOR_MPG_GREY_DARK;
    sLcdFwInfo.colorSeparator   = LCD_COLOR_MPG_GREEN_2;
    sLcdFwInfo.colorTitle       = LCD_COLOR_MPG_GREEN_1;
    sLcdFwInfo.colorText        = LCD_COLOR_BLACK;
    sLcdFwInfo.strTitle         = "TIVA TM4C1294";
    sLcdFwInfo.strFwName        = FW_NAME;
    sLcdFwInfo.strFwVersion     = FW_VERSION;
    sLcdFwInfo.strFwDate        = FW_RELEASEDATE;
    sLcdFwInfo.imageData        = g_pui8LogoMpp;
//    sLcdFwInfo.imageX           = LCD_FRAME_PADDING + LCD_FRAME_WIDTH + LCD_CONTENT_PADDING;
    sLcdFwInfo.imageX           = -1;           // -1 = Auto center image.
    sLcdFwInfo.imageY           = 72;
    LcdFwInfo(&sLcdFwInfo);

    // Send initial information to UART.
    UARTprintf("\n\n**********************************************************************\n");
    UARTprintf("TIVA TM4C1294 %s firmware version %s, release date: %s\n", FW_NAME, FW_VERSION, FW_RELEASEDATE);
    UARTprintf("**********************************************************************\n\n");
    UARTprintf("Type `help' to get an overview of available commands.\n");

    while(1)
    {
        UARTprintf("%s", UI_COMMAND_PROMPT);
        UARTgets(cUartStr, UI_STR_BUF_SIZE);
        cUartCmd = strtok(cUartStr, UI_STR_DELIMITER);
        cUartParam = strtok(NULL, UI_STR_DELIMITER);
        if (cUartCmd == NULL) {
            continue;
        } else if (!strcasecmp(cUartCmd, "help")) {
            Help();
        } else if (!strcasecmp(cUartCmd, "info")) {
            Info();
        // ADC based functions.
        } else if (!strcasecmp(cUartCmd, "adc")) {
            AdcRead(cUartCmd, cUartParam);
        // I2C based functions.
        } else if (!strcasecmp(cUartCmd, "i2c")) {
            I2CAccess(cUartCmd, cUartParam);
        } else if (!strcasecmp(cUartCmd, "temp")) {
            TemperatureRead(cUartCmd, cUartParam);
        } else if (!strcasecmp(cUartCmd, "illum")) {
            IlluminanceRead(cUartCmd, cUartParam);
        // LCD based functions.
        } else if (!strcasecmp(cUartCmd, "lcd")) {
            LcdCmd(&sLcdFwInfo, cUartCmd, cUartParam);
        // GPIO based functions.
        } else if (!strcasecmp(cUartCmd, "led")) {
            LedSet(cUartCmd, cUartParam);
        // PWM based functions.
        } else if (!strcasecmp(cUartCmd, "rgb")) {
            RgbLedSet(cUartCmd, cUartParam);
        // UART based functions.
        } else if (!strcasecmp(cUartCmd, "uart")) {
            UartAccess(cUartCmd, cUartParam);
//        } else if (!strcasecmp(cUartCmd, "sysclk")) {
//            cUartParam = strtok(NULL, UI_STR_DELIMITER);
//            if (cUartParam == NULL) {
//                UARTprintf("ERROR: Parameter required after command `%s'.", cUartCmd);
//            } else {
//                ui32User0 = strtoul(cUartParam, (char **) NULL, 0);
//                ui32User0 &= 0xff;
//                // Disable or enable the system clock output on the DIVSCLK pin (PQ4).
//                if (ui32User0 == 0) ClockOutputDisable();
//                else ClockOutputEnable(ui32User0);
//                UARTprintf("%s.", UI_STR_OK);
//            }
        } else {
            UARTprintf("ERROR: Unknown command `%s'.", cUartCmd);
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
    UARTprintf("  i2c     PORT SLV-ADR R/W NUM|DATA   I2C access (0 = write, 1 = read).\n");
    UARTprintf("  illum   [COUNT]                     Read ambient light sensor info.\n");
    UARTprintf("  info                                Show information about this firmware.\n");
    UARTprintf("  lcd     CMD PARAMS                  LCD commands.\n");
    UARTprintf("  led     VALUE                       Set the LEDs.\n");
    UARTprintf("  rgb     VALUE                       Set the RGB LED.\n");
    UARTprintf("  temp    [COUNT]                     Read temperature sensor info.\n");
    UARTprintf("  uart    PORT R/W NUM|DATA           UART access (0 = write, 1 = read).");
}



// Show information.
void Info(void)
{
    UARTprintf("TIVA TM4C1294 %s firmware version %s, release date: %s\n", FW_NAME, FW_VERSION, FW_RELEASEDATE);
    UARTprintf("It was compiled using gcc %s at %s on %s.", __VERSION__, __TIME__, __DATE__);
}



// Read values from the ADCs.
int AdcRead(char *cCmd, char *cParam)
{
    uint32_t ui32Adc;
    int iCnt;

    if (cParam == NULL) {
        iCnt = 1;
    } else {
        iCnt = strtoul(cParam, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < iCnt; i++) {
        UARTprintf("%s: ", UI_STR_OK);
        #ifdef ADC_VALUES_HEX
        // Joystick.
        ui32Adc = AdcConvert(&adcJoystickX);
        UARTprintf("Joystick: X = 0x%03x ", ui32Adc);
        ui32Adc = AdcConvert(&adcJoystickY);
        UARTprintf("Y = 0x%03x", ui32Adc);
        // Accelerometer.
        ui32Adc = AdcConvert(&adcAccelX);
        UARTprintf(" ; Accelerometer: X = 0x%03x", ui32Adc);
        ui32Adc = AdcConvert(&adcAccelY);
        UARTprintf(" Y = 0x%03x", ui32Adc);
        ui32Adc = AdcConvert(&adcAccelZ);
        UARTprintf(" Z = 0x%03x", ui32Adc);
        #else
        // Joystick.
        ui32Adc = AdcConvert(&adcJoystickX);
        UARTprintf("Joystick: X = %4d ", ui32Adc);
        ui32Adc = AdcConvert(&adcJoystickY);
        UARTprintf("Y = %4d", ui32Adc);
        // Accelerometer.
        ui32Adc = AdcConvert(&adcAccelX);
        UARTprintf(" ; Accelerometer: X = %4d", ui32Adc);
        ui32Adc = AdcConvert(&adcAccelY);
        UARTprintf(" Y = %4d", ui32Adc);
        ui32Adc = AdcConvert(&adcAccelZ);
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
int LcdCmd(tLcdFwInfo *psLcdFwInfo, char *cCmd, char *cParam)
{
    char *cLcdCmd = cParam;
    #define LCD_CMD_PARAM_MAX 64
    char *cLcdParam[LCD_CMD_PARAM_MAX];
    int iLcdParamCnt = 0;
    int iRet = 0;

    if (cLcdCmd == NULL) {
        UARTprintf("%s: LCD command required after command `%s'.", UI_STR_ERROR, cCmd);
        return -1;
    }
    // Parse parameters of the LCD command.
    for (int i = 0; i < LCD_CMD_PARAM_MAX; i++) {
        iLcdParamCnt = i;
        cLcdParam[i] = strtok(NULL, UI_STR_DELIMITER);
        if (cLcdParam[i] == NULL) break;
        // Stop splitting the string for the text command, as the last
        // parameter is of the type string a may contain the deliminator
        // string.
        if (!strcasecmp(cLcdCmd, "text") && (i == 3)) {
            // Set pointer of the 5th parameter (the text message) beyond the
            // terminating 0 of the 4th parameter.
            cLcdParam[i+1] = cLcdParam[i] + strlen(cLcdParam[i]) + 1;
            // Set the correct parameter count.
            iLcdParamCnt = 5;
            break;
        }
    }
    // Show help on LCD commands.
    if (!strcasecmp(cLcdCmd, "help")) {
        LcdHelp();
    // Show info.
    } else if (!strcasecmp(cLcdCmd, "info")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 0)) < 0) return iRet;
        LcdFwInfo(psLcdFwInfo);
    // Draw circle.
    } else if (!strcasecmp(cLcdCmd, "circle")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 5)) < 0) return iRet;
        LcdDrawCircle(psLcdFwInfo->psContext,
                      strtoul(cLcdParam[0], (char **) NULL, 0),
                      strtoul(cLcdParam[1], (char **) NULL, 0),
                      strtoul(cLcdParam[2], (char **) NULL, 0),
                      strtoul(cLcdParam[3], (char **) NULL, 0),
                      strtoul(cLcdParam[4], (char **) NULL, 0));
    // Fill the display with a color.
    } else if (!strcasecmp(cLcdCmd, "clear")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 1)) < 0) return iRet;
        LcdClear(psLcdFwInfo->psContext, strtoul(cLcdParam[0], (char **) NULL, 0));
    // Draw a line.
    } else if (!strcasecmp(cLcdCmd, "line")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 5)) < 0) return iRet;
        LcdDrawLine(psLcdFwInfo->psContext,
                    strtoul(cLcdParam[0], (char **) NULL, 0),
                    strtoul(cLcdParam[1], (char **) NULL, 0),
                    strtoul(cLcdParam[2], (char **) NULL, 0),
                    strtoul(cLcdParam[3], (char **) NULL, 0),
                    strtoul(cLcdParam[4], (char **) NULL, 0));
    // Change the orientation of the display.
    } else if (!strcasecmp(cLcdCmd, "orient")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 1)) < 0) return iRet;
        LcdSetOrientation(psLcdFwInfo->psContext, strtoul(cLcdParam[0], (char **) NULL, 0));
    // Draw a pixel.
    } else if (!strcasecmp(cLcdCmd, "pixel")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 3)) < 0) return iRet;
        LcdDrawPixel(psLcdFwInfo->psContext,
                     strtoul(cLcdParam[0], (char **) NULL, 0),
                     strtoul(cLcdParam[1], (char **) NULL, 0),
                     strtoul(cLcdParam[3], (char **) NULL, 0));
    // Draw a rectangle.
    } else if (!strcasecmp(cLcdCmd, "rect")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 6)) < 0) return iRet;
        LcdDrawRect(psLcdFwInfo->psContext,
                    strtoul(cLcdParam[0], (char **) NULL, 0),
                    strtoul(cLcdParam[1], (char **) NULL, 0),
                    strtoul(cLcdParam[2], (char **) NULL, 0),
                    strtoul(cLcdParam[3], (char **) NULL, 0),
                    strtoul(cLcdParam[4], (char **) NULL, 0),
                    strtoul(cLcdParam[5], (char **) NULL, 0));
    // Draw a text.
    } else if (!strcasecmp(cLcdCmd, "text")) {
        if ((iRet = LcdCheckParamCnt(cLcdCmd, iLcdParamCnt, 5)) < 0) return iRet;
        LcdDrawText(psLcdFwInfo->psContext,
                    cLcdParam[4],       // The text message.
                    strtoul(cLcdParam[0], (char **) NULL, 0),
                    strtoul(cLcdParam[1], (char **) NULL, 0),
                    strtoul(cLcdParam[2], (char **) NULL, 0),
                    strtoul(cLcdParam[3], (char **) NULL, 0));
    } else {
        UARTprintf("%s: Unknown LCD command `%s'!\n", UI_STR_ERROR, cLcdCmd);
        LcdHelp();
        return -1;
    }
    if (iRet == 0) UARTprintf("%s.", UI_STR_OK);
    return iRet;
}



// Check the number of parameters given for an LCD command.
int LcdCheckParamCnt(char *cLcdCmd, int iLcdParamCntActual, int iLcdParamCntTarget)
{
    if (iLcdParamCntActual < iLcdParamCntTarget) {
        UARTprintf("%s: The LCD command `%s' requires %d parameter%s, but only %d %s given!",
                   UI_STR_WARNING, cLcdCmd, iLcdParamCntTarget, (iLcdParamCntTarget == 1) ? "" : "s",
                   iLcdParamCntActual, (iLcdParamCntActual == 1) ? "was" : "were");
        return -1;
    } else if (iLcdParamCntActual > iLcdParamCntTarget) {
        UARTprintf("%s: The LCD command `%s' requires %d parameter%s, but %d %s given! Ignoring the last %d parameter%s.",
                   UI_STR_WARNING, cLcdCmd, iLcdParamCntTarget, (iLcdParamCntTarget == 1) ? "" : "s",
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



// Set the user LEDs.
int LedSet(char *cCmd, char *cParam)
{
    uint32_t ui32LedSet, ui32LedGet;

    if (cParam == NULL) {
        UARTprintf("%s: Parameter required after command `%s'.", UI_STR_ERROR, cCmd);
        return -1;
    }
    ui32LedSet = strtoul(cParam, (char **) NULL, 0);
    GpioLedSet(ui32LedSet);
    ui32LedGet = GpioLedGet();
    if (ui32LedSet != ui32LedGet) {
        UARTprintf("%s: Setting the LEDs to 0x%02x failed!", UI_STR_ERROR, ui32LedSet);
        UARTprintf(" The LEDs were set to 0x%02x instead.", ui32LedGet);
        return -1;
    } else {
        UARTprintf("%s: LEDs set to 0x%02x.", UI_STR_OK, GpioLedGet());
        return 0;
    }
}



// Set the RGB LED on the Educational BoosterPack MK II.
int RgbLedSet(char *cCmd, char *cParam)
{
    uint32_t ui32RgbLedSet;

    if (cParam == NULL) {
        UARTprintf("%s: Parameter required after command `%s'.", UI_STR_ERROR, cCmd);
        return -1;
    } else {
        ui32RgbLedSet = strtoul(cParam, (char **) NULL, 0) & 0xffffff;
        PwmRgbLedSet((ui32RgbLedSet & 0xff0000) >> 16, (ui32RgbLedSet & 0x00ff00) >> 8, (ui32RgbLedSet & 0x0000ff) >> 0);
        UARTprintf("%s: RGB LED set to 0x%06x.", UI_STR_OK, ui32RgbLedSet);
    }

    return 0;
}



// I2C access.
int I2CAccess(char *cCmd, char *cParam)
{
    int i;
    tI2C *i2c;
    uint8_t ui8I2CPort = 0;
    uint8_t ui8I2CSlaveAddr = 0;
    uint8_t ui8I2CRw = 0;   // 0 = write; 1 = read
    uint8_t ui8I2CDataNum = 0;
    uint8_t ui8I2CData[128];
    uint32_t ui32I2CMasterStatus;
    // Parse cParameters.
    for (i = 0; i < 128; i++) {
        if (i != 0) cParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (cParam == NULL) {
                UARTprintf("%s: I2C port number required after command `%s'.", UI_STR_ERROR, cCmd);
                break;
            } else {
                ui8I2CPort = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (cParam == NULL) {
                UARTprintf("%s: I2C slave address required after command `%s'.", UI_STR_ERROR, cCmd);
                break;
            } else {
                ui8I2CSlaveAddr = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 2) {
            if (cParam == NULL) {
                UARTprintf("%s: I2C read/write required after command `%s'.", UI_STR_ERROR, cCmd);
                break;
            } else {
                ui8I2CRw = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0x01;
            }
        } else {
            if (i == 3 && ui8I2CRw == 0 && cParam == NULL) {
                UARTprintf("%s: At least one data byte required after I2C write command `%s'.", UI_STR_ERROR, cCmd);
                return -1;
            }
            if (cParam == NULL) break;
            else ui8I2CData[i-3] = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0xff;
        }
    }
    if (i < 3) return -1;
    // Check if the I2C port number is valid.
    switch (ui8I2CPort) {
        case 2: i2c = &i2c2; break;
        default:
            UARTprintf("%s: Only I2C port number 2 is supported!", UI_STR_ERROR);
            return -1;
    }
    // I2C write.
    if (ui8I2CRw == 0) {
        ui32I2CMasterStatus = I2CMasterWrite(i2c, ui8I2CSlaveAddr, ui8I2CData, i - 3);
    // I2C read.
    } else {
        if (i == 3) ui8I2CDataNum = 1;
        else ui8I2CDataNum = ui8I2CData[0];
        ui32I2CMasterStatus = I2CMasterRead(i2c, ui8I2CSlaveAddr, ui8I2CData, ui8I2CDataNum);
    }
    // Check the I2C status.
    if (ui32I2CMasterStatus) {
        UARTprintf("%s: Error flags from I2C master: 0x%08x", UI_STR_ERROR, ui32I2CMasterStatus);
        if (ui32I2CMasterStatus & I2C_MASTER_INT_TIMEOUT) UARTprintf("\n%s: I2C timeout.", UI_STR_ERROR);
        if (ui32I2CMasterStatus & I2C_MASTER_INT_NACK) UARTprintf("\n%s: NACK received.", UI_STR_ERROR);
        if (ui32I2CMasterStatus & I2C_MASTER_INT_ARB_LOST) UARTprintf("\n%s: I2C bus arbitration lost.", UI_STR_ERROR);
        if (ui32I2CMasterStatus & 0x1) UARTprintf("\n%s: Unknown error.", UI_STR_ERROR);
    } else {
        UARTprintf("%s.", UI_STR_OK);
        if (ui8I2CRw) {
            UARTprintf(" Data:");
            for (i = 0; i < ui8I2CDataNum; i++) UARTprintf(" 0x%02x", ui8I2CData[i]);
        }
    }

    return 0;
}



// Read values from the TMP006 temperature sensor on the Educational
// BoosterPack MK II.
int TemperatureRead(char *cCmd, char *cParam)
{
    float fTemp;
    uint32_t ui32Tmp006ManufacturerId;
    uint32_t ui32Tmp006DeviceId;
    int iCnt;

    if (cParam == NULL) {
        iCnt = 1;
    } else {
        iCnt = strtoul(cParam, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < iCnt; i++) {
        fTemp = I2CTmp006ReadTemp(&i2c2, EDUMKII_I2C_TMP006_SLV_ADR);
        ui32Tmp006ManufacturerId = I2CTmp006ReadManufacturerId(&i2c2, EDUMKII_I2C_TMP006_SLV_ADR);
        ui32Tmp006DeviceId = I2CTmp006ReadDeviceId(&i2c2, EDUMKII_I2C_TMP006_SLV_ADR);
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
int IlluminanceRead(char *cCmd, char *cParam)
{
    float fIlluminance;
    uint32_t ui32Opt3001ManufacturerId;
    uint32_t ui32Opt3001DeviceId;
    int iCnt;

    if (cParam == NULL) {
        iCnt = 1;
    } else {
        iCnt = strtoul(cParam, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < iCnt; i++) {
        fIlluminance = I2COpt3001ReadIlluminance(&i2c2, EDUMKII_I2C_OPT3001_SLV_ADR);
        ui32Opt3001ManufacturerId = I2COpt3001ReadManufacturerId(&i2c2, EDUMKII_I2C_OPT3001_SLV_ADR);
        ui32Opt3001DeviceId = I2COpt3001ReadDeviceId(&i2c2, EDUMKII_I2C_OPT3001_SLV_ADR);
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



// UART access.
int UartAccess(char *cCmd, char *cParam)
{
    int i;
    uint8_t ui8UartPort = 0;
    uint8_t ui8UartRw = 0;   // 0 = write; 1 = read
    uint8_t ui8UartDataNum = 0;
    uint8_t ui8UartData[128];
    tUART *uart;
    uint32_t ui32UartStatus;
    // Parse cParameters.
    for (i = 0; i < 128; i++) {
        if (i != 0) cParam = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (cParam == NULL) {
                UARTprintf("%s: UART port number required after command `%s'.", UI_STR_ERROR, cCmd);
                break;
            } else {
                ui8UartPort = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (cParam == NULL) {
                UARTprintf("%s: UART read/write required after command `%s'.", UI_STR_ERROR, cCmd);
                break;
            } else {
                ui8UartRw = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0x01;
            }
        } else {
            if (i == 2 && ui8UartRw == 0 && cParam == NULL) {
                UARTprintf("%s: At least one data byte required after UART write command `%s'.", UI_STR_ERROR, cCmd);
                return -1;
            }
            if (cParam == NULL) break;
            else ui8UartData[i-2] = (uint8_t) strtoul(cParam, (char **) NULL, 0) & 0xff;
        }
    }
    if (i < 2) return -1;
    // Check if the UART port number is valid.
    switch (ui8UartPort) {
        case 6: uart = &uartBoosterPack2; break;
        default:
            UARTprintf("%s: Only UART port number 6 is supported!", UI_STR_ERROR);
            return -1;
    }
    // UART write.
    if (ui8UartRw == 0) {
        ui32UartStatus = UartWrite(uart, ui8UartData, i - 2);
        // Check the UART status.
        if (ui32UartStatus) {
            UARTprintf("%s: Error flags from the UART: 0x%08x", UI_STR_ERROR, ui32UartStatus);
        } else {
            UARTprintf("%s.", UI_STR_OK);
        }
    // UART read.
    } else {
        if (i == 2) ui8UartDataNum = 1;
        else ui8UartDataNum = ui8UartData[0];
        ui32UartStatus = UartRead(uart, ui8UartData, ui8UartDataNum);
        // Check the UART status.
        if (ui32UartStatus != ui8UartDataNum) {
            UARTprintf("%s: Could only read %d data bytes from the UART instead of %d.", UI_STR_ERROR, ui32UartStatus, ui8UartDataNum);
        } else {
            UARTprintf("%s.", UI_STR_OK);
        }
        if (ui32UartStatus > 0) {
            UARTprintf(" Data:");
            for (i = 0; i < ui32UartStatus; i++) UARTprintf(" 0x%02x", ui8UartData[i]);
        }
    }
    
    return 0;
}

