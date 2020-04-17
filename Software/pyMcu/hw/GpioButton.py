# File: GpioButton.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 17 Apr 2020
# Rev.: 17 Apr 2020
#
# Python class for getting the status of the buttons connected to GPIO ports of
# the TM4C1294NCPDT MCU.
#



import McuSerial



class GpioButton:

    # Message prefixes.
    prefixError = "ERROR: {0:s}: ".format(__file__)
    prefixDebug = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.

    # Hardware parameters.
    buttonNum = 4
    strButtonStatus = "status:"
    strButtonPressedCnt = "pressed count:"
    strButtonReleasedCnt = "released count:"



    # Initialize the GPIO buttons.
    def __init__(self, mcuSer):
        self.mcuSer = mcuSer
        self.errorCount = 0



    # Get the GPIO button status.
    def get(self, index):
        if index < 0 or index >= self.buttonNum:
            self.errorCount += 1
            print(self.prefixDebug + "Button index {0:d} outside of valid range 0..{1:d}.".format(index, self.buttonNum - 1))
            return []
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Getting the status of button {0:d}.".format(index))
        cmd = "button {0:d}".format(index)
        # Debug: Show command.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Sending command for buttons: " + cmd)
        # Send command.
        self.mcuSer.send(cmd)
        # Debug: Show response.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Response from MCU:")
            print(self.mcuSer.get_full())
        # Evaluate response.
        if self.mcuSer.eval():
            self.errorCount += 1
            print(self.prefixError + "Error sending command for buttons!")
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return []
        # Get and parse response from MCU.
        ret = self.mcuSer.get()
        buttonStatusPos = ret.find(self.strButtonStatus)
        buttonPressedCntPos = ret.find(self.strButtonPressedCnt)
        buttonReleasedCntPos = ret.find(self.strButtonReleasedCnt)
        if (buttonStatusPos < 0 or
                buttonPressedCntPos < 0 or
                buttonReleasedCntPos < 0):
            self.errorCount += 1
            print(self.prefixError + "Error parsing the GPIO button status for button {0:d}!".format(index))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return []
        buttonStatusPos += len(self.strButtonStatus)
        buttonPressedCntPos += len(self.strButtonPressedCnt)
        buttonReleasedCntPos += len(self.strButtonReleasedCnt)
        buttonStatus = int(ret[buttonStatusPos:ret.find(self.strButtonPressedCnt)].replace(',', ''), 0)
        buttonPressedCnt = int(ret[buttonPressedCntPos:ret.find(self.strButtonReleasedCnt)].replace(',', ''), 0)
        buttonReleasedCnt = int(ret[buttonReleasedCntPos:], 0)

        return [buttonStatus, buttonPressedCnt, buttonReleasedCnt]

