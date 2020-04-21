# File: RgbLed.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Mar 2020
# Rev.: 21 Apr 2020
#
# Python class for setting an RGB LED connected to PWM ports of the
# TM4C1294NCPDT MCU.
#



import McuSerial



class RgbLed:

    # Message prefixes.
    prefixError = "ERROR: {0:s}: ".format(__file__)
    prefixDebug = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.



    # Initialize the RGB LED.
    def __init__(self, mcuSer):
        self.mcuSer = mcuSer
        self.errorCount = 0



    # Set the RGB LED.
    def set(self, valR, valG, valB):
        rgb = ((valR & 0xff) << 16) | ((valG & 0xff) << 8) | (valB & 0xff)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Setting the RGB LED to {0:3d}, {1:3d}, {2:3d} = 0x{3:06x}: ".\
                format(valR & 0xff, valG & 0xff, valB & 0xff, rgb))
        cmd = "rgb 0x{0:02x}".format(rgb)
        # Debug: Show command.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Sending command for RGB LED: " + cmd)
        # Send command.
        self.mcuSer.send(cmd)
        # Debug: Show response.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Response from MCU:")
            print(self.mcuSer.get_full())
        # Evaluate response.
        ret = self.mcuSer.eval()
        if ret:
            self.errorCount += 1
            print(self.prefixError + "Error sending command for RGB LED!")
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret
        return 0

