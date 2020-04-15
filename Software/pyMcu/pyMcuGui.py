#!/usr/bin/env python3
#
# File: pyMcuGui.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Mar 2020
# Rev.: 15 Apr 2020
#
# Python GUI for accessing the TM4C1294NCPDT MCU on the TM4C1294 Connected
# LaunchPad Evaluation Kit over a serial port (UART).
#



# Append hardware classes folder to python path.
import sys
import os
sys.path.append(os.path.dirname(__file__) + '/hw')

# System modules.
import time

# GUI: tkinter
from tkinter import *

# Hardware classes.
import Adc
import GpioLed
import I2CTmp006
import I2COpt3001
import McuI2C
import McuSerial
import McuUart
import RgbLed



# GUI class.
class PyMcuGui(Frame):

    # Message prefixes and separators.
    prefixError = "ERROR: {0:s}: ".format(__file__)
    prefixDebug = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.

    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master = master
        self.init_window()

    def init_window(self):
        self.master.title("pyMcu GUI")
        self.grid(padx=5, pady=5)
        # ---
        self.frameFwInfo = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameFwInfo.grid(row=0, column=0, sticky=W+E, pady=2)
        self.labelFwInfo = Label(self.frameFwInfo, text="", anchor=W, justify=LEFT)
        self.labelFwInfo.grid(row=0, column=0, sticky=W+E)
        # ---
        self.frameLed = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameLed.grid(row=1, column=0, sticky=W+E, pady=2)
        self.labelGpioLed = Label(self.frameLed, text="GPIO LEDs", anchor=W, width=10)
        self.labelGpioLed.grid(row=0, column=0, sticky=W+E)
        self.entryGpioLed = Entry(self.frameLed, width=5)
        self.entryGpioLed.grid(row=0, column=1)
        self.entryGpioLed.insert(0, "0x0")
        self.buttonGpioLedSet = Button(self.frameLed, text="Set LEDs", command=self.gpio_led_set)
        self.buttonGpioLedSet.grid(row=0, column=4, sticky=W+E, padx=10)
        # ---
        self.labelRgbLed = Label(self.frameLed, text="RGB LED", anchor=W)
        self.labelRgbLed.grid(row=1, column=0, sticky=W+E)
        self.entryRgbLedR = Entry(self.frameLed, width=5)
        self.entryRgbLedR.grid(row=1, column=1)
        self.entryRgbLedR.insert(0, "0x00")
        self.entryRgbLedG = Entry(self.frameLed, width=5)
        self.entryRgbLedG.grid(row=1, column=2)
        self.entryRgbLedG.insert(0, "0x00")
        self.entryRgbLedB = Entry(self.frameLed, width=5)
        self.entryRgbLedB.grid(row=1, column=3)
        self.entryRgbLedB.insert(0, "0x00")
        self.buttonRgbLedSet = Button(self.frameLed, text="Set RGB LED", command=self.rgb_led_set)
        self.buttonRgbLedSet.grid(row=1, column=4, sticky=W+E, padx=10)
        # ---
        self.frameAnalog = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameAnalog.grid(row=2, column=0, sticky=W+E, pady=2)
        self.labelAnalogX = Label(self.frameAnalog, text="X")
        self.labelAnalogX.grid(row=0, column=1, sticky=W+E)
        self.labelAnalogY = Label(self.frameAnalog, text="Y")
        self.labelAnalogY.grid(row=0, column=2, sticky=W+E)
        self.labelAnalogZ = Label(self.frameAnalog, text="Z")
        self.labelAnalogZ.grid(row=0, column=3, sticky=W+E)
        self.labelJoystick = Label(self.frameAnalog, text="Joystick", anchor=W, width=15)
        self.labelJoystick.grid(row=1, column=0, sticky=W+E)
        self.entryJoystickX = Entry(self.frameAnalog, width=5, justify=RIGHT, state="readonly")
        self.entryJoystickX.grid(row=1, column=1, sticky=W)
        self.entryJoystickY = Entry(self.frameAnalog, width=5, justify=RIGHT, state="readonly")
        self.entryJoystickY.grid(row=1, column=2, sticky=W)
        self.labelAccel = Label(self.frameAnalog, text="Accelerometer", anchor=W)
        self.labelAccel.grid(row=2, column=0, sticky=W+E)
        self.entryAccelX = Entry(self.frameAnalog, width=5, justify=RIGHT, state="readonly")
        self.entryAccelX.grid(row=2, column=1, sticky=W)
        self.entryAccelY = Entry(self.frameAnalog, width=5, justify=RIGHT, state="readonly")
        self.entryAccelY.grid(row=2, column=2, sticky=W)
        self.entryAccelZ = Entry(self.frameAnalog, width=5, justify=RIGHT, state="readonly")
        self.entryAccelZ.grid(row=2, column=3, sticky=W)
        self.buttonAnalogUpdate = Button(self.frameAnalog, text="Update", command=self.analog_update, repeatdelay=200, repeatinterval=1)
        self.buttonAnalogUpdate.grid(row=1, column=4, sticky=W+E, padx=10)
        self.analogAutoUpdate = IntVar()
        self.checkbuttonAnalogUpdate = Checkbutton(self.frameAnalog, text="Auto Update", variable=self.analogAutoUpdate)
        self.checkbuttonAnalogUpdate.grid(row=2, column=4, sticky=W+E, padx=5)
        # ---
        self.frameSensor = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameSensor.grid(row=3, column=0, sticky=W+E, pady=2)
        self.labelSensorManufacturerId = Label(self.frameSensor, text="Manuf. ID")
        self.labelSensorManufacturerId.grid(row=0, column=1, sticky=W+E)
        self.labelSensorDeviceId = Label(self.frameSensor, text="Device ID")
        self.labelSensorDeviceId.grid(row=0, column=2, sticky=W+E)
        self.labelSensorValue = Label(self.frameSensor, text="Value")
        self.labelSensorValue.grid(row=0, column=3, sticky=W+E)
        self.labelSensorTmp006 = Label(self.frameSensor, text="Temperature (TMP006)", anchor=W, width=20)
        self.labelSensorTmp006.grid(row=1, column=0, sticky=W+E)
        self.entrySensorTmp006ManId = Entry(self.frameSensor, width=8, justify=RIGHT, state="readonly")
        self.entrySensorTmp006ManId.grid(row=1, column=1, sticky=W)
        self.entrySensorTmp006DevId = Entry(self.frameSensor, width=8, justify=RIGHT, state="readonly")
        self.entrySensorTmp006DevId.grid(row=1, column=2, sticky=W)
        self.entrySensorTmp006Value = Entry(self.frameSensor, width=14, justify=RIGHT, state="readonly")
        self.entrySensorTmp006Value.grid(row=1, column=3, sticky=W)
        self.labelSensorOpt3001 = Label(self.frameSensor, text="Light (OPT3001)", anchor=W, width=20)
        self.labelSensorOpt3001.grid(row=2, column=0, sticky=W+E)
        self.entrySensorOpt3001ManId = Entry(self.frameSensor, width=8, justify=RIGHT, state="readonly")
        self.entrySensorOpt3001ManId.grid(row=2, column=1, sticky=W)
        self.entrySensorOpt3001DevId = Entry(self.frameSensor, width=8, justify=RIGHT, state="readonly")
        self.entrySensorOpt3001DevId.grid(row=2, column=2, sticky=W)
        self.entrySensorOpt3001Value = Entry(self.frameSensor, width=14, justify=RIGHT, state="readonly")
        self.entrySensorOpt3001Value.grid(row=2, column=3, sticky=W)
        self.buttonSensorUpdate = Button(self.frameSensor, text="Update", command=self.sensor_update, repeatdelay=200, repeatinterval=1)
        self.buttonSensorUpdate.grid(row=1, column=4, sticky=W+E, padx=10)
        self.sensorAutoUpdate = IntVar()
        self.checkbuttonSensorUpdate = Checkbutton(self.frameSensor, text="Auto Update", variable=self.sensorAutoUpdate)
        self.checkbuttonSensorUpdate.grid(row=2, column=4, sticky=W+E, padx=5)
        # ---
        self.buttonQuit = Button(self, text="Quit", command=self.quit)
        self.buttonQuit.grid(row=4, column=0, columnspan=5, sticky=W+E, pady=2)

    # Initialize the hardware.
    def init_hw(self, dev):
        # Open the MCU serial interface.
        self.mcuSer = McuSerial.McuSerial(dev)
        self.mcuSer.debugLevel = 0
        self.mcuSer.simulateHwAccess = False
        self.mcuSer.clear()
        # Define the MCU peripherals.
        self.mcuI2C2 = McuI2C.McuI2C(self.mcuSer, 2)
        self.mcuI2C2.debugLevel = 1
        self.mcuUart6 = McuUart.McuUart(self.mcuSer, 6)
        self.mcuUart6.debugLevel = 1
        self.adc = Adc.Adc(self.mcuSer)
        self.adc.debugLevel = 1
        self.gpioLED = GpioLed.GpioLed(self.mcuSer)
        self.gpioLED.debugLevel = 1
        self.rgbLED = RgbLed.RgbLed(self.mcuSer)
        self.rgbLED.debugLevel = 1
        self.i2cTmp006 = I2CTmp006.I2CTmp006(self.mcuI2C2, 0x40)
        self.i2cTmp006.debugLevel = 1
        self.i2cOpt3001 = I2COpt3001.I2COpt3001(self.mcuI2C2, 0x44)
        self.i2cOpt3001.debugLevel = 1
        # Read and show the MCU firmware info.
        self.mcuSer.send("info")
        self.labelFwInfo['text'] = "Firmware info:\n" + self.mcuSer.get().replace('\r', '')

    # Set the GPIO LEDs.
    def gpio_led_set(self):
        try:
            led = int(self.entryGpioLed.get(), 0)
            self.gpioLED.set(led)
        except Exception as e:
            print(self.prefixError + "Error setting the GPIO LEDs: " + str(e))

    # Set the RGB LED.
    def rgb_led_set(self):
        try:
            led = int(self.entryGpioLed.get(), 0)
            self.rgbLED.set(
                int(self.entryRgbLedR.get(), 0),
                int(self.entryRgbLedG.get(), 0),
                int(self.entryRgbLedB.get(), 0))
        except Exception as e:
            print(self.prefixError + "Error setting the RGB LED: " + str(e))

    # Update the analog values.
    def analog_update(self):
        try:
            self.adc.read()
            self.entryJoystickX['state'] = NORMAL
            self.entryJoystickY['state'] = NORMAL
            self.entryAccelX['state'] = NORMAL
            self.entryAccelY['state'] = NORMAL
            self.entryAccelZ['state'] = NORMAL
            self.entryJoystickX.delete(0, END)
            self.entryJoystickY.delete(0, END)
            self.entryAccelX.delete(0, END)
            self.entryAccelY.delete(0, END)
            self.entryAccelZ.delete(0, END)
            self.entryJoystickX.insert(0, "{0:4d}".format(self.adc.joystickX))
            self.entryJoystickY.insert(0, "{0:4d}".format(self.adc.joystickY))
            self.entryAccelX.insert(0, "{0:4d}".format(self.adc.accelX))
            self.entryAccelY.insert(0, "{0:4d}".format(self.adc.accelY))
            self.entryAccelZ.insert(0, "{0:4d}".format(self.adc.accelZ))
            self.entryJoystickX['state'] = "readonly"
            self.entryJoystickY['state'] = "readonly"
            self.entryAccelX['state'] = "readonly"
            self.entryAccelY['state'] = "readonly"
            self.entryAccelZ['state'] = "readonly"
        except Exception as e:
            print(self.prefixError + "Error updating the analog values: " + str(e))

    # Update the analog values.
    def sensor_update(self):
        try:
            self.entrySensorTmp006ManId['state'] = NORMAL
            self.entrySensorTmp006DevId['state'] = NORMAL
            self.entrySensorTmp006Value['state'] = NORMAL
            self.entrySensorOpt3001ManId['state'] = NORMAL
            self.entrySensorOpt3001DevId['state'] = NORMAL
            self.entrySensorOpt3001Value['state'] = NORMAL
            self.entrySensorTmp006ManId.delete(0, END)
            self.entrySensorTmp006DevId.delete(0, END)
            self.entrySensorTmp006Value.delete(0, END)
            self.entrySensorOpt3001ManId.delete(0, END)
            self.entrySensorOpt3001DevId.delete(0, END)
            self.entrySensorOpt3001Value.delete(0, END)
            self.entrySensorTmp006ManId.insert(0, "0x{0:04x}".format(self.i2cTmp006.read_manufacturer_id()))
            self.entrySensorTmp006DevId.insert(0, "0x{0:04x}".format(self.i2cTmp006.read_device_id()))
            self.entrySensorTmp006Value.insert(0, "{0:9.5f} degC".format(self.i2cTmp006.read_temperature()))
            self.entrySensorOpt3001ManId.insert(0, "0x{0:04x}".format(self.i2cOpt3001.read_manufacturer_id()))
            self.entrySensorOpt3001DevId.insert(0, "0x{0:04x}".format(self.i2cOpt3001.read_device_id()))
            self.entrySensorOpt3001Value.insert(0, "{0:9.5f} lux".format(self.i2cOpt3001.read_illuminance()))
            self.entrySensorTmp006ManId['state'] = "readonly"
            self.entrySensorTmp006DevId['state'] = "readonly"
            self.entrySensorTmp006Value['state'] = "readonly"
            self.entrySensorOpt3001ManId['state'] = "readonly"
            self.entrySensorOpt3001DevId['state'] = "readonly"
            self.entrySensorOpt3001Value['state'] = "readonly"
        except Exception as e:
            print(self.prefixError + "Error updating the sensor values: " + str(e))

    # Quit.
    def quit(self):
        self.destroy()



# Launch the GUI.
def launch_gui(dev):
    root = Tk()
#    root.geometry("600x400")
    pyMcuGui = PyMcuGui(root)
    pyMcuGui.init_hw(dev)
    while pyMcuGui.winfo_exists():
        root.update_idletasks()
        root.update()
        if pyMcuGui.analogAutoUpdate.get():
            pyMcuGui.analog_update()
        if pyMcuGui.sensorAutoUpdate.get():
            pyMcuGui.sensor_update()



if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='Access to Process some integers.')
    parser.add_argument('-d', '--device', action='store', type=str,
                        dest='serial_device', default='/dev/ttyUSB0',
                        help='Serial device to access the MCU.')
    args = parser.parse_args()
    launch_gui(args.serial_device)

