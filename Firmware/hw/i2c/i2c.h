// File: i2c.h
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 11 Feb 2020
// Rev.: 18 Feb 2020
//
// Header file for the I2C functions on the TI Tiva TM4C1294 connected
// LaunchPad evaluation kit.
//



#ifndef __I2C_H__
#define __I2C_H__



// Types.
typedef struct {
    uint32_t ui32PeripheralI2C;
    uint32_t ui32PeripheralGpio;
    uint32_t ui32PortGpio;
    uint8_t  ui8PinGpioScl;
    uint8_t  ui8PinGpioSda;
    uint32_t ui32PinConfigScl;
    uint32_t ui32PinConfigSda;
    uint32_t ui32BaseI2C;
    uint32_t ui32SysClock;
    bool     bFast;                 // false = 100 kbps; true = 400 kbps
    uint32_t ui32IntFlags;
    uint32_t ui32Timeout;
} tI2C;


// Function prototypes.
void I2C2MasterInit(tI2C *i2c);
uint32_t I2CMasterWrite(tI2C *i2c, uint8_t ui8SlaveAddr, uint8_t *ui8Data, uint8_t ui8Length);
uint32_t I2CMasterRead(tI2C *i2c, uint8_t ui8SlaveAddr, uint8_t *ui8Data, uint8_t ui8Length);



#endif  // __I2C_H__

