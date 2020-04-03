// File: i2c.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 11 Feb 2020
// Rev.: 03 Apr 2020
//
// I2C functions on the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
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



// Initialize an I2C master.
void I2C2MasterInit(tI2C *i2c)
{
    // Setup the IO pins for the I2C master.
    SysCtlPeripheralEnable(i2c->ui32PeripheralGpio);
    GPIOPinConfigure(i2c->ui32PinConfigScl);
    GPIOPinConfigure(i2c->ui32PinConfigSda);
    GPIOPinTypeI2CSCL(i2c->ui32PortGpio, i2c->ui8PinGpioScl);   // SCL
    GPIOPinTypeI2C(i2c->ui32PortGpio, i2c->ui8PinGpioSda);      // SDA

    // Setup the I2C master.
    SysCtlPeripheralDisable(i2c->ui32PeripheralI2C);
    SysCtlPeripheralReset(i2c->ui32PeripheralI2C);
    SysCtlPeripheralEnable(i2c->ui32PeripheralI2C);
    while(!SysCtlPeripheralReady(i2c->ui32PeripheralI2C));
    I2CMasterInitExpClk(i2c->ui32BaseI2C, i2c->ui32SysClock, i2c->bFast);
    I2CMasterIntEnableEx(i2c->ui32BaseI2C, i2c->ui32IntFlags);
    I2CMasterTimeoutSet(i2c->ui32BaseI2C , i2c->ui32Timeout);
    I2CMasterEnable(i2c->ui32BaseI2C);
}



// Write data to an I2C master.
uint32_t I2CMasterWrite(tI2C *i2c, uint8_t ui8SlaveAddr, uint8_t *ui8Data, uint8_t ui8Length)
{
    uint32_t i2cMasterInt, i2cMasterErr;

    if (ui8Length < 1) return 1;

    // Clear all I2C Master interrupts.
    I2CMasterIntClearEx(i2c->ui32BaseI2C, 0xffffffffU);

    // Set the I2C slave address.
    I2CMasterSlaveAddrSet(i2c->ui32BaseI2C, ui8SlaveAddr, false);   // false = write; true = read

    // Wait until the I2C bus is free.
    while (I2CMasterBusBusy(i2c->ui32BaseI2C));

    // Send data.
    for (int i = 0; i < ui8Length; i++) {
        I2CMasterDataPut(i2c->ui32BaseI2C, ui8Data[i]);
        if (ui8Length == 1) {
            I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_SINGLE_SEND);
        } else {
            if (i == 0) I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_START);
            else if (i == ui8Length - 1) I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_FINISH);
            else I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_CONT);
        }
        // Wait until transfer is finished.
        SysCtlDelay(1000);
        while(I2CMasterBusy(i2c->ui32BaseI2C));
        // Read I2C master interrupts.
        i2cMasterInt = I2CMasterIntStatusEx(i2c->ui32BaseI2C, false);
        i2cMasterInt &= I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT;
        if (i2cMasterInt) {
            I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            break;
        }
        // Check for I2C errors.
        i2cMasterErr = I2CMasterErr(i2c->ui32BaseI2C);
        if (i2cMasterErr != I2C_MASTER_ERR_NONE) {
            I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            return 1;
        }
    }

    return i2cMasterInt;
}



// Read data from an I2C master.
uint32_t I2CMasterRead(tI2C *i2c, uint8_t ui8SlaveAddr, uint8_t *ui8Data, uint8_t ui8Length)
{
    uint32_t i2cMasterInt, i2cMasterErr;

    if (ui8Length < 1) return 1;

    // Clear all I2C Master interrupts.
    I2CMasterIntClearEx(i2c->ui32BaseI2C, 0xffffffffU);

    // Set the I2C slave address.
    I2CMasterSlaveAddrSet(i2c->ui32BaseI2C, ui8SlaveAddr, true);    // false = write; true = read

    // Wait until the I2C bus is free.
    while (I2CMasterBusBusy(i2c->ui32BaseI2C));

    // Receive data.
    for (int i = 0; i < ui8Length; i++) {
        if (ui8Length == 1) {
            I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_SINGLE_RECEIVE);
        } else {
            if (i == 0) I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_START);
            else if (i == ui8Length - 1) I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
            else I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        }
        // Wait until transfer is finished.
        SysCtlDelay(1000);
        while(I2CMasterBusy(i2c->ui32BaseI2C));
        // Read I2C master interrupts.
        i2cMasterInt = I2CMasterIntStatusEx(i2c->ui32BaseI2C, false);
        i2cMasterInt &= I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT;
        if (i2cMasterInt) {
            I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
            break;
        }
        // Check for I2C errors.
        i2cMasterErr = I2CMasterErr(i2c->ui32BaseI2C);
        if (i2cMasterErr != I2C_MASTER_ERR_NONE) {
            I2CMasterControl(i2c->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
            return 1;
        }
        // Get the data byte from the I2C master.
        ui8Data[i] = I2CMasterDataGet(i2c->ui32BaseI2C);
    }

    return i2cMasterInt;
}

