// File: i2c.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 11 Feb 2020
// Rev.: 17 Apr 2020
//
// I2C functions on the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "i2c.h"



// Initialize an I2C master.
void I2CMasterInit(tI2C *psI2C)
{
    // Set up the IO pins for the I2C master.
    SysCtlPeripheralEnable(psI2C->ui32PeripheralGpio);
    GPIOPinConfigure(psI2C->ui32PinConfigScl);
    GPIOPinConfigure(psI2C->ui32PinConfigSda);
    GPIOPinTypeI2CSCL(psI2C->ui32PortGpioBase, psI2C->ui8PinGpioScl);   // SCL
    GPIOPinTypeI2C(psI2C->ui32PortGpioBase, psI2C->ui8PinGpioSda);      // SDA

    // Set up the I2C master.
    SysCtlPeripheralDisable(psI2C->ui32PeripheralI2C);
    SysCtlPeripheralReset(psI2C->ui32PeripheralI2C);
    SysCtlPeripheralEnable(psI2C->ui32PeripheralI2C);
    while(!SysCtlPeripheralReady(psI2C->ui32PeripheralI2C));
    I2CMasterInitExpClk(psI2C->ui32BaseI2C, psI2C->ui32I2CClk, psI2C->bFast);
    I2CMasterIntEnableEx(psI2C->ui32BaseI2C, psI2C->ui32IntFlags);
    // CAUTION: Do *not* set the timeout if repeated start is required!
//    I2CMasterTimeoutSet(psI2C->ui32BaseI2C, psI2C->ui32Timeout);
    // Set the timeout to 0 (no timeout).
    I2CMasterTimeoutSet(psI2C->ui32BaseI2C, 0);
    I2CMasterEnable(psI2C->ui32BaseI2C);
}



// Write data to an I2C master.
uint32_t I2CMasterWrite(tI2C *psI2C, uint8_t ui8SlaveAddr, uint8_t *pui8Data, uint8_t ui8Length)
{
    return I2CMasterWriteAdv(psI2C, ui8SlaveAddr, pui8Data, ui8Length, false, true);
}



// Write data to an I2C master (advanced).
uint32_t I2CMasterWriteAdv(tI2C *psI2C, uint8_t ui8SlaveAddr, uint8_t *pui8Data, uint8_t ui8Length, bool bRepeatedStart, bool bStop)
{
    uint32_t ui32I2CMasterInt, ui32I2CMasterErr;
    uint32_t ui32Timeout = psI2C->ui32Timeout + 10;     // Guarantee some minimum timeout value.

    if (ui8Length < 1) return 1;

    // Clear all I2C master interrupts.
    I2CMasterIntClearEx(psI2C->ui32BaseI2C, 0xffffffffU);

    // Set the I2C slave address.
    I2CMasterSlaveAddrSet(psI2C->ui32BaseI2C, ui8SlaveAddr, false);   // false = write; true = read

    // Wait until the I2C bus is free, if no repeated start.
    if (!bRepeatedStart) {
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!I2CMasterBusBusy(psI2C->ui32BaseI2C)) break;
            SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
                                                    // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
            // Timeout while waiting for the I2C bus to be free.
            if (i == ui32Timeout) return I2C_MASTER_INT_ARB_LOST;
        }
    }

    // Send data.
    for (int i = 0; i < ui8Length; i++) {
        I2CMasterDataPut(psI2C->ui32BaseI2C, pui8Data[i]);
        if (ui8Length == 1 && bStop) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_SINGLE_SEND);
        } else {
            if (i == 0) I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_START);
            else if ((i == ui8Length - 1) && bStop) I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_FINISH);
            else I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_CONT);
        }
        // Wait until the transfer is finished.
        SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!I2CMasterBusy(psI2C->ui32BaseI2C)) break;
            SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
            // Timeout while waiting for the I2C master to be ready.
            if (i == ui32Timeout) {
                I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
                return I2C_MASTER_INT_TIMEOUT;
            }
        }
        // Read I2C master interrupts.
        ui32I2CMasterInt = I2CMasterIntStatusEx(psI2C->ui32BaseI2C, false);
        ui32I2CMasterInt &= I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT;
        if (ui32I2CMasterInt) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            break;
        }
        // Check for I2C errors.
        ui32I2CMasterErr = I2CMasterErr(psI2C->ui32BaseI2C);
        if (ui32I2CMasterErr != I2C_MASTER_ERR_NONE) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            return 1;
        }
    }

    return ui32I2CMasterInt;
}



// Read data from an I2C master.
uint32_t I2CMasterRead(tI2C *psI2C, uint8_t ui8SlaveAddr, uint8_t *pui8Data, uint8_t ui8Length)
{
    return I2CMasterReadAdv(psI2C, ui8SlaveAddr, pui8Data, ui8Length, false, true);
}



// Read data from an I2C master (advanced).
uint32_t I2CMasterReadAdv(tI2C *psI2C, uint8_t ui8SlaveAddr, uint8_t *pui8Data, uint8_t ui8Length, bool bRepeatedStart, bool bStop)
{
    uint32_t ui32I2CMasterInt, ui32I2CMasterErr;
    uint32_t ui32Timeout = psI2C->ui32Timeout + 10;     // Guarantee some minimum timeout value.

    if (ui8Length < 1) return 1;

    // Clear all I2C master interrupts.
    I2CMasterIntClearEx(psI2C->ui32BaseI2C, 0xffffffffU);

    // Set the I2C slave address.
    I2CMasterSlaveAddrSet(psI2C->ui32BaseI2C, ui8SlaveAddr, true);    // false = write; true = read

    // Wait until the I2C bus is free, if no repeated start.
    if (!bRepeatedStart) {
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!I2CMasterBusBusy(psI2C->ui32BaseI2C)) break;
            SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
                                                    // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
            // Timeout while waiting for the I2C bus to be free.
            if (i == ui32Timeout) return I2C_MASTER_INT_ARB_LOST;
        }
    }

    // Receive data.
    for (int i = 0; i < ui8Length; i++) {
        if (ui8Length == 1 && bStop) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_SINGLE_RECEIVE);
        } else {
            if (i == 0) I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_START);
            else if ((i == ui8Length - 1) && bStop) I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
            else I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        }
        // Wait until the transfer is finished.
        SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!I2CMasterBusy(psI2C->ui32BaseI2C)) break;
            SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
            // Timeout while waiting for the I2C master to be ready.
            if (i == ui32Timeout) {
                I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
                return I2C_MASTER_INT_TIMEOUT;
            }
        }
        // Read I2C master interrupts.
        ui32I2CMasterInt = I2CMasterIntStatusEx(psI2C->ui32BaseI2C, false);
        ui32I2CMasterInt &= I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT;
        if (ui32I2CMasterInt) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
            break;
        }
        // Check for I2C errors.
        ui32I2CMasterErr = I2CMasterErr(psI2C->ui32BaseI2C);
        if (ui32I2CMasterErr != I2C_MASTER_ERR_NONE) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
            return 1;
        }
        // Get the data byte from the I2C master.
        pui8Data[i] = I2CMasterDataGet(psI2C->ui32BaseI2C);
    }

    return ui32I2CMasterInt;
}



// Send a quick command.
uint32_t I2CMasterQuickCmd(tI2C *psI2C, uint8_t ui8SlaveAddr, bool bReceive)
{
    return I2CMasterQuickCmdAdv(psI2C, ui8SlaveAddr, bReceive, false);
}



// Send a quick command (advanced).
uint32_t I2CMasterQuickCmdAdv(tI2C *psI2C, uint8_t ui8SlaveAddr, bool bReceive, bool bRepeatedStart)
{
    uint32_t ui32I2CMasterInt, ui32I2CMasterErr;
    uint32_t ui32Timeout = psI2C->ui32Timeout + 10;     // Guarantee some minimum timeout value.

    // Clear all I2C Master interrupts.
    I2CMasterIntClearEx(psI2C->ui32BaseI2C, 0xffffffffU);

    // Set the I2C slave address.
    I2CMasterSlaveAddrSet(psI2C->ui32BaseI2C, ui8SlaveAddr, bReceive);  // false = write; true = read

    // Wait until the I2C bus is free, if no repeated start.
    if (!bRepeatedStart) {
        for (int i = 0; i <= ui32Timeout; i++) {
            if (!I2CMasterBusBusy(psI2C->ui32BaseI2C)) break;
            SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
                                                    // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
            // Timeout while waiting for the I2C bus to be free.
            if (i == ui32Timeout) return I2C_MASTER_INT_ARB_LOST;
        }
    }

    // Send the quick command.
    I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_QUICK_COMMAND);
    // Wait until the transfer is finished.
    SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
    for (int i = 0; i <= ui32Timeout; i++) {
        if (!I2CMasterBusy(psI2C->ui32BaseI2C)) break;
        SysCtlDelay(psI2C->ui32I2CClk / 3e5);   // 10 us delay.
        // Timeout while waiting for the I2C master to be ready.
        if (i == ui32Timeout) {
            I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            return I2C_MASTER_INT_TIMEOUT;
        }
    }
    // Read I2C master interrupts.
    ui32I2CMasterInt = I2CMasterIntStatusEx(psI2C->ui32BaseI2C, false);
    ui32I2CMasterInt &= I2C_MASTER_INT_ARB_LOST | I2C_MASTER_INT_NACK | I2C_MASTER_INT_TIMEOUT;
    // Send stop condition.
    I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
    // Check for I2C errors.
    ui32I2CMasterErr = I2CMasterErr(psI2C->ui32BaseI2C);
    if (ui32I2CMasterErr != I2C_MASTER_ERR_NONE) {
        I2CMasterControl(psI2C->ui32BaseI2C, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
        return 1;
    }

    return ui32I2CMasterInt;
}

