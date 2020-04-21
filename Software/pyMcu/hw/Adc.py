# File: Adc.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Mar 2020
# Rev.: 21 Apr 2020
#
# Python class for reading analog values of a joystick and an 3-axes
# accelerometer connected to ADC ports of the TM4C1294NCPDT MCU.
#



import McuSerial



class Adc:

    # Message prefixes.
    prefixError = "ERROR: {0:s}: ".format(__file__)
    prefixDebug = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.



    # Initialize the ADCs.
    def __init__(self, mcuSer):
        self.mcuSer = mcuSer
        self.joystickX = 0
        self.joystickY = 0
        self.accelX = 0
        self.accelY = 0
        self.accelZ = 0



    # Read the ADC values
    def read(self):
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading the ADC values.")
        cmd = "adc"
        # Debug: Show command.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Sending command for ADC values: " + cmd)
        # Send command.
        self.mcuSer.send(cmd)
        # Debug: Show response.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Response from MCU:")
            print(self.mcuSer.get_full())
        # Evaluate response.
        ret = self.mcuSer.eval()
        if ret:
            print(self.prefixError + "Error sending command for ADC values!")
            if self.debugLevel >= 1:
                print(self.prefixDebug + "Command sent to MCU: " + cmd)
                print(self.prefixDebug + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret
        resp = self.mcuSer.get()
        # Get list of ADC values.
        data = list(filter(None, ''.join(filter(lambda x: x.isdigit() or x == " ", resp)).split(" ")))
        self.joystickX = int(data[0], 0)
        self.joystickY = int(data[1], 0)
        self.accelX = int(data[2], 0)
        self.accelY = int(data[3], 0)
        self.accelZ = int(data[4], 0)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "ADC values:", end='')
            print(" Joystick:", end='')
            print(" X = {0:4d}".format(self.joystickX), end='')
            print(" Y = {0:4d}".format(self.joystickY), end='')
            print(" ; Accelerometer:", end='')
            print(" X = {0:4d}".format(self.accelX), end='')
            print(" Y = {0:4d}".format(self.accelY), end='')
            print(" Z = {0:4d}".format(self.accelZ), end='')
            print()
        return 0

