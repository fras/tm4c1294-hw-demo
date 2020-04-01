#!/usr/bin/env python3
#
# File: pyMcuTest.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 24 Mar 2020
# Rev.: 31 Mar 2020
#
# Python script to test hardware features of the TM4C1294NCPDT MCU on the
# TM4C1294 Connected LaunchPad Evaluation Kit over a serial port (UART).
#



# Append hardware classes folder to python path.
import sys
sys.path.append('hw')



# System modules.
import time



# Hardware classes.
import Adc
import GpioLed
import I2CTmp006
import I2COpt3001
import McuI2C
import McuSerial
import McuUart
import RgbLed



# Select tests to run.
separatorTests        = "-----"
# Preset all to fales.
testFwInfo          = False
testMcuSerial_0     = False
testGpioLed         = False
testGpioLedRaw      = False
testRgbLed          = False
testI2C             = False
testUart            = False
testAdc             = False
testTmp006          = False
testOpt3001         = False
testMcuSerial_1     = False
# Set the tests to true that you want to run.
testFwInfo          = True
testMcuSerial_0     = True
testGpioLed         = True
testGpioLedRaw      = True
testRgbLed          = True
testI2C             = True
testUart            = True
testAdc             = True
testTmp006          = True
testOpt3001         = True
testMcuSerial_1     = True



# Print MCU serial port info.
def mcuSerial_print_info(mcuSer):
    mcuSerPrefixDetailsBak = mcuSer.prefixDetails
    mcuSerSeparatorDetailsBak = mcuSer.separatorDetails
    mcuSerdebugLevelBak = mcuSer.debugLevel
    mcuSer.prefixDetails = ""
    mcuSer.separatorDetails = "\n"
    mcuSer.debugLevel = 1
    mcuSer.print_details()
    mcuSer.prefixDetails = mcuSerPrefixDetailsBak
    mcuSer.separatorDetails = mcuSerSeparatorDetailsBak
    mcuSer.debugLevel = mcuSerdebugLevelBak



# Run the hardware test.
def run_test(dev):
    # Open the MCU serial interface.
    mcuSer = McuSerial.McuSerial(dev)
    mcuSer.debugLevel = 0
    mcuSer.simulateHwAccess = False
    mcuSer.clear()
    
    
    
    # Define the MCU peripherals.
    mcuI2C2 = McuI2C.McuI2C(mcuSer, 2)
    mcuI2C2.debugLevel = 1
    mcuUart6 = McuUart.McuUart(mcuSer, 6)
    mcuUart6.debugLevel = 1
    adc = Adc.Adc(mcuSer)
    adc.debugLevel = 1
    gpioLED = GpioLed.GpioLed(mcuSer)
    gpioLED.debugLevel = 1
    rgbLED = RgbLed.RgbLed(mcuSer)
    rgbLED.debugLevel = 1
    i2cTmp006 = I2CTmp006.I2CTmp006(mcuI2C2, 0x40)
    i2cTmp006.debugLevel = 1
    i2cOpt3001 = I2COpt3001.I2COpt3001(mcuI2C2, 0x44)
    i2cOpt3001.debugLevel = 1



    # Read the MCU info.
    if testFwInfo:
        print("MCU firmware info:")
        mcuSer.send("info")
        print(mcuSer.get())
        print(separatorTests)



    # Read the MCU serial port info.
    if testMcuSerial_0:
        print("MCU serial port {0:s} info:".format(mcuSer.ser.port))
        mcuSerial_print_info(mcuSer)
        print(separatorTests)



    # Blink the LEDs.
    if testGpioLed:
        print("LED running light.")
        led = 0x1
        for i in range(0, 5):
            gpioLED.set(led)
            time.sleep(0.1)
            led <<= 1
        print(separatorTests)
    if testGpioLedRaw:
        print("LED running light (raw).")
        led = 0x1
        for i in range(0, 5):
            mcuSer.send("led 0x%x" % led)
        #    print(mcuSer.get())
            print(mcuSer.get_full())
            if mcuSer.eval(): print("Error!")
            led <<= 1
            time.sleep(0.1)
        print(separatorTests)



    # Set the RGB LED.
    if testRgbLed:
        print("RGB LED.")
        rgbLED.set(0xff, 0x00, 0x00)
        time.sleep(0.5)
        rgbLED.set(0x00, 0xff, 0x00)
        time.sleep(0.5)
        rgbLED.set(0x00, 0x00, 0xff)
        time.sleep(0.5)
        rgbLED.set(0xff, 0xff, 0xff)
        time.sleep(0.5)
        rgbLED.set(0x00, 0x00, 0x00)
        print(separatorTests)



    # I2C test.
    if testI2C:
        print("I2C master port {0:d} test.".format(mcuI2C2.port))
    #    for i in range(0, 200):
    #        mcuI2C2.ms_write(0x40, [0x00, 0x34])
    #        mcuI2C2.ms_read(0x40, 2)
        mcuI2C2.ms_write(0x40, [0x01, 0x12, 0x34])
        data = mcuI2C2.ms_read(0x40, 2)
        print("I2C data read:", end = '')
        for i in range(0, len(data)):
            print(" 0x{0:02x}".format(data[i]), end = '')
        print()
        print(separatorTests)



    # UART test.
    if testUart:
        print("UART port {0:d} test.".format(mcuUart6.port))
    #    for i in range(0, 100):
    #        mcuUart6.write([i])
    #        mcuUart6.read(1)
        # Binary write/read.
        mcuUart6.clear()
        mcuUart6.write([0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07])
        data = mcuUart6.read_all()
        print("Data read from UART:", end = '')
        for i in range(0, len(data)):
            print(" 0x{0:02x}".format(data[i]), end = '')
        print()
        # String write/read.
        mcuUart6.clear()
        mcuUart6.write_str("Hello world!")
        s = mcuUart6.read_str(0)    # 0 = Read all data.
        print("String read from UART: {0:s}".format(s))
        print(separatorTests)



    # ADC test.
    if testAdc:
        print("ADC test.")
    #    for i in range(0, 200):
        for i in range(0, 10):
            adc.read()
            print("ADC values:", end = '')
            print(" Joystick:", end = '')
            print(" X = {0:4d}".format(adc.joystickX), end = '')
            print(" Y = {0:4d}".format(adc.joystickY), end = '')
            print(" ; Accelerometer:", end = '')
            print(" X = {0:4d}".format(adc.accelX), end = '')
            print(" Y = {0:4d}".format(adc.accelY), end = '')
            print(" Z = {0:4d}".format(adc.accelZ), end = '')
            print()
        print(separatorTests)



    # Read info from the TPM006 temperature sensor.
    if testTmp006:
        print("Read info from the TPM006 temperature sensor.")
    #    i2cTmp006.reset()
    #    time.sleep(1.0)
        i2cTmp006.init()
        print("Manufacturer ID : 0x{0:04x}".format(i2cTmp006.read_manufacturer_id()))
        print("Device ID       : 0x{0:04x}".format(i2cTmp006.read_device_id()))
        for i in range (0, 1):
            print("Sensor voltage  : {0:9.5f} uV".format(i2cTmp006.read_voltage() / 1e3))
            print("Temperature     : {0:9.5f} degC".format(i2cTmp006.read_temperature()))
        print(separatorTests)



    # Read info from the OPT3001 ambient light sensor.
    if testOpt3001:
        print("Read info from the OPT3001 ambient light sensor.")
    #    i2cOpt3001.reset()
    #    time.sleep(1.0)
        i2cOpt3001.init()
        print("Manufacturer ID : 0x{0:04x}".format(i2cOpt3001.read_manufacturer_id()))
        print("Device ID       : 0x{0:04x}".format(i2cOpt3001.read_device_id()))
        for i in range (0, 1):
            print("Illuminance     : {0:9.5f} lux".format(i2cOpt3001.read_illuminance()))
        print(separatorTests)



    # Read the MCU serial port info.
    if testMcuSerial_1:
        print("MCU serial port {0:s} info:".format(mcuSer.ser.port))
        mcuSerial_print_info(mcuSer)
        print(separatorTests)



# Execute the test.
if __name__ == "__main__":
    run_test('/dev/ttyUSB0')
    print("\nBye-bye!")

