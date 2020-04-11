// File: i2c_tmp006.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 12 Feb 2020
// Rev.: 11 Apr 2020
//
// I2C functions for the TMP006 temperature sensor on the Educational
// BoosterPack MK II connected to the TI Tiva TM4C1294 Connected LaunchPad
// Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "i2c.h"



// Reset the TMP006 sensor IC.
uint32_t I2CTmp006Reset(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[3] = {0x02, 0x80, 0x00};

    return I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 3);
}



// Initialize the TMP006 sensor IC.
uint32_t I2CTmp006Init(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[3] = {0x02, 0x75, 0x00};

    return I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 3);
}



// Read the sensor voltage from the TMP006 sensor IC.
// Returns the sensor voltage in nano Volts (nV).
float I2CTmp006ReadVolt(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t ui32VoltageRaw;
    int iVoltageSign;
    float fVoltage;

    // Set the pointer register of the TMP006 IC to the sensor voltage register (0x00).
    ui8Data[0] = 0x00;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return -255;

    // Read the sensor voltage register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return -255;

    // Calculate the sensor voltage in nano Volts (nV).
    ui32VoltageRaw = (uint32_t) (ui8Data[0] << 8) | ui8Data[1];
    // Negative sensor voltage.
    if (ui32VoltageRaw & 0x8000) {
        ui32VoltageRaw = (~ui32VoltageRaw + 1) & 0x7fff;
        iVoltageSign = -1;
    } else {
        iVoltageSign = 1;
    }
    fVoltage = (float) ui32VoltageRaw * 156.25 * iVoltageSign;

    return fVoltage;
}



// Read the temperature from the TMP006 sensor IC.
// Returns the temperature in degree Celcius.
float I2CTmp006ReadTemp(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t ui32TemperatureRaw;
    int iTemperatureSign;
    float fTemperature;

    // Set the pointer register of the TMP006 IC to the ambient temperature register (0x01).
    ui8Data[0] = 0x01;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return -255;

    // Read the ambient temperature register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return -255;

    // Calculate the temperature in degree Celcius.
    ui32TemperatureRaw = (uint32_t) (ui8Data[0] << 6) | (ui8Data[1] >> 2);
    // Negative temperature.
    if (ui32TemperatureRaw & 0x2000) {
        ui32TemperatureRaw = (~ui32TemperatureRaw + 1) & 0x1fff;
        iTemperatureSign = -1;
    } else {
        iTemperatureSign = 1;
    }
    fTemperature = (float) ui32TemperatureRaw / 32 * iTemperatureSign;

    return fTemperature;
}



// Read the manufacturer ID from the TMP006 sensor IC.
uint32_t I2CTmp006ReadManufacturerId(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t manufacturerId;

    // Set the pointer register of the TMP006 IC to the manufacturer ID register (0xfe).
    ui8Data[0] = 0xfe;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return ~0;

    // Read the manufacturer ID register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return ~0;

    // Calculate the manufacturer ID.
    manufacturerId = (uint32_t) (ui8Data[0] << 8) | (ui8Data[1]);

    return manufacturerId;
}



// Read the device ID from the TMP006 sensor IC.
uint32_t I2CTmp006ReadDeviceId(tI2C *psI2C, uint8_t ui8SlaveAddr)
{
    uint8_t ui8Data[2];
    uint32_t ui32I2CMasterStatus;
    uint32_t deviceId;

    // Set the pointer register of the TMP006 IC to the device ID register (0xff).
    ui8Data[0] = 0xff;
    ui32I2CMasterStatus = I2CMasterWrite(psI2C, ui8SlaveAddr, ui8Data, 1);
    if (ui32I2CMasterStatus) return ~0;

    // Read the device ID register.
    ui32I2CMasterStatus = I2CMasterRead(psI2C, ui8SlaveAddr, ui8Data, 2);
    if (ui32I2CMasterStatus) return ~0;

    // Calculate the device ID.
    deviceId = (uint32_t) (ui8Data[0] << 8) | (ui8Data[1]);

    return deviceId;
}

