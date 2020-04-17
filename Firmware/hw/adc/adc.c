// File: adc.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 13 Feb 2020
// Rev.: 11 Apr 2020
//
// ADC functions on the TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit.
//



#include <stdbool.h>
#include <stdint.h>
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "adc.h"



// Reset an ADC.
void AdcReset(tADC *psAdc)
{
    AdcInit(psAdc);
    SysCtlPeripheralReset(psAdc->ui32PeripheralAdc);
    while(!SysCtlPeripheralReady(psAdc->ui32PeripheralAdc));
}



// Initialize an ADC.
void AdcInit(tADC *psAdc)
{
    // Set up the IO pins for the I2C master.
    SysCtlPeripheralEnable(psAdc->ui32PeripheralGpio);
    GPIOPinTypeADC(psAdc->ui32PortGpioBase, psAdc->ui8PinGpio);

    // Set up the ADC.
    SysCtlPeripheralEnable(psAdc->ui32PeripheralAdc);
    // CAUTION:
    // Do *not* call "SysCtlPeripheralReset(psAdc->ui32PeripheralAdc);" here!
    // This will cause a freeze of the MCU, when several sequences are defined
    // for the same ADC and AdcConvert() is called for all sequences except the
    // last one. The reason is that the SysCtlPeripheralReset function call
    // when initialzing the last sequence will clear all configuration for the
    // previously initialized sequences.
    while(!SysCtlPeripheralReady(psAdc->ui32PeripheralAdc));
    ADCClockConfigSet(psAdc->ui32BaseAdc, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 30);
//    ADCClockConfigSet(psAdc->ui32BaseAdc, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);
    ADCReferenceSet(psAdc->ui32BaseAdc, ADC_REF_INT);
    ADCSequenceConfigure(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum, psAdc->ui32Step, psAdc->ui32Config);
    ADCSequenceEnable(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum);
    ADCIntClear(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum);
}



// Trigger an ADC conversion and return the result.
uint32_t AdcConvert(tADC *psAdc)
{
    uint32_t ui32ADCValue;

    // Trigger the ADC conversion.
    ADCProcessorTrigger(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum);

    // Wait for conversion to be completed.
    while(!ADCIntStatus(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum, false));

    // Clear the ADC interrupt flag.
    ADCIntClear(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum);

    // Read the ADC value.
    ADCSequenceDataGet(psAdc->ui32BaseAdc, psAdc->ui32SequenceNum, &ui32ADCValue);

    return ui32ADCValue;
}

