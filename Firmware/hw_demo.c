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
#include "hw/pwm/pwm_rgb_led.h"
#include "hw/system/system.h"
#include "hw/uart/uart.h"
#include "hw_demo.h"
#include "uart_ui.h"



// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("FATAL: Unhandled error occured in file `%s', line %d.\n", pcFilename, ui32Line);
}
#endif



// Function prototypes.
void Help(void);
void Info(void);
int AdcRead(char *cmd, char *param);
int LedSet(char *cmd, char *param);
int RgbLedSet(char *cmd, char *param);
int I2CAccess(char *cmd, char *param);
int TemperatureRead(char *cmd, char *param);
int IlluminanceRead(char *cmd, char *param);
int UartAccess(char *cmd, char *param);



// Initialize hardware, get and process commands.
int main(void)
{
    uint32_t ui32SysClock;
    char uart_str[UI_STR_BUF_SIZE];
    char *uart_cmd;
    char *uart_param;

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

    // Send initial information.
    UARTprintf("\n\n**********************************************************************\n");
    UARTprintf("TIVA TM4C1294 %s firmware version %s, release date: %s\n", PROG_NAME, PROG_VERSION, PROG_RELEASEDATE);
    UARTprintf("**********************************************************************\n\n");
    UARTprintf("Type `help' to get an overview of available commands.\n");

    while(1)
    {
        UARTprintf("%s", UI_COMMAND_PROMPT);
        UARTgets(uart_str, UI_STR_BUF_SIZE);
        uart_cmd = strtok(uart_str, UI_STR_DELIMITER);
        uart_param = strtok(NULL, UI_STR_DELIMITER);
        if (uart_cmd == NULL) {
            continue;
        } else if (!strcasecmp(uart_cmd, "help")) {
            Help();
        } else if (!strcasecmp(uart_cmd, "info")) {
            Info();
        // ADC based functions.
        } else if (!strcasecmp(uart_cmd, "adc")) {
            AdcRead(uart_cmd, uart_param);
        // GPIO based functions.
        } else if (!strcasecmp(uart_cmd, "led")) {
            LedSet(uart_cmd, uart_param);
        // I2C based functions.
        } else if (!strcasecmp(uart_cmd, "i2c")) {
            I2CAccess(uart_cmd, uart_param);
        } else if (!strcasecmp(uart_cmd, "temp")) {
            TemperatureRead(uart_cmd, uart_param);
        } else if (!strcasecmp(uart_cmd, "illum")) {
            IlluminanceRead(uart_cmd, uart_param);
        // PWM based functions.
        } else if (!strcasecmp(uart_cmd, "rgb")) {
            RgbLedSet(uart_cmd, uart_param);
        // UART based functions.
        } else if (!strcasecmp(uart_cmd, "uart")) {
            UartAccess(uart_cmd, uart_param);
//        } else if (!strcasecmp(uart_cmd, "sysclk")) {
//            uart_param = strtok(NULL, UI_STR_DELIMITER);
//            if (uart_param == NULL) {
//                UARTprintf("ERROR: Parameter required after command `%s'.", uart_cmd);
//            } else {
//                ui32User0 = strtoul(uart_param, (char **) NULL, 0);
//                ui32User0 &= 0xff;
//                // Disable or enable the system clock output on the DIVSCLK pin (PQ4).
//                if (ui32User0 == 0) ClockOutputDisable();
//                else ClockOutputEnable(ui32User0);
//                UARTprintf("%s.", UI_STR_OK);
//            }
        } else {
            UARTprintf("ERROR: Unknown command `%s'.", uart_cmd);
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
    UARTprintf("  led     VALUE                       Set the LEDs.\n");
    UARTprintf("  rgb     VALUE                       Set the RGB LED.\n");
    UARTprintf("  temp    [COUNT]                     Read temperature sensor info.\n");
    UARTprintf("  uart    PORT R/W NUM|DATA           UART access (0 = write, 1 = read).");
}



// Show information.
void Info(void)
{
    UARTprintf("TIVA TM4C1294 %s firmware version %s, release date: %s\n", PROG_NAME, PROG_VERSION, PROG_RELEASEDATE);
    UARTprintf("It was compiled using gcc %s at %s on %s.", __VERSION__, __TIME__, __DATE__);
}



// Read values from the ADCs.
int AdcRead(char *cmd, char *param)
{
    uint32_t ui32Adc;
    int count;

    if (param == NULL) {
        count = 1;
    } else {
        count = strtoul(param, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < count; i++) {
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
        if (i < count - 1) {
            SysCtlDelay(1000000);
            UARTprintf("\n");
        }
    }

    return 0;
}



// Set the user LEDs.
int LedSet(char *cmd, char *param)
{
    uint32_t ui32LedSet, ui32LedGet;

    if (param == NULL) {
        UARTprintf("%s: Parameter required after command `%s'.", UI_STR_ERROR, cmd);
        return -1;
    }
    ui32LedSet = strtoul(param, (char **) NULL, 0);
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
int RgbLedSet(char *cmd, char *param)
{
    uint32_t ui32RgbLedSet;

    if (param == NULL) {
        UARTprintf("%s: Parameter required after command `%s'.", UI_STR_ERROR, cmd);
        return -1;
    } else {
        ui32RgbLedSet = strtoul(param, (char **) NULL, 0) & 0xffffff;
        PwmRgbLedSet((ui32RgbLedSet & 0xff0000) >> 16, (ui32RgbLedSet & 0x00ff00) >> 8, (ui32RgbLedSet & 0x0000ff) >> 0);
        UARTprintf("%s: RGB LED set to 0x%06x.", UI_STR_OK, ui32RgbLedSet);
    }

    return 0;
}



// I2C access.
int I2CAccess(char *cmd, char *param)
{
    int i;
    tI2C *i2c;
    uint8_t ui8I2CPort = 0;
    uint8_t ui8I2CSlaveAddr = 0;
    uint8_t ui8I2CRw = 0;   // 0 = write; 1 = read
    uint8_t ui8I2CDataNum = 0;
    uint8_t ui8I2CData[128];
    uint32_t ui32I2CMasterStatus;
    // Parse parameters.
    for (i = 0; i < 128; i++) {
        if (i != 0) param = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (param == NULL) {
                UARTprintf("%s: I2C port number required after command `%s'.", UI_STR_ERROR, cmd);
                break;
            } else {
                ui8I2CPort = (uint8_t) strtoul(param, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (param == NULL) {
                UARTprintf("%s: I2C slave address required after command `%s'.", UI_STR_ERROR, cmd);
                break;
            } else {
                ui8I2CSlaveAddr = (uint8_t) strtoul(param, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 2) {
            if (param == NULL) {
                UARTprintf("%s: I2C read/write required after command `%s'.", UI_STR_ERROR, cmd);
                break;
            } else {
                ui8I2CRw = (uint8_t) strtoul(param, (char **) NULL, 0) & 0x01;
            }
        } else {
            if (i == 3 && ui8I2CRw == 0 && param == NULL) {
                UARTprintf("%s: At least one data byte required after I2C write command `%s'.", UI_STR_ERROR, cmd);
                return -1;
            }
            if (param == NULL) break;
            else ui8I2CData[i-3] = (uint8_t) strtoul(param, (char **) NULL, 0) & 0xff;
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
int TemperatureRead(char *cmd, char *param)
{
    float fTemp;
    uint32_t ui32Tmp006ManufacturerId;
    uint32_t ui32Tmp006DeviceId;
    int count;

    if (param == NULL) {
        count = 1;
    } else {
        count = strtoul(param, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < count; i++) {
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
        if (i < count - 1) {
            SysCtlDelay(1000000);
            UARTprintf("\n");
        }
    }

    return 0;
}



// Read values from the OPT3001 ambient light sensor on the Educational
// BoosterPack MK II.
int IlluminanceRead(char *cmd, char *param)
{
    float fIlluminance;
    uint32_t ui32Opt3001ManufacturerId;
    uint32_t ui32Opt3001DeviceId;
    int count;

    if (param == NULL) {
        count = 1;
    } else {
        count = strtoul(param, (char **) NULL, 0) & 0xffffff;
    }

    for (int i = 0; i < count; i++) {
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
        if (i < count - 1) {
            SysCtlDelay(1000000);
            UARTprintf("\n");
        }
    }

    return 0;
}



// UART access.
int UartAccess(char *cmd, char *param)
{
    int i;
    uint8_t ui8UartPort = 0;
    uint8_t ui8UartRw = 0;   // 0 = write; 1 = read
    uint8_t ui8UartDataNum = 0;
    uint8_t ui8UartData[128];
    tUART *uart;
    uint32_t ui32UartStatus;
    // Parse parameters.
    for (i = 0; i < 128; i++) {
        if (i != 0) param = strtok(NULL, UI_STR_DELIMITER);
        if (i == 0) {
            if (param == NULL) {
                UARTprintf("%s: UART port number required after command `%s'.", UI_STR_ERROR, cmd);
                break;
            } else {
                ui8UartPort = (uint8_t) strtoul(param, (char **) NULL, 0) & 0xff;
            }
        } else if (i == 1) {
            if (param == NULL) {
                UARTprintf("%s: UART read/write required after command `%s'.", UI_STR_ERROR, cmd);
                break;
            } else {
                ui8UartRw = (uint8_t) strtoul(param, (char **) NULL, 0) & 0x01;
            }
        } else {
            if (i == 2 && ui8UartRw == 0 && param == NULL) {
                UARTprintf("%s: At least one data byte required after UART write command `%s'.", UI_STR_ERROR, cmd);
                return -1;
            }
            if (param == NULL) break;
            else ui8UartData[i-2] = (uint8_t) strtoul(param, (char **) NULL, 0) & 0xff;
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

