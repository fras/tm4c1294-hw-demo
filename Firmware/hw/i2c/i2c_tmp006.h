// File: i2c_tmp006.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 12 Feb 2020
// Rev.: 19 Feb 2020
//
// Header file for the I2C functions for the TMP006 temperature sensor on the
// Educational BoosterPack MK II connected to the TI Tiva TM4C1294 Connected
// LaunchPad Evaluation Kit.
//



#ifndef __I2C_TMP006_H__
#define __I2C_TMP006_H__



#include "i2c.h"



// Function prototypes.
uint32_t I2CTmp006Reset(tI2C *i2c, uint8_t ui8SlaveAddr);
uint32_t I2CTmp006Init(tI2C *i2c, uint8_t ui8SlaveAddr);
float I2CTmp006ReadVolt(tI2C *i2c, uint8_t ui8SlaveAddr);
float I2CTmp006ReadTemp(tI2C *i2c, uint8_t ui8SlaveAddr);
uint32_t I2CTmp006ReadManufacturerId(tI2C *i2c, uint8_t ui8SlaveAddr);
uint32_t I2CTmp006ReadDeviceId(tI2C *i2c, uint8_t ui8SlaveAddr);



#endif  // __I2C_TMP006_H__

