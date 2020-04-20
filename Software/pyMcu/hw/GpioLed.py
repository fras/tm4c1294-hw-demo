# File: GpioLed.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Mar 2020
# Rev.: 17 Apr 2020
#
# Python class for setting the LEDs connected to GPIO ports of the
# TM4C1294NCPDT MCU.
#



import McuSerial



class GpioLed:

    # Message prefixes.
    prefixError = "ERROR: {0:s}: ".format(__file__)
    prefixDebug = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.

    # Hardware parameters.
    ledMask = 0x0f



    # Initialize the GPIO LEDs.
    def __init__(self, mcuSer):
        self.mcuSer = mcuSer
        self.errorCount = 0



    # Set the GPIO LEDs.
    def set(self, val):
        led = val & self.ledMask
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Setting the LEDs to 0x{0:02x}.".format(led))
        cmd = "led 0x{0:02x}".format(led)
        # Debug: Show command.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Sending command for LEDs: " + cmd)
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
            print(self.prefixError + "Error sending command for LEDs!")
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret
        return 0

