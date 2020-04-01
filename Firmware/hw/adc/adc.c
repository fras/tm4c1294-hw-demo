// File: adc.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 13 Feb 2020
// Rev.: 30 Mar 2020
//
// ADC functions on the TI Tiva TM4C1294 connected LaunchPad evaluation kit.
//



#include <stdint.h>
#include <stdbool.h>
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
#include "utils/uartstdio.h"
#include "adc.h"



// Reset an ADC.
void AdcReset(tADC *adc)
{
    AdcInit(adc);
    SysCtlPeripheralReset(adc->ui32PeripheralAdc);
    while(!SysCtlPeripheralReady(adc->ui32PeripheralAdc));
}



// Initialize an ADC.
void AdcInit(tADC *adc)
{
    // Setup the IO pins for the I2C master.
    SysCtlPeripheralEnable(adc->ui32PeripheralGpio);
    GPIOPinTypeADC(adc->ui32PortGpio, adc->ui8PinGpio);

    // Setup the ADC.
    SysCtlPeripheralEnable(adc->ui32PeripheralAdc);
    // CAUTION:
    // Do *not* call "SysCtlPeripheralReset(adc->ui32PeripheralAdc);" here!
    // This will cause a freeze of the MCU, when several sequences are defined
    // for the same ADC and AdcConvert() is called for all sequences except the
    // last one. The reason is that the SysCtlPeripheralReset function call
    // when initialzing the last sequence will clear all configuration for the
    // previously initialized sequences.
    while(!SysCtlPeripheralReady(adc->ui32PeripheralAdc));
    ADCClockConfigSet(adc->ui32BaseAdc, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 30);
//    ADCClockConfigSet(adc->ui32BaseAdc, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);
    ADCReferenceSet(adc->ui32BaseAdc, ADC_REF_INT);
    ADCSequenceConfigure(adc->ui32BaseAdc, adc->ui32SequenceNum, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(adc->ui32BaseAdc, adc->ui32SequenceNum, adc->ui32Step, adc->ui32Config);
    ADCSequenceEnable(adc->ui32BaseAdc, adc->ui32SequenceNum);
    ADCIntClear(adc->ui32BaseAdc, adc->ui32SequenceNum);
}



// Trigger an ADC conversion and return the result.
uint32_t AdcConvert(tADC *adc)
{
    uint32_t ui32ADCValue;

    // Trigger the ADC conversion.
    ADCProcessorTrigger(adc->ui32BaseAdc, adc->ui32SequenceNum);

    // Wait for conversion to be completed.
    while(!ADCIntStatus(adc->ui32BaseAdc, adc->ui32SequenceNum, false));

    // Clear the ADC interrupt flag.
    ADCIntClear(adc->ui32BaseAdc, adc->ui32SequenceNum);

    // Read the ADC value.
    ADCSequenceDataGet(adc->ui32BaseAdc, adc->ui32SequenceNum, &ui32ADCValue);

    return ui32ADCValue;
}

