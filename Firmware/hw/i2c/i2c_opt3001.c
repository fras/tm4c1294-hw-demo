// File: i2c_opt3001.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 13 Feb 2020
// Rev.: 04 Apr 2020
//
// I2C functions for the OPT3001 ambient light sensor on the Educational
// BoosterPack MK II connected to the TI Tiva TM4C1294 Connected LaunchPad
// Evaluation Kit.
//



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "i2c.h"



// Reset the OPT3001 sensor IC.
uint32_t I2COpt3001Reset(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[1] = {0x06};

    // The OPT3001 sensor IC accepts the general-call reset command. This is
    // done by writing the value 0x06 to slave address 0x00. For details see
    // the OPT3001 datasheet, section 7.5.1.2 "General-Call Reset Command".
    return I2CMasterWrite(psI2C, 0x00, ui8Data, 1);
}



// Initialize the OPT3001 sensor IC.
uint32_t I2COpt3001Init(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[3] = {0x01, 0xce, 0x10};

    return I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 3);
}



// Read the light illuminance from the OPT3001 sensor IC.
// Returns the light illuminance in lux.
float I2COpt3001ReadIlluminance(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t ui32IlluminanceRaw;
    uint32_t ui32IlluminanceMantissa, ui32IlluminanceExponent;
    float fIlluminance;

    // Set the pointer register of the OPT3001 IC to the result register (0x00).
    ui8Data[0] = 0x00;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return -255;

    // Read the result register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return -255;

    // Calculate the light illuminance in lux.
    ui32IlluminanceRaw = (uint32_t) (ui8Data[0] << 8) | ui8Data[1];
    ui32IlluminanceMantissa = ui32IlluminanceRaw & 0x0fff;
    ui32IlluminanceExponent = (ui32IlluminanceRaw & 0xf000) >> 12;
    fIlluminance = (float) ui32IlluminanceMantissa * 0.01 * (1 << ui32IlluminanceExponent);

    return fIlluminance;
}



// Read the manufacturer ID from the OPT3001 sensor IC.
uint32_t I2COpt3001ReadManufacturerId(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t ui32ManufacturerID;

    // Set the pointer register of the OPT3001 IC to the manufacturer ID register (0x7e).
    ui8Data[0] = 0x7e;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return ~0;

    // Read the manufacturer ID register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return ~0;

    // Calculate the manufacturer ID.
    ui32ManufacturerID = (uint32_t) (ui8Data[0] << 8) | (ui8Data[1]);

    return ui32ManufacturerID;
}



// Read the device ID from the OPT3001 sensor IC.
uint32_t I2COpt3001ReadDeviceId(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t ui32DeviceID;

    // Set the pointer register of the OPT3001 IC to the device ID register (0x7f).
    ui8Data[0] = 0x7f;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return ~0;

    // Read the device ID register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return ~0;

    // Calculate the device ID.
    ui32DeviceID = (uint32_t) (ui8Data[0] << 8) | (ui8Data[1]);

    return ui32DeviceID;
}

