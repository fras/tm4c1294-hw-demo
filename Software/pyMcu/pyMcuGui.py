#!/usr/bin/env python3
#
# File: pyMcuGui.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Mar 2020
# Rev.: 17 Apr 2020
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
from tkinter import messagebox

# Hardware classes.
import Adc
import GpioButton
import GpioLed
import I2CTmp006
import I2COpt3001
import McuI2C
import McuSerial
import McuUart
import RgbLed



# GUI class.
class PyMcuGui(Frame):

    # Software version.
    swName      = "pyMCU"
    swVersion   = "0.1.1"
    swDate      = "17 Apr 2020"

    # Window titles.
    titleMain   = swName + " GUI - v" + swVersion + " - " + swDate
    titleError  = swName + " GUI - ERROR"

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
        self.master.title(self.titleMain)
        self.master.resizable(True, True)
        self.grid(padx=5, pady=5)
        # ***** Firmware info. *****
        self.frameFwInfo = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameFwInfo.grid(row=0, column=0, sticky=W+E, pady=2)
        self.labelFwInfo = Label(self.frameFwInfo, text="", anchor=W, justify=LEFT)
        self.labelFwInfo.grid(row=0, column=0, sticky=W+E)
        self.buttonFwInfoUpdate = Button(self.frameFwInfo, text="Update\nFW Info", command=self.mcu_fw_info)
        self.buttonFwInfoUpdate.grid(row=0, column=1, sticky=W+E, padx=10)
        # ***** GPIO buttons. *****
        self.frameButton = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameButton.grid(row=1, column=0, sticky=W+E, pady=2)
        self.labelButton = Label(self.frameButton, text="Button", anchor=CENTER, justify=CENTER)
        self.labelButton.grid(row=0, column=0, sticky=W+E)
        self.labelButtonStatus = Label(self.frameButton, text="Status", anchor=CENTER, justify=CENTER)
        self.labelButtonStatus.grid(row=0, column=1, sticky=W+E)
        self.labelButtonPressedCnt = Label(self.frameButton, text="Pressed Count", anchor=CENTER, justify=CENTER)
        self.labelButtonPressedCnt.grid(row=0, column=2, sticky=W+E)
        self.labelButtonReleasedCnt = Label(self.frameButton, text="Released Count", anchor=CENTER, justify=CENTER)
        self.labelButtonReleasedCnt.grid(row=0, column=3, sticky=W+E)
        self.labelButtonName = []
        buttonNames = ["USR_SW1", "USR_SW2", "EDU_S1", "EDU_S2"]
        self.entryButtonStatus = []
        self.entryButtonPressedCnt = []
        self.entryButtonReleasedCnt = []
        for i in range (0, GpioButton.GpioButton.buttonNum):
            self.labelButtonName.append(Label(self.frameButton, text=buttonNames[i] + " ({0:d})".format(i), anchor=W, justify=LEFT, width = 12))
            self.labelButtonName[i].grid(row=i+1, column=0, sticky=W+E)
            self.entryButtonStatus.append(Entry(self.frameButton, width=10, justify=RIGHT, state="readonly"))
            self.entryButtonStatus[i].grid(row=i+1, column=1, sticky=W+E)
            self.entryButtonPressedCnt.append(Entry(self.frameButton, width=14, justify=RIGHT, state="readonly"))
            self.entryButtonPressedCnt[i].grid(row=i+1, column=2, sticky=W+E)
            self.entryButtonReleasedCnt.append(Entry(self.frameButton, width=14, justify=RIGHT, state="readonly"))
            self.entryButtonReleasedCnt[i].grid(row=i+1, column=3, sticky=W+E)
        self.buttonButtonGet = Button(self.frameButton, text="Get Button Status", command=self.gpio_button_get)
        self.buttonButtonGet.grid(row=1, rowspan=2, column=4, sticky=W+E, padx=10)
        self.varButtonAutoUpdate = IntVar()
        self.checkbuttonButtonUpdate = Checkbutton(self.frameButton, text="Auto Update", variable=self.varButtonAutoUpdate)
        self.checkbuttonButtonUpdate.grid(row=3, column=4, sticky=W, padx=5)
        # ***** GPIO LEDs. *****
        self.frameLed = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameLed.grid(row=2, column=0, sticky=W+E, pady=2)
        self.labelGpioLed = Label(self.frameLed, text="GPIO LEDs", anchor=W, width=10)
        self.labelGpioLed.grid(row=0, column=0, sticky=W+E)
        self.entryGpioLed = Entry(self.frameLed, width=5)
        self.entryGpioLed.grid(row=0, column=1)
        self.entryGpioLed.insert(0, "0x0")
        self.buttonGpioLedSet = Button(self.frameLed, text="Set LEDs", command=self.gpio_led_set)
        self.buttonGpioLedSet.grid(row=0, column=4, sticky=W+E, padx=10)
        self.buttonGpioLedTest = Button(self.frameLed, text="Test LEDs", command=self.gpio_led_test)
        self.buttonGpioLedTest.grid(row=0, column=5, sticky=W+E, padx=0)
        # ***** RGB LED using PWM. *****
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
        self.buttonRgbLedTest = Button(self.frameLed, text="Test RGB LED", command=self.rgb_led_test)
        self.buttonRgbLedTest.grid(row=1, column=5, sticky=W+E, padx=0)
        # ***** Analog (ADC) values. *****
        self.frameAnalog = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameAnalog.grid(row=3, column=0, sticky=W+E, pady=2)
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
        self.varAnalogAutoUpdate = IntVar()
        self.checkbuttonAnalogUpdate = Checkbutton(self.frameAnalog, text="Auto Update", variable=self.varAnalogAutoUpdate)
        self.checkbuttonAnalogUpdate.grid(row=2, column=4, sticky=W, padx=5)
        # ***** Sensors. *****
        self.frameSensor = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameSensor.grid(row=4, column=0, sticky=W+E, pady=2)
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
        self.varSensorAutoUpdate = IntVar()
        self.checkbuttonSensorUpdate = Checkbutton(self.frameSensor, text="Auto Update", variable=self.varSensorAutoUpdate)
        self.checkbuttonSensorUpdate.grid(row=2, column=4, sticky=W, padx=5)
        # ***** I2C. ******
        self.frameI2C = Frame(self, bd=2, relief=GROOVE, padx=5, pady=5)
        self.frameI2C.grid(row=5, column=0, sticky=W+E, pady=2)
        self.labelI2CBus = Label(self.frameI2C, text="I2C Bus", anchor=CENTER, padx=5)
        self.labelI2CBus.grid(row=0, column=0, sticky=W+E)
        self.labelI2CSlaveAdr = Label(self.frameI2C, text="Slave Adr.", anchor=CENTER, padx=5)
        self.labelI2CSlaveAdr.grid(row=0, column=1, sticky=W+E)
        self.labelI2CDataWr = Label(self.frameI2C, text="Data Write", anchor=CENTER, padx=5)
        self.labelI2CDataWr.grid(row=0, column=2, sticky=W+E)
        self.labelI2CDataRd = Label(self.frameI2C, text="Data Read", anchor=CENTER, padx=5)
        self.labelI2CDataRd.grid(row=0, column=3, sticky=W+E)
        self.varI2CBus = IntVar()
        self.optionMenuI2CBus = OptionMenu(self.frameI2C, self.varI2CBus, 0, 2)
        self.optionMenuI2CBus.grid(row=1, column=0, sticky=W+E)
        self.varI2CBus.set(2)
        self.entryI2CSlaveAdr = Entry(self.frameI2C, width=5, justify=CENTER)
        self.entryI2CSlaveAdr.grid(row=1, column=1, sticky=W+E)
        self.entryI2CSlaveAdr.insert(0, "0x40")
        self.entryI2CDataWr = Entry(self.frameI2C, width=18, justify=LEFT)
        self.entryI2CDataWr.grid(row=1, column=2, sticky=W+E)
        self.entryI2CDataWr.insert(0, "0x00")
        self.entryI2CDataRd = Entry(self.frameI2C, width=18, justify=LEFT, state="readonly")
        self.entryI2CDataRd.grid(row=1, column=3, sticky=W+E)
        self.buttonI2CAccess = Button(self.frameI2C, text="Execute", command=self.i2c_access)
        self.buttonI2CAccess.grid(row=1, column=4, sticky=W+E, padx=10)
        self.frameI2CDetail = Frame(self.frameI2C, bd=0, padx=5, pady=5)
        self.frameI2CDetail.grid(row=2, column=0, columnspan=5, sticky=W+E)
        self.varI2CRead = IntVar()
        self.checkbuttonI2CRead = Checkbutton(self.frameI2CDetail, text="Read", variable=self.varI2CRead)
        self.checkbuttonI2CRead.grid(row=0, column=0, sticky=W+E, padx=0)
        self.entryI2CDataRdCnt = Entry(self.frameI2CDetail, width=3, justify=CENTER)
        self.entryI2CDataRdCnt.grid(row=0, column=1, sticky=W+E)
        self.entryI2CDataRdCnt.insert(0, "2")
        self.labelI2CReadCnt = Label(self.frameI2CDetail, text="byte(s).", anchor=CENTER, padx=0)
        self.labelI2CReadCnt.grid(row=0, column=2, sticky=W+E)
        self.varI2CRepeatedStart = IntVar()
        self.checkbuttonI2CRepeatedStart = Checkbutton(self.frameI2CDetail, text="Repeated Start", variable=self.varI2CRepeatedStart)
        self.checkbuttonI2CRepeatedStart.grid(row=0, column=3, sticky=W+E, padx=5)
        self.varI2CStopCondition = IntVar()
        self.checkbuttonI2CStopCondition = Checkbutton(self.frameI2CDetail, text="Stop Condition", variable=self.varI2CStopCondition)
        self.checkbuttonI2CStopCondition.grid(row=0, column=4, sticky=W+E, padx=5)
        self.checkbuttonI2CStopCondition.select()
        self.varI2CQuickCommand = IntVar()
        self.checkbuttonI2CQuickCommand = Checkbutton(self.frameI2CDetail, text="Quick Command", variable=self.varI2CQuickCommand)
        self.checkbuttonI2CQuickCommand.grid(row=0, column=5, sticky=W+E, padx=5)
        self.frameI2CExtra = Frame(self.frameI2C, bd=0, padx=5, pady=5)
        self.frameI2CExtra.grid(row=3, column=0, columnspan=5, sticky=W+E)
        self.entryI2CDetect = Entry(self.frameI2CExtra, width=60, justify=LEFT, state="readonly")
        self.entryI2CDetect.grid(row=0, column=0, sticky=W+E)
        self.buttonI2CDetect = Button(self.frameI2CExtra, text="Detect Devices", command=self.i2c_detect)
        self.buttonI2CDetect.grid(row=0, column=1, sticky=W+E, padx=10)
        # ***** Quit button. *****
        self.buttonQuit = Button(self, text="Quit", command=self.quit)
        self.buttonQuit.grid(row=6, column=0, columnspan=5, sticky=W+E, pady=2)

    # Initialize the hardware.
    def init_hw(self, dev):
        # Open the MCU serial interface.
        self.mcuSer = McuSerial.McuSerial(dev)
        self.mcuSer.debugLevel = 0
        self.mcuSer.simulateHwAccess = False
        self.mcuSer.clear()
        # Define the MCU peripherals.
        self.mcuI2C0 = McuI2C.McuI2C(self.mcuSer, 0)
        self.mcuI2C0.debugLevel = 1
        self.mcuI2C2 = McuI2C.McuI2C(self.mcuSer, 2)
        self.mcuI2C2.debugLevel = 1
        self.mcuUart6 = McuUart.McuUart(self.mcuSer, 6)
        self.mcuUart6.debugLevel = 1
        self.adc = Adc.Adc(self.mcuSer)
        self.adc.debugLevel = 1
        self.gpioButton = GpioButton.GpioButton(self.mcuSer)
        self.gpioButton.debugLevel = 1
        self.gpioLED = GpioLed.GpioLed(self.mcuSer)
        self.gpioLED.debugLevel = 1
        self.rgbLED = RgbLed.RgbLed(self.mcuSer)
        self.rgbLED.debugLevel = 1
        self.i2cTmp006 = I2CTmp006.I2CTmp006(self.mcuI2C2, 0x40)
        self.i2cTmp006.debugLevel = 1
        self.i2cOpt3001 = I2COpt3001.I2COpt3001(self.mcuI2C2, 0x44)
        self.i2cOpt3001.debugLevel = 1
        self.mcu_fw_info()

    # Auxiliary function to set the text of a tkinter Entry element that is read-only.
    def entry_readonly_set_text(self, entry, text):
        try:
            entry['state'] = NORMAL
            entry.delete(0, END)
            entry.insert(0, text)
            entry['state'] = 'readonly'
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError setting the text of a tkinter Entry element:\n" + str(e))

    # Read and show the MCU firmware info.
    def mcu_fw_info(self):
        self.mcuSer.clear()
        self.mcuSer.send("info")
        self.labelFwInfo['text'] = "Firmware info:\n" + self.mcuSer.get().replace('\r', '')

    # Get the GPIO button status.
    def gpio_button_get(self):
        try:
            for i in range(0, self.gpioButton.buttonNum):
                buttonStatus = self.gpioButton.get(i)
                if len(buttonStatus) != 3:
                    raise Exception("Unable to get the full status of button {0:d}.".format(i))
                self.entry_readonly_set_text(self.entryButtonStatus[i], "0x{0:01x}".format(buttonStatus[0]))
                self.entry_readonly_set_text(self.entryButtonPressedCnt[i], "{0:d}".format(buttonStatus[1]))
                self.entry_readonly_set_text(self.entryButtonReleasedCnt[i], "{0:d}".format(buttonStatus[2]))
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError getting the GPIO button status:\n" + str(e))

    # Set the GPIO LEDs.
    def gpio_led_set(self):
        try:
            led = int(self.entryGpioLed.get(), 0)
            self.gpioLED.set(led)
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError setting the GPIO LEDs:\n" + str(e))

    # Test the GPIO LEDs.
    def gpio_led_test(self):
        try:
            led = 0x1
            for i in range(0, 5):
                self.gpioLED.set(led)
                time.sleep(0.1)
                led <<= 1
            for i in range(0, 3):
                self.gpioLED.set(0xf)
                time.sleep(0.1)
                self.gpioLED.set(0x0)
                time.sleep(0.1)
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError testing the GPIO LEDs:\n" + str(e))

    # Set the RGB LED.
    def rgb_led_set(self):
        try:
            self.rgbLED.set(
                int(self.entryRgbLedR.get(), 0),
                int(self.entryRgbLedG.get(), 0),
                int(self.entryRgbLedB.get(), 0))
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError setting the RGB LED:\n" + str(e))

    # Test the RGB LED.
    def rgb_led_test(self):
        try:
            for i in range(0, 0x80, 2):
                self.rgbLED.set(i, 0x00, 0x00)
            for i in range(0, 0x80, 2):
                self.rgbLED.set(0x00, i, 0x00)
            for i in range(0, 0x80, 2):
                self.rgbLED.set(0x00, 0x00, i)
            for i in range(0, 0x80, 2):
                self.rgbLED.set(i, i, 0x00)
            for i in range(0, 0x80, 2):
                self.rgbLED.set(i, 0x00, i)
            for i in range(0, 0x80, 2):
                self.rgbLED.set(0x00, i, i)
            for i in range(0, 0x80, 2):
                self.rgbLED.set(i, i, i)
            self.rgbLED.set(0x00, 0x00, 0x00)
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError testing the RGB LED:\n" + str(e))

    # Update the analog values.
    def analog_update(self):
        try:
            self.adc.read()
            self.entry_readonly_set_text(self.entryJoystickX, "{0:4d}".format(self.adc.joystickX))
            self.entry_readonly_set_text(self.entryJoystickY, "{0:4d}".format(self.adc.joystickY))
            self.entry_readonly_set_text(self.entryAccelX, "{0:4d}".format(self.adc.accelX))
            self.entry_readonly_set_text(self.entryAccelY, "{0:4d}".format(self.adc.accelY))
            self.entry_readonly_set_text(self.entryAccelZ, "{0:4d}".format(self.adc.accelZ))
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError updating the analog values:\n" + str(e))

    # Update the sensor values.
    def sensor_update(self):
        try:
            self.entry_readonly_set_text(self.entrySensorTmp006ManId, "0x{0:04x}".format(self.i2cTmp006.read_manufacturer_id()))
            self.entry_readonly_set_text(self.entrySensorTmp006DevId, "0x{0:04x}".format(self.i2cTmp006.read_device_id()))
            self.entry_readonly_set_text(self.entrySensorTmp006Value, "{0:9.5f} degC".format(self.i2cTmp006.read_temperature()))
            self.entry_readonly_set_text(self.entrySensorOpt3001ManId, "0x{0:04x}".format(self.i2cOpt3001.read_manufacturer_id()))
            self.entry_readonly_set_text(self.entrySensorOpt3001DevId, "0x{0:04x}".format(self.i2cOpt3001.read_device_id()))
            self.entry_readonly_set_text(self.entrySensorOpt3001Value, "{0:9.5f} lux".format(self.i2cOpt3001.read_illuminance()))
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError updating the sensor values:\n" + str(e))

    # Perform an I2C access.
    def i2c_access(self):
        try:
            # Get I2C options.
            if self.varI2CBus.get() == 0:
                mcuI2C = self.mcuI2C0
            elif self.varI2CBus.get() == 2:
                mcuI2C = self.mcuI2C2
            else:
                messagebox.showerror(self.titleError, "{0:s}:\nI2C bus {1:d} not supported!".format(self.prefixError, self.varI2CBus.get()))
                return -1
            i2cSlaveAdr = int(self.entryI2CSlaveAdr.get(), 0)
            i2cDataWr = [int(i, 0) for i in filter(None, self.entryI2CDataWr.get().split(" "))]
            i2cDataRdCnt = int(self.entryI2CDataRdCnt.get(), 0)
            i2cRead = self.varI2CRead.get()
            i2cRepeatedStart = self.varI2CRepeatedStart.get()
            i2cStopCondition = self.varI2CStopCondition.get()
            i2cQuickCommand = self.varI2CQuickCommand.get()
            i2cAccMode = (0x01 if i2cRead else 0x00) | \
                         (0x02 if i2cRepeatedStart else 0x00) | \
                         (0x04 if not i2cStopCondition else 0x00) | \
                         (0x08 if i2cQuickCommand else 0x00)
            if i2cQuickCommand:
                mcuI2C.ms_quick_cmd_adv(i2cSlaveAdr, i2cRead, i2cRepeatedStart)
            elif i2cRead:
                i2cDataRd = mcuI2C.ms_read_adv(i2cSlaveAdr, i2cDataRdCnt, i2cRepeatedStart, i2cStopCondition)
                i2cDataRdStr = ""
                for i in range(0, len(i2cDataRd)):
                    if i > 0: i2cDataRdStr += " "
                    i2cDataRdStr += "0x{0:02x}".format(i2cDataRd[i])
                self.entry_readonly_set_text(self.entryI2CDataRd, i2cDataRdStr)
            else:
                mcuI2C.ms_write_adv(i2cSlaveAdr, i2cDataWr, i2cRepeatedStart, i2cStopCondition)
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError during I2C access:\n" + str(e))

    # Detect devices on the I2C bus.
    def i2c_detect(self):
        try:
            if (self.varI2CBus.get() != 0) and (self.varI2CBus.get() != 2):
                messagebox.showerror(self.titleError, "{0:s}:\nI2C bus {1:d} not supported!".format(self.prefixError, self.varI2CBus.get()))
                return -1
            cmd = "i2c-det {0:d}".format(self.varI2CBus.get())
            # Temporarily increase the timeout of the serial device.
            serTimeoutBackup = self.mcuSer.ser.timeout
            self.mcuSer.ser.timeout = 0.01
            self.mcuSer.send(cmd)
            self.mcuSer.ser.timeout = serTimeoutBackup
            if self.mcuSer.eval():
                messagebox.showerror(self.titleError, "{0:s}:\nDetection of I2C devices on bus {1:d} failed!".format(self.prefixError, self.varI2CBus.get()))
                return -1
            self.entry_readonly_set_text(self.entryI2CDetect, self.mcuSer.get().lstrip(' \t').strip(' \n\r'))
        except Exception as e:
            messagebox.showerror(self.titleError, self.prefixError + "\nError detecting I2C devices:\n" + str(e))

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
        if pyMcuGui.varButtonAutoUpdate.get():
            pyMcuGui.gpio_button_get()
        if pyMcuGui.varAnalogAutoUpdate.get():
            pyMcuGui.analog_update()
        if pyMcuGui.varSensorAutoUpdate.get():
            pyMcuGui.sensor_update()
        # Lower CPU load when idle.
        if (not pyMcuGui.varButtonAutoUpdate.get() and
                not pyMcuGui.varAnalogAutoUpdate.get() and
                not pyMcuGui.varSensorAutoUpdate.get()):
            time.sleep(0.01)



if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='Access to Process some integers.')
    parser.add_argument('-d', '--device', action='store', type=str,
                        dest='serial_device', default='/dev/ttyUSB0',
                        help='Serial device to access the MCU.')
    args = parser.parse_args()
    launch_gui(args.serial_device)

