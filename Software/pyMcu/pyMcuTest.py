#!/usr/bin/env python3
#
# File: pyMcuTest.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 24 Mar 2020
# Rev.: 30 May 2020
#
# Python script to test hardware features of the TM4C1294NCPDT MCU on the
# TM4C1294 Connected LaunchPad Evaluation Kit over a serial port (UART).
#



# Append hardware classes folder to Python path.
import os
import sys
sys.path.append(os.path.relpath(os.path.join(os.path.dirname(__file__), 'hw')))



# System modules.
import time



# Hardware classes.
import Adc
import GpioButton
import GpioLed
import I2CTmp006
import I2COpt3001
import McuI2C
import McuSsi
import McuSerial
import McuUart
import RgbLed



# Message prefixes and separators.
separatorTests      = "-----"
prefixDebug         = "DEBUG: {0:s}: ".format(__file__)
prefixError         = "ERROR: {0:s}: ".format(__file__)

# Select tests to run.
# Preset all to false.
testFwInfo          = False
testMcuSerial_0     = False
testGpioButton      = False
testGpioButtonRaw   = False
testGpioLed         = False
testGpioLedRaw      = False
testRgbLed          = False
testI2C             = False
testSsi             = False
testUart            = False
testAdc             = False
testTmp006          = False
testOpt3001         = False
testLcd             = False
testMcuSerial_1     = False
# Set the tests to true that you want to run.
testFwInfo          = True
testMcuSerial_0     = True
testGpioButton      = True
testGpioButtonRaw   = True
testGpioLed         = True
testGpioLedRaw      = True
testRgbLed          = True
testI2C             = True
testSsi             = True
testUart            = True
testAdc             = True
testTmp006          = True
testOpt3001         = True
testLcd             = True
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
def run_test(serialDevice, verbosity):
    # Open the MCU serial interface.
    mcuSer = McuSerial.McuSerial(serialDevice)
    mcuSer.debugLevel = verbosity
    mcuSer.clear()



    # Define the MCU peripherals.
    mcuI2C2 = McuI2C.McuI2C(mcuSer, 2)
    mcuI2C2.debugLevel = verbosity
    mcuSsi2 = McuSsi.McuSsi(mcuSer, 2)
    mcuSsi2.debugLevel = verbosity
    mcuUart6 = McuUart.McuUart(mcuSer, 6)
    mcuUart6.debugLevel = verbosity
    adc = Adc.Adc(mcuSer)
    adc.debugLevel = verbosity
    gpioButton = GpioButton.GpioButton(mcuSer)
    gpioButton.debugLevel = verbosity
    gpioLED = GpioLed.GpioLed(mcuSer)
    gpioLED.debugLevel = verbosity
    rgbLED = RgbLed.RgbLed(mcuSer)
    rgbLED.debugLevel = verbosity
    i2cTmp006 = I2CTmp006.I2CTmp006(mcuI2C2, 0x40)
    i2cTmp006.debugLevel = verbosity
    i2cOpt3001 = I2COpt3001.I2COpt3001(mcuI2C2, 0x44)
    i2cOpt3001.debugLevel = verbosity



    # Read the MCU info.
    if testFwInfo:
        print("MCU firmware info:")
        mcuSer.send("info")
        print(mcuSer.get())
        print(separatorTests)



    # Read the MCU serial port info.
    if testMcuSerial_0:
        print("MCU serial port '{0:s}' info:".format(mcuSer.ser.port))
        mcuSerial_print_info(mcuSer)
        print(separatorTests)



    # Get the status of the buttons.
    if testGpioButton:
        print("Get GPIO button status.")
        for i in range(0, 4):
            ret, buttonStatus = gpioButton.get(i)
            if ret:
                print("Error getting the GPIO button {0:d} status.".format(i))
            else:
                print("Button {0:d}: Status = 0x{1:01x}, pressed count = {2:d}, released count = {3:d}".format(
                    i, buttonStatus[0], buttonStatus[1], buttonStatus[2]))
        print(separatorTests)



    # Get the status of the buttons (raw).
    if testGpioButtonRaw:
        print("Get GPIO button status (raw).")
        mcuSer.send("button")
        print(mcuSer.get())
        for i in range(0, 4):
            mcuSer.send("button {0:d}".format(i))
            print(mcuSer.get())
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



    # Raw access of the LEDs.
    if testGpioLedRaw:
        print("LED running light (raw).")
        led = 0x1
        for i in range(0, 5):
            mcuSer.send("led 0x%x" % led)
#            print(mcuSer.get())
            print(mcuSer.get_full())
            if mcuSer.eval():
                print("Error!")
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
#        for i in range(0, 200):
#            mcuI2C2.ms_write(0x40, [0x00, 0x34])
#            mcuI2C2.ms_read(0x40, 2)
        # Standard access.
        mcuI2C2.ms_write(0x40, [0x01, 0x12, 0x34])
        ret, data = mcuI2C2.ms_read(0x40, 2)
        print("I2C data read:", end='')
        for datum in data:
            print(" 0x{0:02x}".format(datum), end='')
        print()
        # Advanced access.
        mcuI2C2.ms_write_adv(0x40, [0x01], False, False)        # No repeated start, no stop condition.
        ret, data = mcuI2C2.ms_read_adv(0x40, 2, True, True)     # Repeated start, stop condition.
        print("I2C data read:", end='')
        for datum in data:
            print(" 0x{0:02x}".format(datum), end='')
        print()
        print(separatorTests)



    # SSI test.
    if testSsi:
        print("SSI port {0:d} test.".format(mcuSsi2.port))
        mcuSsi2.setup(1000000, 0, 8)    # 1 MHz SSI clock, SPI CPOL=0 CPHA=0, data width =8
#        for i in range(0, 200):
#            mcuSsi2.write([0x12, 0x34, 0x56, 0x78])
#            mcuSsi2.read(4)
        # Binary write/read.
        mcuSsi2.clear()
        mcuSsi2.write([0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07])
        ret, data = mcuSsi2.read_all()
        print("Data read from SSI:", end='')
        for datum in data:
            print(" 0x{0:02x}".format(datum), end='')
        print()
        print(separatorTests)



    # UART test.
    if testUart:
        print("UART port {0:d} test.".format(mcuUart6.port))
        # Set up the UART.
        mcuUart6.setup(115200, 0, 1)    # 115200 baud, no parity, internal loopback enabled.
#        for i in range(0, 100):
#            mcuUart6.write([i])
#            mcuUart6.read(1)
        # Binary write/read.
        mcuUart6.clear()
        mcuUart6.write([0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07])
        ret, data = mcuUart6.read_all()
        print("Data read from UART:", end='')
        for datum in data:
            print(" 0x{0:02x}".format(datum), end='')
        print()
        # String write/read.
        mcuUart6.clear()
        mcuUart6.write_str("Hello world!")
        ret, s = mcuUart6.read_str(0)   # 0 = Read all data.
        print("String read from UART: {0:s}".format(s))
        print(separatorTests)



    # ADC test.
    if testAdc:
        print("ADC test.")
#        for i in range(0, 200):
        for i in range(0, 10):
            adc.read()
            print("ADC values:", end='')
            print(" Joystick:", end='')
            print(" X = {0:4d}".format(adc.joystickX), end='')
            print(" Y = {0:4d}".format(adc.joystickY), end='')
            print(" ; Accelerometer:", end='')
            print(" X = {0:4d}".format(adc.accelX), end='')
            print(" Y = {0:4d}".format(adc.accelY), end='')
            print(" Z = {0:4d}".format(adc.accelZ), end='')
            print()
        print(separatorTests)



    # Read info from the TPM006 temperature sensor.
    if testTmp006:
        print("Read info from the TPM006 temperature sensor.")
#        i2cTmp006.reset()
#        time.sleep(1.0)
        i2cTmp006.init()
        print("Manufacturer ID : 0x{0:04x}".format(i2cTmp006.read_manufacturer_id()[1]))
        print("Device ID       : 0x{0:04x}".format(i2cTmp006.read_device_id()[1]))
        for i in range(0, 1):
            print("Sensor voltage  : {0:9.5f} uV".format(i2cTmp006.read_voltage()[1] / 1e3))
            print("Temperature     : {0:9.5f} degC".format(i2cTmp006.read_temperature()[1]))
        print(separatorTests)



    # Read info from the OPT3001 ambient light sensor.
    if testOpt3001:
        print("Read info from the OPT3001 ambient light sensor.")
#        i2cOpt3001.reset()
#        time.sleep(1.0)
        i2cOpt3001.init()
        print("Manufacturer ID : 0x{0:04x}".format(i2cOpt3001.read_manufacturer_id()[1]))
        print("Device ID       : 0x{0:04x}".format(i2cOpt3001.read_device_id()[1]))
        for i in range(0, 1):
            print("Illuminance     : {0:9.5f} lux".format(i2cOpt3001.read_illuminance()[1]))
        print(separatorTests)



    # Test the LCD.
    if testLcd:
        print("LCD test.")
        mcuSer.send("lcd clear 0x000000")
        time.sleep(0.5)
        mcuSer.send("lcd clear 0xff0000")
        time.sleep(0.5)
        mcuSer.send("lcd clear 0x00ff00")
        time.sleep(0.5)
        mcuSer.send("lcd clear 0x0000ff")
        time.sleep(0.5)
        mcuSer.send("lcd clear 0xffffff")
        time.sleep(0.5)
        mcuSer.send("lcd info")
        time.sleep(0.5)
        mcuSer.send("lcd orient 1")
        time.sleep(0.5)
        mcuSer.send("lcd orient 2")
        time.sleep(0.5)
        mcuSer.send("lcd orient 3")
        time.sleep(0.5)
        mcuSer.send("lcd orient 0")
        print(separatorTests)

    # Read the MCU serial port info.
    if testMcuSerial_1:
        print("MCU serial port '{0:s}' info:".format(mcuSer.ser.port))
        mcuSerial_print_info(mcuSer)
        print(separatorTests)



# Execute the test.
if __name__ == "__main__":
    # Command line arguments.
    import argparse
    parser = argparse.ArgumentParser(description='Run an automated set of MCU tests.')
    parser.add_argument('-d', '--device', action='store', type=str,
                        dest='serialDevice', default='/dev/ttyUSB0',
                        help='Serial device to access the MCU.')
    parser.add_argument('-v', '--verbosity', action='store', type=int,
                        dest='verbosity', default="1", choices=range(0, 5),
                        help='Set the verbosity level. The default is 1.')
    args = parser.parse_args()

    # Run the hardware test.
    run_test(args.serialDevice, args.verbosity)

    print("\nBye-bye!")

