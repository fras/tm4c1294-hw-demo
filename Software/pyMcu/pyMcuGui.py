#!/usr/bin/env python3
#
# File: pyMcuGui.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Mar 2020
# Rev.: 14 Apr 2020
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
        self.pack(fill=BOTH, expand=1)
        # ---
        self.labelGpioLed = Label(self, text="GPIO LEDs")
        self.labelGpioLed.grid(row=0, column=0, sticky=W)
        self.entryGpioLed = Entry(self, width=5)
        self.entryGpioLed.grid(row=0, column=1)
        self.entryGpioLed.insert(0, "0x0")
        self.buttonGpioLedSet = Button(self, text="Set LEDs", command=self.gpio_led_set)
        self.buttonGpioLedSet.grid(row=0, column=4, sticky=W+E)
        # ---
        self.labelRgbLed = Label(self, text="RGB LED")
        self.labelRgbLed.grid(row=1, column=0)
        self.entryRgbLedR = Entry(self, width=5)
        self.entryRgbLedR.grid(row=1, column=1)
        self.entryRgbLedR.insert(0, "0x00")
        self.entryRgbLedG = Entry(self, width=5)
        self.entryRgbLedG.grid(row=1, column=2)
        self.entryRgbLedG.insert(0, "0x00")
        self.entryRgbLedB = Entry(self, width=5)
        self.entryRgbLedB.grid(row=1, column=3)
        self.entryRgbLedB.insert(0, "0x00")
        self.buttonRgbLedSet = Button(self, text="Set RGB LED", command=self.rgb_led_set)
        self.buttonRgbLedSet.grid(row=1, column=4, sticky=W+E)
        # ---
        self.buttonQuit = Button(self, text="Quit", command=self.quit)
        self.buttonQuit.grid(row=2, column=0, columnspan=5, sticky=W+E)

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

    # Quit.
    def quit(self):
        exit()



# Launch the GUI.
def launch_gui(dev):
    root = Tk()
    root.geometry("400x300")
    pyMcuGui = PyMcuGui(root)
    pyMcuGui.init_hw(dev)
    root.mainloop()



if __name__ == "__main__":
    launch_gui('/dev/ttyUSB0')

