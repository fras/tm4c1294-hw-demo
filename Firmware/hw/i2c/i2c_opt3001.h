// File: i2c_opt3001.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 13 Feb 2020
// Rev.: 19 Feb 2020
//
// Header file for the I2C functions for the OPT3001 ambient light sensor on
// the Educational BoosterPack MK II connected to the TI Tiva TM4C1294
// connected LaunchPad evaluation kit.
//



#ifndef __I2C_OPT3001_H__
#define __I2C_OPT3001_H__



// Function prototypes.
uint32_t I2COpt3001Reset(tI2C *i2c, uint8_t ui8SlaveAddr);
uint32_t I2COpt3001Init(tI2C *i2c, uint8_t ui8SlaveAddr);
float I2COpt3001ReadIlluminance(tI2C *i2c, uint8_t ui8SlaveAddr);
uint32_t I2COpt3001ReadManufacturerId(tI2C *i2c, uint8_t ui8SlaveAddr);
uint32_t I2COpt3001ReadDeviceId(tI2C *i2c, uint8_t ui8SlaveAddr);



#endif  // __I2C_OPT3001_H__

